#ifndef _DATABASE_H__
#define _DATABASE_H__

#include "Common.h"


#define SELLATT " (    "			\
  " ID    INT PRIMARY KEY REFERENCES TORDER(ID) ON UPDATE CASCADE ON DELETE CASCADE, "	\
  " ACCOUNT INT REFERENCES ACCOUNT(ID), "       \
  " SYM   TEXT   NOT NULL, "			\
  " PRICE INT   NOT NULL, "			\
  " AMOUNT        INT             NOT NULL ," \
  "order by price asc);"

#define BUYATT " (    "                    \
  " ID    INT PRIMARY KEY REFERENCES TORDER(ID) ON UPDATE CASCADE ON DELETE CASCADE, "  \
  " ACCOUNT INT REFERENCES ACCOUNT(ID), "       \
  " SYM   TEXT   NOT NULL, "                    \
  " PRICE INT   NOT NULL, "                     \
  " AMOUNT        INT             NOT NULL ," \
  "order by price DESC);"



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
  void insert_order(string db, string sym, int id, int price, int amount, int oid);
  void excuted_order(int oid, int account, string sym, int b_price, int s_price, int amount);
  int match_order(string type, string & sym, int b_ac, int s_ac, int b_p, int s_p, int b_am, int s_am, int s_oid, int b_oid) ;
  result get_price(string &sym);
  void run_command(string &sql);
  void delete_tuple(int id);
  void change_tuple (int id, int amount, string sym);
  string add_new_account(int id, int balance);
  string add_new_symbol(string & sym, vector<int> &id, vector<int> &pos, \
		      int n);
  int add_sorder(string & sym, int id, int amount, int price);
  int add_border(string & sym, int id, int amount, int price);
  void query(int id, pugi::xml_node & nodeRoot);
  void cancel(int id, pugi::xml_node & nodeRoot);
};

#endif
