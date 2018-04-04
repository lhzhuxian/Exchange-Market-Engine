#include "parser.h"


Parser::Parser(char *buf, int size) {
  pugi::xml_parse_result result = doc.load_buffer_inplace(buf, size);
  cout << "parser created\n";
  if (!result){
    cout << "Error description: " << result.description() << "\n";
  }
  if(doc.child("create")) {
    type = 0;
  } else if (doc.child("transactions")){
    type = 1;
  } else {
    // error response
    cerr << "unvalid request\n";
  }
}

void Parser::start () {
  if (type) {
    transcation_handler();
  } else {
    create_handler();
  }
}

void Parser::create_handler() {
  pugi::xml_node create = doc.child("create");
  
  for (pugi::xml_node request: create.children())  {
    string name = request.name();
    if (name == "account") {
      cout << "account get"<<endl;
      int id = std::stoi(request.attribute("id").value());
      int balance = std::stoi(request.attribute("balance").value());
      //database::add_new_account(id, balance);
      cout<<"id="<<id<<" balance="<<balance<<endl;
    } else if (name == "symbol"){
      cout << "symbol get"<<endl;
      string sym = request.attribute("sym").value();
      vector<int> id;
      vector<int> pos;
      int n = 0;
      for (pugi::xml_node account: request.children()) {
	id.push_back(std::stoi(account.attribute("id").value()));
	cout<<"account id = "<<account.attribute("id").value()<<endl;
	pos.push_back(std::stoi(account.child_value()));
	cout<<"amount on this account = "<<account.child_value()<<endl;
	++n;
      }
      //database::add_new_symbol(sym, id, pos, n);
    }
  }
}

void Parser::transcation_handler() {
  pugi::xml_node transcation = doc.child("transcation");
  for (pugi::xml_node request: transcation.children())  {
    if (request.name() == "order") {
      string sym = request.attribute("sym").value();
      int amount = std::stoi(request.attribute("amount").value());
      int limit = std::stoi(request.attribute("limit").value());
      if (amount > 0) {
	//database::add_border(sym, -amount, limit);
      } else if (amount < 0) {
	//database::add_sorder(sym, amount, limit);
      }
    } else if(request.name() == "cancel") {
      int id = std::stoi(request.attribute("id").value());
      //database::cancel(id);
    } else if(request.name() == "query") {
      int id = std::stoi(request.attribute("id").value());
      //database::query(id);
    }
  }

}




