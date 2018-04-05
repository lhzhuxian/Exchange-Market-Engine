#include "database.h"
database::database() {
  c = new connection("dbname=enginedb user=postgres password = passw0rd");
  
}
database::~database() {
  delete c;
}

int database::check_order(int id, int amount, int price, string &sym) {
  if (amount == 0 || price < 0) {
    return 0;
  }
  string sql = "SELECT balance FROM ACCOUNT WHERE id =" + to_string(id) + ";";
  result R = run_query(sql);
  if (R.empty()) {
    return 0;
  }
  if (amount > 0) {
    int balance = R[0][0].as<int>();
    if ((amount * price) > balance) return 0;
    // valid order, keep the money
    alter_balance(-amount * price, id);
  } else {
    sql = "SELECT pos FROM SYMBOL WHERE id = " + to_string(id) + " AND sym = '" + sym + "'";
    R = run_query(sql);
    int pos =  R[0][0].as<int>();
    if (R.empty() || pos + amount < 0) return 0;
    // valid order, keep the shares
    alter_pos(pos + amount, id, sym);
  }
  return 1;
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
    sql = "SELECT price, amount, account, id FROM " + sym + " WHERE price = (SELECT MIN(price) FROM " + sym + ");";
  } else {
    sql = "SELECT price, amount, account, id FROM " + sym + " WHERE price = (SELECT MAX(price) FROM " + sym + ");";
  }
  return run_query(sql);
}

void database::delete_tuple (int id) {
  string sql = "DELETE FROM TORDER WHERE id = " + to_string(id) + ";";
  run_command(sql);
}

void database::change_tuple (int id, int amount, string sym) {
  string sql = "UPDATE TORDER" \
    "SET amount = " + to_string(amount) + " WHERE id = " + to_string(id) + ";";
  sql += "UPDATE " +  sym + \
    "SET amount = " + to_string(amount) + " WHERE id = " + to_string(id) + ";";
  run_command(sql);
}


string database::add_new_account(int id, int balance) {
  string sql = "INSERT INTO ACCOUNT(ID, BALANCE) "			\
    "VALUES (" + to_string(id) + ", " + to_string(balance) + ");";

  try {
    run_command(sql);
    return "success";
  } catch (const std::exception &e) {
    return e.what();
  }
}

string database::add_new_symbol(string & sym, vector<int> &id, vector<int> &pos, int n) {
  string sql;
  for(int i = 0; i < n; ++i) {
    sql += "INSERT INTO SYMBOL(SYM, ID, POS) "			\
      "VALUES ('" + sym + "'," + to_string(id[i]) + "," + to_string(pos[i]) + ");";
  }
  sql += "CREATE TABLE S"+ sym + ATT;
  sql += "CREATE TABLE B"+ sym + ATT;
  
  try {
    run_command(sql);
    return "success";
  } catch (const std::exception &e) {
    return e.what();
  }

}

void database::insert_order(string sym, int id, int price, int amount) {
  string sql;
  sql += "INSERT INTO TORDER(ID, ACCOUNT, SYM,PRICE, AMOUNT) "		\
    "VALUES (" + to_string(order_id) + "," + to_string(id) + ", '" + sym + "' ," + to_string(price) + "," + to_string(amount) + ");";
  sql += "INSERT INTO " + sym + "(ID, ACCOUNT, SYM, PRICE, AMOUNT) "	\
    "VALUES (" + to_string(order_id) + "," + to_string(id) + ", '" + sym + "' ," + to_string(price) + "," + to_string(amount) + ");";
  run_command(sql);
  order_id++; 
}

int database::change_border(string &sym, int s_am, int s_p, int s_ac,  int b_am, int b_p, int b_ac, int order_id){
  int deal;
  if (-s_am >= b_am) {
    deal = b_am;
    delete_tuple (order_id);
  } else {
    deal = -s_am;
    change_tuple (order_id, b_am + s_am, "b" + sym);
  }
  int s_balance = deal * s_p;
  int b_balance = b_p * deal - s_balance;
  
  // alter the balance of two account
  alter_balance(s_balance, s_ac);
  alter_balance(b_balance, b_ac);
  
  // alter the pos of sym in two account

  alter_pos(deal, b_ac, sym);
  
}

