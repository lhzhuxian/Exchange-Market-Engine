#ifndef _COMMON_H__
#define _COMMON_H__

#include <utility>
#include <unordered_map>
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "pugiconfig.hpp"
#include "pugixml.hpp"
#include  <memory>
#include <string>
#include <vector>
#include <pqxx/pqxx>
using namespace pqxx;
using std::pair;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::unique_ptr;
using std::string;
using std::to_string;
using std::vector;
using std::stoi;
using std::unordered_map;

extern int order_id;
#endif
