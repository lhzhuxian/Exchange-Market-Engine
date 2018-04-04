PQXX=-lpqxx -lpq
BOOST= -lboost_system -I/usr/local/include/boost -L/usr/local/lib
engine: main.cpp server.o parser.o database.o pugixml.o Common.h server.h database.h
	g++ -std=c++11 main.cpp server.o parser.o database.o pugixml.o $(BOOST) $(PQXX) -pthread -o engine
server.o: server.cpp server.h
	g++ -std=c++11 server.cpp -c
parser.o: parser.cpp parser.h
	g++ -std=c++11 parser.cpp -c
database.o: server.cpp database.h
	g++ -std=c++11 database.cpp -c
pugixml.o: pugixml.cpp pugixml.hpp pugiconfig.hpp
	g++ -std=c++11 pugixml.cpp -c
