#include <iostream>
#include "litepqxx.h"

namespace lite
{
namespace pqxx
{

prepare::invocation::invocation(work * w, sqlite3_stmt * stmt) : w(w), stmt(stmt), count(0) {
}

prepare::invocation::~invocation() {
	sqlite3_finalize(stmt);
}

template<> prepare::invocation & prepare::invocation::operator() (const int & value) {
	if (sqlite3_bind_int(stmt, ++ count, value) != SQLITE_OK) {
		// error
	}
	return * this;
}

template<> prepare::invocation & prepare::invocation::operator() (const std::string & value) {
	if (sqlite3_bind_text(stmt, ++ count, value.c_str(), value.length(), SQLITE_TRANSIENT) != SQLITE_OK) {
		// error
	}
	return * this;
}

template<> prepare::invocation & prepare::invocation::operator() (const char * value) {
	if (sqlite3_bind_text(stmt, ++ count, value, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		// error
	}
	return * this;
}

result prepare::invocation::exec() {
	result r;
	w->exec(stmt, r);
	return r;
}

}
}

