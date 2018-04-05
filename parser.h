#ifndef _PARSER_H__
#define _PARSER_H__


#include "Common.h"
#include "database.h"
class Parser {
 private:
  pugi::xml_document res;
  pugi::xml_document doc;
  int type;
  database db;
 public:
  Parser(char *buf, int size);
  //determine type
  string get_response();

  void start();

  void create_response(string attr, string value, string &message);

  void create_handler( pugi::xml_node create);

  void transcation_handler( pugi::xml_node transcation);
  
};

struct xml_string_writer: pugi::xml_writer
{
    std::string result;
    
    virtual void write(const void* data, size_t size)
    {
        result.append(static_cast<const char*>(data), size);
    }
};


  
#endif
