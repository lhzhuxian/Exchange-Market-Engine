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

void Parser::start () {
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

void Parser::create_response(string attr, string value, string message) {
  pugi::xml_node nodeRoot = res.child("results");
  if (message.compare("success")) {
    pugi::xml_node nodeError = nodeRoot.append_child("error");
    nodeError.append_attribute(attr.data()) = value.data();
    nodeError.append_child(pugi::node_pcdata).set_value(message.data());
  } else {
    pugi::xml_node nodeCreated = nodeRoot.append_child("created");
    nodeCreated.append_attribute(attr.data()) = value.data();
  }  
}
bool Parser::AllisNum(string str) {
  for(auto i: str) {
    if ( i < '0' || i >'9')
      return false;
  }
  return true;
}

bool Parser::AllisAlphaNum(string str) {
  for (auto i:str) {
    if (!isalnum) {
      return false;
    }
  }
  return true;
}
void Parser::create_handler(pugi::xml_node create) {

  for (pugi::xml_node request: create.children())  {
    
    if (!strcmp(request.name(),"account")) {
      string id = request.attribute("id").value();
      if (AllisNum(id)) {
	int balance = std::stoi(request.attribute("balance").value());
	if (balance >= 0) {
  
	  cout << "request: create account: " << id << " blance: " << balance << endl;
	  
	  string message = db.add_new_account(std::stoi(id) , balance);
	  create_response("id", id, message);
	} else {
	  create_response("id", id, "Invalid Balance");
	}
      } else {
	create_response("id", id, "Invalid Account Id");
      }
      
    } else if (!strcmp(request.name(),"symbol")){
      string sym = request.attribute("sym").value();
      int flag = 1;
      if (AllisAlphaNum(sym)) {
	vector<int> id;
	vector<int> pos;
	int n = 0;
	for (pugi::xml_node account: request.children()) {
	  string tmp_id = account.attribute("id").value();
	  string tmp_pos = account.child_value();
	  if (AllisNum(tmp_id) && AllisNum(tmp_pos)) {
	    id.push_back(std::stoi(account.attribute("id").value()));
	    pos.push_back(std::stoi(account.child_value()));
	    ++n;
	  } else {
	    create_response("sym", sym ,"Invalid Account id or Position");
	    flag = 0;
	    break;
	  }
	}
	if (flag) {
	  cout << "request: create symbol: " << sym << endl;
	  for (int i = 0; i < id.size(); ++i) {
	    cout << "account: " << id[i] << " amount: " << pos[i] << endl;
	  }
	  string message = db.add_new_symbol(sym, id, pos, n);
	  
	  create_response("sym", sym ,message);
	}
      } else {
	create_response("sym", sym ,"Invalid symbol name");
      }
    }
  }
}
void Parser::transcation_response(string sym, int amount, int limit, int order_id) {
  pugi::xml_node nodeRoot = res.child("results");
  pugi::xml_node response;
  if (order_id == -1) {
    response = nodeRoot.append_child("error");
    response.append_child(pugi::node_pcdata).set_value("invalid order");
  } else {
    response = nodeRoot.append_child("opened");
    response.append_attribute("id") = to_string(order_id).data();
  }
  response.append_attribute("sym") = sym.data();
  response.append_attribute("amount") = to_string(amount).data();
  response.append_attribute("limit") = to_string(limit).data();
}


void Parser::transcation_handler( pugi::xml_node transcation) {
  pugi::xml_node nodeRoot = res.child("results");
  
  int id = std::stoi(transcation.attribute("id").value());
  
  for (pugi::xml_node request: transcation.children())  {
    if (!strcmp(request.name(),"order")) {
      string sym = request.attribute("sym").value();
      int amount = std::stoi(request.attribute("amount").value());
      int limit = std::stoi(request.attribute("limit").value());
      int order_id;
      if (amount > 0) {
	order_id = db.add_border(sym, id, amount, limit);
      } else if (amount < 0) {
	order_id = db.add_sorder(sym, id, amount, limit);
      }
      transcation_response(sym, amount, limit, order_id);
    } else if(!strcmp(request.name(),"cancel")) {
      int id = std::stoi(request.attribute("id").value());
      cout << "cancel order: id " << id << endl;
      db.cancel(id, nodeRoot);
      
      
      
    } else if(!strcmp(request.name(),"query")) {
      int id = std::stoi(request.attribute("id").value());
      cout << "query order: id " << id <<endl;
      db.query(id, nodeRoot);

    }
  }
}
  
  
  
