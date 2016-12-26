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
}

connection::~connection() {
	int status = sqlite3_close(db);
	if (status != SQLITE_OK) {
		// error
	}
}

}
}

