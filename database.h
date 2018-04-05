#ifndef _DATABASE_H__
#define _DATABASE_H__

#include "Common.h"


#define ATT " (    "			\
  " ID    INT PRIMARY KEY REFERENCES TORDER(ID) ON UPDATE CASCADE ON DELETE CASCADE, "	\
  " ACCOUNT INT REFERENCES ACCOUNT(ID), "       \
  " SYM   TEXT   NOT NULL, "			\
  " PRICE INT   NOT NULL, "			\
  " AMOUNT        INT             NOT NULL );"



class database {
private:
  connection * c;
  
  
public:
  database();
  ~database();
  int check_order(int id, int amount, int price, string & sym);
  void alter_pos(int pos, int id, string & sym);
  void alter_balance(int balance, int id);
  result run_query(string &sql);
  void insert_order(string sym, int id, int price, int amount);
  result get_price(string &sym);
  int run_command(string &sql);
  void delete_tuple(int id);
  void change_tuple (int id, int amount, string sym);
  string add_new_account(int id, int balance);
  string add_new_symbol(string & sym, vector<int> &id, vector<int> &pos, \
		      int n);
  int change_border(string &sym, int s_am, int s_p, int s_ac,  int b_am, int b_p, int b_ac, int order_id);
  int change_sorder(string &sym, int s_am, int s_p, int s_ac,  int b_am, int b_p, int b_ac, int order_id);
  int s_match_order(string & sym, int b_ac, int s_ac, int b_p, int s_p, int b_am, int &s_am, int o_id);
  int b_match_order(string & sym, int b_ac, int s_ac, int b_p, int s_p, int &b_am, int s_am, int o_id);
  void add_sorder(string & sym, int id, int amount, int price);
  void add_border(string & sym, int id, int amount, int price);
  result query(int id);
  void cancel(int id);
};

#endif
