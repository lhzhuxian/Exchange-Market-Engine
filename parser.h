#include "Common.h"
#include "database.h"
class Parser {
 private:
  pugi::xml_document doc;
  int type;
  
 public:
  Parser(char *buf, int size);
  //determine type
  void check();
  //
  pugi::xml_node create_accout();
  pugi::xml_node create_symbol();
  pugi::xml_node get_order();
  
}
  