int database::change_sorder(string &sym, int s_am, int s_p, int s_ac,  int b_am, int b_p, int b_ac, int order_id){
  int deal;
  if (-s_am >= b_am) {
    deal = b_am;
    delete_tuple (order_id);
  } else {
    deal = -s_am;
    change_tuple (order_id, b_am + s_am, "s" + sym);
  }
  int s_balance = deal * s_p;
  int b_balance = b_p * deal - s_balance;

  // alter the balance of two account
  alter_balance(s_balance, s_ac);
  alter_balance(b_balance, b_ac);

  // alter the pos of sym in two account

  alter_pos(deal, b_ac, sym);

}


int database::s_match_order(string & sym, int b_ac, int s_ac, int b_p, int s_p, int b_am, int &s_am, int o_id) {
  if (b_p < s_p) {
    insert_order("s" + sym, s_ac, s_p, s_am);
    return 0;
  } else {
    int tmp = s_am + b_am;
    if (tmp >= 0) {
      change_border(sym, s_am, s_p, s_ac, b_am, b_p, b_ac, o_id);
      return 0;
    } else {
      s_am = tmp;
      return 1;
    }
  }
}
			    
int database::b_match_order(string & sym, int b_ac, int s_ac, int b_p, int s_p, int &b_am, int s_am, int o_id) {
  if (b_p < s_p) {
    insert_order("b" + sym, b_ac, b_p, b_am);
    return 0;
  } else {
    int tmp = s_am + b_am;
    if (tmp <= 0) {
      change_sorder(sym, s_am, s_p, s_ac, b_am, b_p, b_ac, o_id);
      return 0;
    } else {
      b_am = tmp;
      return 1;
    }
  }
}


void database::add_sorder(string & sym, int s_id, int s_am, int s_p) {
   if (!check_order(s_id, s_am, s_p, sym)){
     cerr << "invalid order\n";
     return;
   }
   int flag = 1;
   string dname = 'b' + sym;
   while(flag) {
    result lp = get_price(dname);
    if (lp.empty()) {
      flag = 0;
      insert_order("s" + sym, s_id, s_p, s_am);
    }
    for(auto it:lp) {
      int b_p = it[0].as<int>();
      int b_a = it[1].as<int>();
      int b_id = it[2].as<int>();
      int o_id = it[3].as<int>();
      flag = s_match_order(sym, b_id, s_id, b_p, s_p, b_a, s_am, o_id);
      if (flag = 0) break;	
    }
   }
}
 
void database::add_border(string & sym, int b_id, int b_am, int b_p) {
  if (!check_order(b_id, b_am, b_p, sym)){
    cerr << "invalid order\n";
    return;
  }
  int flag = 1;
  string dname = 's' + sym;
  while(flag) {
    result lp = get_price(dname);
    if (lp.empty()) {
      flag = 0;
      insert_order("b" + sym, b_id, b_p, b_am); 
    }
    for(auto it:lp) {
      int s_p = it[0].as<int>();
      int s_a = it[1].as<int>();
      int s_id = it[2].as<int>();
      int o_id = it[3].as<int>();
      flag = b_match_order(sym, b_id, s_id, b_p, s_p, b_am, s_a, o_id);
      if (flag = 0) break;
    }
  }  
}

result database::query(int id) {
  string sql = "SELECT * FROM TORDER WHERE id = " + to_string(id) + ";";
  return run_query(sql);
}

void database::cancel(int id) {
  result R = query(id);
  if (R.empty()) {
    // response
    return;
  }
  int account = R[0][1].as<int>();
  string sym = R[0][2].as<string>();
  int price = R[0][3].as<int>();
  int amount = R[0][4].as<int>();
  if (amount < 0) {
    alter_pos(-amount , account, sym);
  } else {
    alter_balance(amount * price, account); 
  }
      
  delete_tuple(id);
    
}
