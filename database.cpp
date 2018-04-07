#include "database.h"

ssize_t GetCurrentStamp()
{
  boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
  boost::posix_time::time_duration time_from_epoch = boost::posix_time::second_clock::universal_time() - epoch;
  
  return time_from_epoch.total_seconds();
}

database::database() {
  c = new connection("dbname=enginedb user=postgres password = passw0rd");
  
}
database::~database() {
  delete c;
}

int database::check_order(int id, int amount, int price, string &sym) {
  if (amount == 0 || price < 0) {
    return -1;
  }
  string sql = "SELECT balance FROM ACCOUNT WHERE id =" + to_string(id) + ";";
  result R = run_query(sql);
  if (R.empty()) {
    return -1;
  }
  if (amount > 0) {
    int balance = R[0][0].as<int>();
    if ((amount * price) > balance) return -1;
    // valid order, keep the money
    alter_balance(-amount * price, id);
  } else {
    sql = "SELECT pos FROM SYMBOL WHERE id = " + to_string(id) + " AND sym = '" + sym + "'";
    R = run_query(sql);
    int pos =  R[0][0].as<int>();
    if (R.empty() || pos + amount < 0) return -1;
    // valid order, keep the shares
    alter_pos(amount, id, sym);
  }
  return order_id++;
}

void database::alter_pos(int pos, int id, string & sym) {
  string sql =  "UPDATE SYMBOL "					\
    "SET  pos = pos + " + to_string(pos) + " WHERE id = " + to_string(id) + " AND SYM = '" + sym + "' ;";
  run_command(sql);
}
void database::alter_balance(int balance, int id) {
  string sql =  "UPDATE ACCOUNT "						\
    "SET balance = balance + " + to_string(balance) + " WHERE id = " + to_string(id) + ";";
  run_command(sql);
}
result database::run_query(string &sql) {
  nontransaction N(*c);
  result R(N.exec(sql));
  return R;
}
void database::run_command(string &sql) {
    work W(*c);
    W.exec(sql);
    W.commit();
}

result database::get_price(string &sym) {
  //get the highest price in border or the lowest price in sorder
  string sql;
  if (sym[0] == 's') {
    sql = "SELECT price, amount, account, id FROM " + sym + " WHERE price = (SELECT MIN(price) FROM " + sym + ") ORDER BY ID ASC;";
  } else {
    sql = "SELECT price, amount, account, id FROM " + sym + " WHERE price = (SELECT MAX(price) FROM " + sym + ") ORDER BY ID ASC;";
  }
  return run_query(sql);
}

void database::delete_tuple (int id) {
  try{
    string sql = "DELETE FROM TORDER WHERE id = " + to_string(id) + ";";
    run_command(sql);
  }  catch (const std::exception &e) {
    cout << e.what() << endl;
  }

}

void database::change_tuple (int id, int amount, string sym) {
  string sql = "UPDATE TORDER" \
    " SET amount = " + to_string(amount) + " WHERE id = " + to_string(id) + ";";
  sql += "UPDATE " +  sym + \
    " SET amount = " + to_string(amount) + " WHERE id = " + to_string(id) + ";";
  run_command(sql);
}


string database::add_new_account(int id, int balance) {
  string sql = "INSERT INTO ACCOUNT(ID, BALANCE) "			\
    "VALUES (" + to_string(id) + ", " + to_string(balance) + ");";

  try {
    run_command(sql);
    return "success";
  } catch (const std::exception &e) {
    return "Account Already Exits";
  }
}

string database::add_new_symbol(string & sym, vector<int> &id, vector<int> &pos, int n) {
  string sql;
  for(int i = 0; i < n; ++i) {
    sql += "INSERT INTO SYMBOL(SYM, ID, POS) "			\
      "VALUES ('" + sym + "'," + to_string(id[i]) + "," + to_string(pos[i]) + ");";
  }
  sql += "CREATE TABLE S"+ sym + SELLATT;
  sql += "CREATE TABLE B"+ sym + BUYATT;
  
  try {
    run_command(sql);
    return "success";
  } catch (const std::exception &e) {
    return "Invalid Account id or Position";
  }

}

