#include <iostream>
#include "litepqxx.h"

namespace lite
{
namespace pqxx
{

connection::connection(const std::string & options) {
	check_sqlite3(sqlite3_open(options.c_str(), &db));
	prepared = std::shared_ptr<std::map<std::string, std::string>>(new std::map<std::string, std::string>());
}

connection::~connection() {
	sqlite3_close(db);
}

void connection::prepare(const std::string & name, const std::string & statement) {
	(* prepared)[name] = statement;
}

}
}

