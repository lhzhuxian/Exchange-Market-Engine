PQXX=-lpqxx -lpq
BOOST= -lboost_system -I/usr/local/include/boost -L/usr/local/lib
GDB = -g
engine: main.cpp server.o parser.o database.o pugixml.o Common.h server.h database.h
	g++ -std=c++11 main.cpp server.o parser.o database.o pugixml.o $(BOOST) $(PQXX) $(GDB) -pthread -o engine
server.o: server.cpp server.h
	g++ -std=c++11 server.cpp -c $(GDB)
parser.o: parser.cpp parser.h
	g++ -std=c++11 parser.cpp -c $(GDB)
database.o: server.cpp database.h
	g++ -std=c++11 database.cpp -c $(GDB)
pugixml.o: pugixml.cpp pugixml.hpp pugiconfig.hpp
	g++ -std=c++11 pugixml.cpp -c
clean:
	rm -f *~ database.o parser.o server.o  engine
