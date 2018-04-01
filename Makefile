engine: main.cpp server.o pugixml.o Common.h server.h
	g++ -std=c++11 main.cpp server.o pugixml.o -lboost_system -I/usr/local/include/boost -L/usr/local/lib -pthread -o engine
server.o: server.cpp Common.h server.h
	g++ -std=c++11 server.cpp -c
pugixml.o: pugixml.cpp pugixml.hpp pugiconfig.hpp
	g++ -std=c++11 pugixml.cpp -c
