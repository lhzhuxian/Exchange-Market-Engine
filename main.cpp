#include "Common.h"
#include "server.h"
  

  
  
  
int main(void) {  
    try {  
        cout << "server start." << endl;  
        // 建造服务对象  
        boost::asio::io_service ios;  
        // 构建Server实例  
        Server server(ios);  
    }  
    catch (std::exception& _e) {  
      cout << _e.what() << endl;  
    }  
    cout << "server end." << endl;  
    return 0;  
}  
