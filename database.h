#include "Common.h"
#include <pqxx/pqxx>

string attribute =" (    "			\
  " ID    INT PRIMARY KEY, "	\
  " SYM   TEXT   NOT NULL, "			\
  " ID    INT            NOT NULL, "		\
  " PRICE INT   NOT NULL, "			\
  " AMOUNT        INT             NOT NULL );";


class database {
public:
  static unique_ptr<connection> c;
  static void add_account();
  void run_command(string &sql);
}
