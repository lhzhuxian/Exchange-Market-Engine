#include "Common.h"
#include "server.h"
#include "database.h"


unique_ptr<connection> database::c(new connection("dbname=xxx user=postgres password = passw0rd"); 
  
  
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
