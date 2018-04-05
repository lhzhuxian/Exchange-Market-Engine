#include "parser.h"



Parser::Parser(char *buf, int size) {
  try {
    pugi::xml_node nodeRoot = res.append_child("results");
    pugi::xml_node pre = res.prepend_child(pugi::node_declaration);
    pre.append_attribute("version") = "1.0";
    pre.append_attribute("encoding") = "UTF-8";
    
    pugi::xml_parse_result result = doc.load_buffer_inplace(buf, size);
    if (!result){
      cout << "Error description: " << result.description() << "\n";
    }
  } catch (const std::exception& e) { // caught by reference to base
    cout << e.what() << endl;
  }
}

string Parser::start () {
  for (pugi::xml_node request: doc.children()) {
    if (!strcmp(request.name(),"create")) {
      create_handler(request);
    } else {
      transcation_handler(request);
    }
  }
 
}

string Parser::get_response() {
  struct xml_string_writer writer;
  res.save(writer);
  return writer.result;
}

void Parser::create_response(string attr, string value, string &message) {
  pugi::xml_node nodeRoot = res.child("results");
  if (message.compare("sucess")) {
    pugi::xml_node nodeError = res.append_child("error");
    nodeError.append_attribute(attr) = value;
    nodeError.append_child(pugi::node_pcdata).set_value(message);
  } else {
    pugi::xml_node nodeCreated = res.append_child("created");
    nodeCreated.append_attribute(attr) = value;
  }  
}
void Parser::create_handler(pugi::xml_node create) {

  for (pugi::xml_node request: create.children())  {
    string name = request.name();
    if (!strcmp(request.name(),"account")) {
      int id = std::stoi(request.attribute("id").value());
      int balance = std::stoi(request.attribute("balance").value());

      cout << "request: create account: " << id << " blance: " << balance << endl;

      string message = db.add_new_account(id, balance);
      create_response("id", to_string(id), message);
      //response
    } else if (!strcmp(request.name(),"symbol")){
      string sym = request.attribute("sym").value();
      vector<int> id;
      vector<int> pos;
      int n = 0;
      for (pugi::xml_node account: request.children()) {
	id.push_back(std::stoi(account.attribute("id").value()));
	pos.push_back(std::stoi(account.child_value()));
	++n;
      }

      cout << "request: create symbol: " << sym << endl;
      for (int i = 0; i < id.size(); ++i) {
	cout << "account: " << id[i] << " amount: " << pos[i] << endl;
      }
      
      string message = db.add_new_symbol(sym, id, pos, n);
      create_response("sym", sym ,message);
    }
  }
}

void Parser::transcation_handler( pugi::xml_node transcation) {

  int id = std::stoi(transcation.attribute("id").value());
  
  for (pugi::xml_node request: transcation.children())  {
    if (!strcmp(request.name(),"order")) {
      string sym = request.attribute("sym").value();
      int amount = std::stoi(request.attribute("amount").value());
      int limit = std::stoi(request.attribute("limit").value());
      
      if (amount > 0) {
	db.add_border(sym, id, amount, limit);
      } else if (amount < 0) {
	db.add_sorder(sym, id, amount, limit);
      }
      
      cout << "place order: " << endl << "account: " << id << " sym: " << sym << " amount: " << amount << " limit: " << limit <<endl;
														   
    } else if(!strcmp(request.name(),"cancel")) {
      int id = std::stoi(request.attribute("id").value());
      cout << "cancel order: id " << id << endl;
      db.cancel(id);
    } else if(!strcmp(request.name(),"query")) {
      int id = std::stoi(request.attribute("id").value());
      cout << "query order: id " << id <<endl;
      db.query(id);
    }
  }
}
  
  
  
