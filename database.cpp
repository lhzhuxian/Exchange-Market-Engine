#include "database.h"

static void databse::run_command(string &sql) {
  try {
    work W(*c);
    W.exec(sql);
    W.commit();
  } catch (const std::exception &e) {
    cerr << e.what() << endl;
    //exit(EXIT_FAILURE);
  }
}

static result database::get_price(string &sym) {
  string sql;
  if (sym[0] == 's') {
    //sql = "SELECT min(price) FROM " + sym + " WHERE ";
  } else {
    //sql = "SELECT max(price) FROM " + sym + " WHERE ";
  }
  nontransaction N(*c);
  result R(N.exec(sql));
  
  return R;
}

static void databaes::delete_tuple (int id) {
  string sql = "DELETE FROM TORDER WHERE id = " + stoi(id) ";";
  run_command(sql);
}

static void database::change_tuple (int id, int amount) {
  string sql = "UPDATE TORDER" \
    "SET amount = " + stoi(amount) + " WHERE id = " + stoi(id) + ";";
  run_command(sql);
}

static void database::add_new_account(int id, int balance) {
  string sql = "INSERT INTO ACCOUNT(ID, BALANCE) "\
    "VALUES (" + to_string(id) + ", " + to_string(balance) + ");";
  run_command(sql);
}
static void database::add_new_symbol(string & sym, vector<int> &id, vector<int> &pos, int n) {
  for(int i = 0; i < n; ++i) {
    string sql += "INSERT INTO SYMBOL(SYM, ID, POS) "			\
      "VALUES (" sym + "," + to_string(id[i]) + "," + to_string(pos[i]) + ");";
  }
  sql += "CREATE TABLE S"+ sym + attribute;
  sql += "CREATE TABLE B"+ sym + attribute;
  run_command(sql);
}


static void database::add_sorder(string & sym, int id, int amount, int price) {
  int flag = 1;
  while(flag) {
    lp = get_price('b' + sym);
    for(auto it:lp) {
      int b_p = it[0].as<int>();
      int b_a = it[1].as<int>();
      int b_id = it[2].as<int>();
      
      if (b_p >= price) {
	amount += b_a;
	if (amount < 0) {
	  delete_tuple(b_id);
	} else if (amount == 0) {
	  delete_tuple(b_id);
	  delete_tuple(id);
	  flag = 0;
	  break;
	} else {
	  change_tuple(b_id, amount);
	    flag = 0;
	    break;
	}
      } else {
	sql = "INSERT INTO S" + sym + "(SYM, ID, PRICE, AMOUNT) "	\
	  "VALUES (" sym + "," + to_string(id) + "," + to_string(price) + "," + to_string(amount) + ");";
	flag = 0;
	break;
      }
    }
  }
  run_command(sql);
}

static void database::add_border(string & sym, int id, int amount, int price) {
  int flag = 1;
  while(flag) {
    lp = get_price('s' + sym);
    for(auto it:lp) {
      int s_p = it[0].as<int>();
      int s_a = it[1].as<int>();
      int s_id = it[2].as<int>();

      if (s_p <= price) {
        amount += s_a;
        if (amount > 0) {
          delete_tuple(s_id);
        } else if (amount == 0) {
          delete_tuple(s_id);
          delete_tuple(id);
          flag = 0;
          break;
        } else {
          change_tuple(s_id, amount);
            flag = 0;
            break;
        }
      } else {
        sql = "INSERT INTO B" + sym + "(SYM, ID, PRICE, AMOUNT) "       \
          "VALUES (" sym + "," + to_string(id) + "," + to_string(price) + "," + to_string(amount) + ");";
        flag = 0;
        break;
      }
    }
  }
  run_command(sql);
}

static result database::query(int id) {
  string sql = "SELECT * FROM TORDER WHERE id = " + stoi(id) + ";";
  nontransaction N(*c);
  result R(N.exec(sql));
  return R;

}

static void database::cancel(int id) {
  delete_tuple(id);
}
