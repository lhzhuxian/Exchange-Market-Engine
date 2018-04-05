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
    size_t len = 0;
    //add
    while(true) {

      len += socket->read_some(boost::asio::buffer(buf), error);
      if (error == boost::asio::error::eof) {
	break;
      } else if (error) {
	throw boost::system::system_error(error);
      }
      
    }


    Parser request(buf, len);
    string response = request.start();
    //string res = request.get_response();
    socket->write_some(boost::asio::buffer(response), error);
    
  }
}

void Server::transaction(Parser & request) {
}

void Server::create(Parser & request) {
}





