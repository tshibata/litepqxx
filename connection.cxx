#include <iostream>
#include "litepqxx.h"

namespace lite
{
namespace pqxx
{

connection::connection(const std::string & options) {
	int status = sqlite3_open(options.c_str(), &db);
	if (status != SQLITE_OK) {
		// error
	}
	prepared = std::shared_ptr<std::map<std::string, std::string>>(new std::map<std::string, std::string>());
}

connection::~connection() {
	int status = sqlite3_close(db);
	if (status != SQLITE_OK) {
		// error
	}
}

void connection::prepare(const std::string & name, const std::string & statement) {
	(* prepared)[name] = statement;
}

}
}

