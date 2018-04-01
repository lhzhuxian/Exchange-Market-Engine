#include "Common.h"
#include "parser.h"

class Server {

private:

    // 服务实例
    boost::asio::io_service& ios_;

    // 接收器实例
    boost::asio::ip::tcp::acceptor acceptor_;

    // socket智能指针
    typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

public:

    Server(boost::asio::io_service& _ios);
    
    // 启动网络侦听的操作入口
    void start(void);


    void transaction(Parser & request);

    void create(Parser & request);
    
};
