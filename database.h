#ifndef _DATABASE_H__
#define _DATABASE_H__

#include "Common.h"


#define ATT " (    "			\
  " ID    INT PRIMARY KEY, "	\
  " SYM   TEXT   NOT NULL, "			\
  " ID    INT            NOT NULL, "		\
  " PRICE INT   NOT NULL, "			\
  " AMOUNT        INT             NOT NULL );"


class database {
private:
  connection * c;
public:
  database();
  ~database();
  result get_price(string &sym);
  void run_command(string &sql);
  void delete_tuple(int id);
  void change_tuple (int id, int amount);
  void add_new_account(int id, int balance);
  void add_new_symbol(string & sym, vector<int> &id, vector<int> &pos, \
				int n);
  void add_sorder(string & sym, int id, int amount, int price);
  void add_border(string & sym, int id, int amount, int price);
  result query(int id);
  void cancel(int id);
};

#endif
