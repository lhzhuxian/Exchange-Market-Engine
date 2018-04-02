#include "parser.h"
/*

pugi::xml_document doc;
pugi::xml_parse_result result = doc.load_buffer_inplace(buf, SIZE);


for (pugi::xml_node child: doc.children())  {
   cout << "child: " << child.name() << endl;
   for (pugi::xml_attribute attr: child.attributes()){
       cout << " " << attr.name() << "=" << attr.value() << endl;
   }
   for (pugi::xml_node sub_child: child.children()) {
       cout << "sub_child: " << sub_child.name() << endl;
 
       for (pugi::xml_attribute attr: sub_child.attributes()){
           cout << " " << attr.name() << "=" << attr.value() << endl;
       }
   }
}
cout << endl;*/

Parser::Parser(char *buf, int size) {
  pugi::xml_parse_result result = doc.load_buffer_inplace(buf, SIZE);
  if (!result){
    cout << "XML [" << source << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
    cout << "Error description: " << result.description() << "\n";
    cout << "Error offset: " << result.offset << " (error at [..." << (source + result.offset) << "]\n\n";
  }
  if(doc.child("create")) {
    type = 0;
  } else {
    type = 1;
  }
}

void Paresr::start () {
  if (type) {
    transcation_handler();
  } else {
    create_handler();
  }
}

void Parser::create_handler() {
  pugi::xml_node create = doc.child("create");
  for (pugi::xml_node request: create.children())  {
    if (request.name() == "account") {
      int id = std::stoi(request.attribute("id").value());
      int balance = std::stoi(request.attribute("balance").value());
      database::add_new_account(id, balance);
    } else if (request.name() == "symbol"){
      string sym = request.attribute("sym").value();
      vector<int> id;
      vector<int> pos;
      int n = 0;
      for (pugi::xml_node account: request.children()) {
	id.push_back(std::stoi(account.attribute("id").value()));
	pos.push_back(std::stoi(account.text()));
	++n;
      }
      database::add_new_symbol(sym, id, pos, n);
    }
  }
}

void Parser::transcation_handler() {
  pugi::xml_node transcation = doc.child("transcation");
  for (pugi::xml_node request: transcatopm.children())  {
    if (request.name() == "order") {
      
    } else if(request.name() == "cancel") {

    } else if{request.name() == "query") {
      
    }
  }

}