void database::insert_order(string db, string sym, int id, int price, int amount, int oid) {
  string sql;
  sql += "INSERT INTO TORDER(ID, ACCOUNT, SYM,PRICE, AMOUNT) "		\
    "VALUES (" + to_string(oid) + "," + to_string(id) + ", '" + sym + "' ," + to_string(price) + "," + to_string(amount) + ");";
  sql += "INSERT INTO " + db + "(ID, ACCOUNT, SYM, PRICE, AMOUNT) "	\
    "VALUES (" + to_string(oid) + "," + to_string(id) + ", '" + sym + "' ," + to_string(price) + "," + to_string(amount) + ");";
  run_command(sql);
}


void database::excuted_order(int oid, int account, string sym, int b_price, int s_price, int amount) {
  ssize_t epoch = GetCurrentStamp();;
  string sql = "INSERT INTO EXCUTED(ID, ACCOUNT, SYM, PRICE, AMOUNT, EPOCH) " \
    "VALUES (" + to_string(oid) + ", " + to_string(account) + ", '" + sym + "', " + to_string(s_price) + ", " + to_string(amount) + ", " + to_string(epoch) + ");";
  run_command(sql);
  if (amount > 0) {
    alter_balance((b_price - s_price) * amount, account);
  } else {
    alter_balance(-s_price * amount, account);
  }
  alter_pos(amount, account, sym);
}

int database::match_order(string type, string & sym, int b_ac, int s_ac, int b_p, int s_p, int b_am, int s_am, int b_oid, int s_oid) {
  
  if (b_p < s_p) {
    if (!type.compare("s")) {
      insert_order(type + sym, sym,  s_ac, s_p, s_am, s_oid);
    } else {
      insert_order(type + sym, sym,  b_ac, b_p, b_am, b_oid);
    }
    return -1;
  } else {
    
    if (b_am + s_am == 0) {
      excuted_order(s_oid, s_ac, sym, b_p,s_p, s_am);
      excuted_order(b_oid, b_ac, sym, b_p,s_p, b_am);
      // only need to delete the opposite order, since the new order is not in the database

      if (!type.compare("s")) {

	delete_tuple(b_oid);
      } else {

	delete_tuple(s_oid);
      }
      return -1;
    } else if (b_am + s_am > 0) {
      excuted_order(s_oid, s_ac, sym, b_p, s_p, s_am);
      excuted_order(b_oid, b_ac, sym, b_p, s_p, -s_am);
      if (!type.compare("s")) {
	change_tuple (b_oid, b_am + s_am, 'b' + sym);
	return -1;
      } else {
        delete_tuple(s_oid);
	return b_am + s_am;
      }

    } else  {
      excuted_order(s_oid, s_ac, sym, b_p, s_p, -b_am);
      excuted_order(b_oid, b_ac, sym, b_p, s_p, b_am);
      if (!type.compare("s")) {
	  delete_tuple(b_oid);
	  return b_am + s_am;
      } else {
	change_tuple (s_oid, b_am + s_am, 's' + sym);
	return -1;
	
      }
      
    }
    
  }
}


int database::add_sorder(string & sym, int s_id, int s_am, int s_p) {
  int s_oid = check_order(s_id, s_am, s_p, sym);
  if (s_oid == -1){
     cerr << "invalid order\n";
     return -1;
   }
   int flag = 1;
   string dname = 'b' + sym;
   while(flag) {
    result lp = get_price(dname);
    if (lp.empty()) {
      flag = 0;
      insert_order("s" + sym, sym, s_id, s_p, s_am, s_oid);
    }
    for(auto it:lp) {
      int b_p = it[0].as<int>();
      int b_am = it[1].as<int>();
      int b_id = it[2].as<int>();
      int b_oid = it[3].as<int>();
      s_am = match_order("s", sym, b_id, s_id, b_p, s_p, b_am, s_am, b_oid, s_oid);
      if (s_am == -1) {
	flag = 0;
	break;
      }
    }
   }
   return s_oid;
}

