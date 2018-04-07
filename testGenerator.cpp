#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <thread>
#include <vector>
#include <netdb.h>
#include <unistd.h>
#include <time.h>


// This is the test case generator for load testing our exchange machine
void send_to_server(std::string xml);

// Generate random account with random balance
void generate_acount(std::vector<int> & accountID, std::vector<std::string> & accXML) {
    int gen_accountID = rand() % 1000 + 1;
    int gen_balance = rand() % 100000;
    accountID.push_back(gen_accountID);
    std::string xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    xml += "<create>\n";
    xml += "  <account id=\"";
    xml += std::to_string(gen_accountID);
    xml.append("\"");
    xml += " balance=\"" + std::to_string(gen_balance);
    xml.append("\"");
    xml += "/>\n";
    xml += "</create>\n";
    int sz = xml.size();
    xml= std::to_string(sz) + "\n" + xml;
    accXML.push_back(xml);
}

// Generate random symbol with random limit from random existing account
void generate_symbol(std::vector<std::string> & symbol, std::vector<int> & accountID, std::vector<std::string> & symXML) {
    std::string all("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    int symbol_len = 5;
    std::string gen_symbol = "";
    // Generate random symbol
    for (int j = 0; j < symbol_len; j++) {
        gen_symbol += all[rand() % (sizeof(all) - 1)];
    }
    // std::cout << gen_symbol << std::endl;
    gen_symbol = "X";
    symbol.push_back(gen_symbol);

    int gen_amount = rand() % 1000 + 1;

    int account_index = rand() % accountID.size();
    int gen_account = accountID[account_index];

    std::string xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    xml += "<create>\n";
    xml += "  <symbol sym=\"";
    xml += gen_symbol;
    xml.append("\"");
    xml += ">\n";
    xml += "    <account id=\"";
    xml += std::to_string(gen_account);
    xml.append("\"");
    xml += ">";
    xml += std::to_string(gen_amount);
    xml += "</account>\n";
    xml += "  </symbol>\n";
    xml += "</create>\n";
    int sz = xml.size();
    xml= std::to_string(sz) + "\n" + xml;
    symXML.push_back(xml);
}

// Generate Transaction Order
void generate_order(std::vector<std::string> & symbol, std::vector<int> & accountID, std::vector<std::string> & ordXML) {
    int account_index = rand() % accountID.size();
    int gen_account = accountID[account_index];
    int symbol_index = rand() % symbol.size();
    std::string gen_symbol = symbol[symbol_index];
    int amount_limit = 10;
    int gen_amount = rand() % amount_limit + 1;
    if (rand() % 2) {
        gen_amount *= -1;
    }
    int max_limit = 200;
    int gen_limit_int = rand() % max_limit + 1;
    double gen_limit_decimal = (rand() % 100) / 100.0;
    double gen_limit = gen_limit_int + gen_limit_decimal;
    std::string xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    xml += "<transactions account=\"";
    xml += std::to_string(gen_account);
    xml.append("\"");
    xml += ">\n";
    xml += "  <order sym=\"";
    xml += gen_symbol;
    xml.append("\"");
    xml += " amount=\"";
    xml += std::to_string(gen_amount);
    xml.append("\"");
    xml += " limit=\"";
    xml += std::to_string(gen_limit);
    xml.append("\"");
    xml += "/>\n";
    xml += "</transactions>\n";
    int sz = xml.size();
    xml= std::to_string(sz) + "\n" + xml;
    ordXML.push_back(xml);
}

// Generate Transaction Query
void generate_query(std::vector<int> & accountID, std::vector<std::string> & qryXML) {
    int account_index = rand() % accountID.size();
    int gen_account = accountID[account_index];
    int gen_query = rand() % 100 + 1;
    std::string xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    xml += "<transactions account=\"";
    xml += std::to_string(gen_account);
    xml.append("\"");
    xml += ">\n";
    xml += "  <query id=\"";
    xml += std::to_string(gen_query);
    xml.append("\"");
    xml += "/>\n";
    xml += "</transactions>\n";
    int sz = xml.size();
    xml= std::to_string(sz) + "\n" + xml;
    qryXML.push_back(xml);
}

// Generate Transaction Cancel
void generate_cancel(std::vector<int> & accountID, std::vector<std::string> & cnlXML) {
    int account_index = rand() % accountID.size();
    int gen_account = accountID[account_index];
    int gen_cancel = rand() % 100 + 1;
    std::string xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    xml += "<transactions account=\"";
    xml += std::to_string(gen_account);
    xml.append("\"");
    xml += ">\n";
    xml += "  <cancel id=\"";
    xml += std::to_string(gen_cancel);
    xml.append("\"");
    xml += "/>\n";
    xml += "</transactions>\n";
    int sz = xml.size();
    xml= std::to_string(sz) + "\n" + xml;
    cnlXML.push_back(xml);
}

int main(void) {
    srand(time(NULL));
    std::vector<std::string> symbol;
    std::vector<int> accountID;
    std::vector<std::string> accXML;
    std::vector<std::string> symXML;
    std::vector<std::string> ordXML;
    std::vector<std::string> qryXML;
    std::vector<std::string> cnlXML;

    int account_limit = 1000;
    int symbol_limit = 1000;
    int order_limit = 1000;
    int query_limit = 500;
    int cancel_limit = 500;

    // Generate all the account
    for (int i = 0; i < account_limit; i++) {
        generate_acount(accountID, accXML);
    }
    for (int i = 0; i < symbol_limit; i++) {
        generate_symbol(symbol, accountID, symXML);
    }
    for (int i = 0; i < order_limit; i++) {
        generate_order(symbol, accountID, ordXML);
    }
    for (int i = 0; i < query_limit; i++) {
        generate_query(accountID, qryXML);
    }
    for (int i = 0; i < cancel_limit; i++) {
        generate_cancel(accountID, cnlXML);
    }

    for (int i = 0; i < account_limit; i++) {
        std::cout << accXML[i] << std::endl;
    }
    for (int i = 0; i < symbol_limit; i++) {
        std::cout << symXML[i] << std::endl;
    }
    for (int i = 0; i < order_limit; i++) {
        std::cout << ordXML[i] << std::endl;
    }
    for (int i = 0; i < query_limit; i++) {
        std::cout << qryXML[i] << std::endl;
    }
    for (int i = 0; i < cancel_limit; i++) {
        std::cout << cnlXML[i] << std::endl;
    }

    int nThreads=account_limit;

    // Account
    std::vector<std::thread> T;
    clock_t start, finish;
    start = clock();

    for(int i=0;i<nThreads;++i){
        T.push_back(std::thread(send_to_server, accXML[i]));
    }
    for(int j=0;j<nThreads;++j){
      T[j].join();
    }

    // Symbol
    nThreads=symbol_limit;
    std::vector<std::thread> S;
    for(int i=0;i<nThreads;++i){
      S.push_back(std::thread(send_to_server, symXML[i]));
    }
    for(int j=0;j<nThreads;++j){
      S[j].join();
    }

    // Order
    nThreads = order_limit;
    std::vector<std::thread> O;
    for(int i=0;i<nThreads;++i){
      O.push_back(std::thread(send_to_server, ordXML[i]));
    }
    for(int j=0;j<nThreads;++j){
      O[j].join();
    }

    // Query
    nThreads = query_limit;
    std::vector<std::thread> Q;
    for(int i=0;i<nThreads;++i){
      Q.push_back(std::thread(send_to_server, qryXML[i]));
    }
    for(int j=0;j<nThreads;++j){
      Q[j].join();
    }

    // Cancel
    nThreads = cancel_limit;
    std::vector<std::thread> C;
    for(int i=0;i<nThreads;++i){
      C.push_back(std::thread(send_to_server, cnlXML[i]));
    }
    for(int j=0;j<nThreads;++j){
      C[j].join();
    }

    finish = clock();
    std::cout<<"time used :"<<(double)(finish - start) / CLOCKS_PER_SEC<< std::endl;
    return 0;    
}

void send_to_server(std::string xml) {
    // Create socket and send it to the server
    // The following code was adapted from ECE 650 lecture slides
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = "vcm-3223.vm.duke.edu";
    const char *port     = "12345";

    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family   = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0) {
        std::cerr << "Error: cannot get address info for host" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        exit(EXIT_FAILURE);
    } //if

    socket_fd = socket(host_info_list->ai_family, 
		     host_info_list->ai_socktype, 
		     host_info_list->ai_protocol);
    if (socket_fd == -1) {
        std::cerr << "Error: cannot create socket" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        exit(EXIT_FAILURE);
    } //if

    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
        std::cerr << "Error: cannot connect to socket" << std::endl;
        std::cerr << "  (" << hostname << "," << port << ")" << std::endl;
        exit(EXIT_FAILURE);
    } //if

    const char * message = xml.c_str();
    send(socket_fd, message, strlen(message), 0);

    char buffer[2];
    recv(socket_fd, buffer, 1, 0);
    buffer[1] = 0;

    freeaddrinfo(host_info_list);
    close(socket_fd);
}
