#include "server.h"
#define SIZE 1024
Server::Server(boost::asio::io_service& _ios) : ios_(_ios),
						acceptor_(_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 12345)) {
        // 默认执行
        start();
    }

void Server::start(void) {


  boost::system::error_code error;  
  while (true) {


    socket_ptr socket(new boost::asio::ip::tcp::socket(ios_));
    //auto buf = std::unique_ptr<char []> (new char[size]);
   
    char buf[SIZE];
    acceptor_.accept(*socket);
    size_t len = socket->read_some(boost::asio::buffer(buf), error);
   


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
    cout << endl;
    
    /*
    if (error == boost::asio::error::eof) {
      break;
    } else if (error) {
      throw boost::system::system_error(error);
     }
    */
    socket->write_some(boost::asio::buffer(buf, len), error);

  }
}

void Server::transaction(Parser & request) {
}

void Server::create(Parser & request) {
}





