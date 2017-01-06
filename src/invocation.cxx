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
	check_sqlite3(sqlite3_bind_int(stmt, ++ count, value));
	return * this;
}

template<> prepare::invocation & prepare::invocation::operator() (const std::string & value) {
	check_sqlite3(sqlite3_bind_text(stmt, ++ count, value.c_str(), value.length(), SQLITE_TRANSIENT));
	return * this;
}

template<> prepare::invocation & prepare::invocation::operator() (const binarystring & value) {
	check_sqlite3(sqlite3_bind_blob(stmt, ++ count, value.address, value.length, SQLITE_TRANSIENT));
	return * this;
}

template<> prepare::invocation & prepare::invocation::operator() (const char * value) {
	check_sqlite3(sqlite3_bind_text(stmt, ++ count, value, -1, SQLITE_TRANSIENT));
	return * this;
}

result prepare::invocation::exec() {
	result r;
	w->exec(stmt, r);
	return r;
}

}
}

