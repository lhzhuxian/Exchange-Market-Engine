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

static int database::get_price(int type, string & sym) {
  string sql;
  if (!type) {
    sql = "SELECT min(price) FROM SORDER WHERE sym = " + sym + ";";
  } else {
    sql = "SELECT max(price) FROM BORDER  WHERE sym = " + sym + ";";
  }
  nontransaction N(*c);
  result R(N.exec(sql));
  auto it = R.begin();
  return it[0].as<int>();
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
  run_command(sql);
}

static void database::add_sorder(string & sym, int id, int amount, int price) {
  int lp = get_price(0, sym);
  string sql;
  if (price >= lp) {
    sql = "INSERT INTO SORDER(SYM, ID, PRICE, AMOUNT) " \
      "VALUES (" sym + "," + to_string(id) + "," + to_string(price) + "," + to_string(amount) + ");";
  } else {
    
  }
  run_command(sql);
}

static void database::add_border(string & sym, int id, int amount, int price) {
  int hp = get_price(1, sym);
  string sql;
  if (price <= hp) {
    sql = "INSERT INTO BORDER(SYM, ID, PRICE, AMOUNT) " \
      "VALUES (" sym + "," + to_string(id) + "," + to_string(price) + "," + to_string(amount) + ");";
  } else {
    

  }
  run_command(sql);

}

  
