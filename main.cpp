#include "Common.h"
#include "server.h"
#include "database.h"
#include <pqxx/pqxx>

int order_id = 0;

int get_id () {
  unique_ptr<connection> c (new connection("dbname=enginedb user=postgres password = passw0rd"));
  string sql = "SELECT MAX(id) FROM TORDER;";
  nontransaction N(*c);
  result R(N.exec(sql));
  if (!R[0][0].is_null()){
    return R[0][0].as<int>() + 1;
  }
  else {
    return 0;
  }
}
int main(void) {
  order_id = get_id();
  cout << "order start from " << order_id << endl; 
  //  try {  
    cout << "server start." << endl;  
    // 建造服务对象  
    boost::asio::io_service ios;  
        // 构建Server实例
    
    Server server(ios);  
    /*    }  
  catch (std::exception& _e) {  
    cout << _e.what() << endl;
    cout << "miaomiaomiao" << endl;
    }  */
  cout << "server end." << endl;  
  return 0;  
}  
