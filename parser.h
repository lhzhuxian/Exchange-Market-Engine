#include "Common.h"
//#include "database.h"
class Parser {
 private:
  pugi::xml_document doc;
  int type;
  
 public:
  Parser(char *buf, int size);
  //determine type
  void start();

  void create_handler();

  void transcation_handler();
  
};
  