int database::add_border(string & sym, int b_id, int b_am, int b_p) {
  int b_oid = check_order(b_id, b_am, b_p, sym);
  if (b_oid == -1){
    cerr << "invalid order\n";
    return -1;
  }
  int flag = 1;
  string dname = 's' + sym;
  while(flag) {
    result lp = get_price(dname);
    if (lp.empty()) {
      flag = 0;
      insert_order("b" + sym,sym,  b_id, b_p, b_am, b_oid); 
    }
    for(auto it:lp) {
      int s_p = it[0].as<int>();
      int s_am = it[1].as<int>();
      int s_id = it[2].as<int>();
      int s_oid = it[3].as<int>();
      b_am = match_order("b", sym, b_id, s_id, b_p, s_p, b_am, s_am, b_oid, s_oid);
      if (b_am == -1) {
        flag = 0;
        break;
      }
    }
  }
  return b_oid;
}

void database::query(int id,  pugi::xml_node &nodeRoot) {
  string ts = "SELECT AMOUNT FROM TORDER WHERE id = " + to_string(id) + ";";
  string es = "SELECT AMOUNT, PRICE, EPOCH FROM EXCUTED WHERE id = " + to_string(id) + ";";
  string cs = "SELECT AMOUNT, EPOCH FROM CANCELED WHERE id = " + to_string(id) + ";";

  result torder = run_query(ts);
  result excuted = run_query(es);
  result canceled = run_query(cs);

  if (torder.empty() && excuted.empty() && canceled.empty()) {
    pugi::xml_node ans = nodeRoot.append_child("error");
    ans.append_child(pugi::node_pcdata).set_value("Invalid Order Id");
    nodeRoot.append_copy(ans);
    return;
  }
  pugi::xml_node ans = nodeRoot.append_child("status");
  if (!torder.empty()) {
    pugi::xml_node tmp = ans.append_child("open");
    tmp.append_attribute("shares") = torder[0][0].as<string>().data();
  }
  if (!excuted.empty()) {
    for(auto it:excuted){
      pugi::xml_node tmp = ans.append_child("executed");
      tmp.append_attribute("shares") = it[0].as<string>().data();
      tmp.append_attribute("price") = it[1].as<string>().data();
      tmp.append_attribute("time") = it[2].as<string>().data();
    }
  }
  if (!canceled.empty()) {
    pugi::xml_node tmp = ans.append_child("canceled");
    tmp.append_attribute("shares") = canceled[0][0].as<string>().data();
    tmp.append_attribute("TIME") = canceled[0][1].as<string>().data();
  }
  return;
}

void database::cancel(int id, pugi::xml_node &nodeRoot) {
  string ts = "SELECT * FROM TORDER WHERE id = " + to_string(id) + ";";
  string es = "SELECT AMOUNT, PRICE, EPOCH FROM EXCUTED WHERE id = " + to_string(id) + ";";
  result excuted = run_query(es);
  result R = run_query(ts);

  if (R.empty()) {
    pugi::xml_node ans = nodeRoot.append_child("error");
    ans.append_child(pugi::node_pcdata).set_value("Invalid Order Id");
    nodeRoot.append_copy(ans);
    return;
  }
  pugi::xml_node ans = nodeRoot.append_child("status");
  if (!excuted.empty()) {
    for(auto it:excuted){
      pugi::xml_node tmp = ans.append_child("executed");
      tmp.append_attribute("shares") = it[0].as<string>().data();
      tmp.append_attribute("price") = it[1].as<string>().data();
      tmp.append_attribute("time") = it[2].as<string>().data();
    }
  }

  int account = R[0][1].as<int>();
  string sym = R[0][2].as<string>();
  int price = R[0][3].as<int>();
  int amount = R[0][4].as<int>();
  ssize_t epoch = GetCurrentStamp();
  if (amount < 0) {
    alter_pos(-amount , account, sym);
  } else {
    alter_balance(amount * price, account); 
  }
  string sql = "INSERT INTO CANCELED(ID, ACCOUNT, SYM, PRICE, AMOUNT, EPOCH) " \
    "VALUES (" + to_string(id) + ", " + to_string(account) + ", '" + sym + "', " + to_string(price) + ", " + to_string(amount) + ", " + to_string(epoch) + ");";
  delete_tuple(id);
  run_command(sql);

  pugi::xml_node tmp = ans.append_child("canceled");
  tmp.append_attribute("shares") = to_string(amount).data();
  tmp.append_attribute("TIME") = to_string(epoch).data();

  return;
}
