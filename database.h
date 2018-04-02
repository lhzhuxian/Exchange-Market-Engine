#include "Common.h"
#include <pqxx/pgxx>



class database {
public:
  static unique_ptr<connection> c;
  static void add_account();
  void run_command(string &sql);
}
