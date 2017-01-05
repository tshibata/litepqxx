#include <iostream>
#include "litepqxx.h"

namespace lite
{
namespace pqxx
{

work::work(const connection & c) {
	// block other threads?
	done = false;
	conn = & c;
	check_sqlite3(sqlite3_exec(conn->db, "BEGIN", NULL, NULL, NULL));
}

work::~work() {
	if (! done) {
		sqlite3_exec(conn->db, "ROLLBACK", NULL, NULL, NULL);
	}
}

void work::commit() {
	check_sqlite3(sqlite3_exec(conn->db, "COMMIT", NULL, NULL, NULL));
	done = true;
}

void work::abort() {
	check_sqlite3(sqlite3_exec(conn->db, "ROLLBACK", NULL, NULL, NULL));
	done = true;
}

void work::exec(sqlite3_stmt * stmt, result & r) {
	int status;

	while (true) {
		status = sqlite3_step(stmt);
		if (status == SQLITE_ROW) {
			int count = sqlite3_column_count(stmt);
			r.rows->push_back(std::vector<std::unique_ptr<result::field>>(count));
			std::vector<std::unique_ptr<result::field>> & row = r.rows->back();
			for (int i = 0; i < count; ++i) {
				switch (sqlite3_column_type(stmt, i)) {
				case SQLITE_INTEGER:
					row[i] = std::unique_ptr<result::field>(new result::integer_field(sqlite3_column_int(stmt, i)));
					// or sqlite3_column_int64?
					break;
				case SQLITE_FLOAT:
					row[i] = std::unique_ptr<result::field>(new result::float_field(sqlite3_column_double(stmt, i)));
					break;
				case SQLITE_TEXT:
					row[i] = std::unique_ptr<result::field>(new result::text_field(sqlite3_column_text(stmt, i)));
					// or sqlite3_column_text16?
					break;
				case SQLITE_BLOB:
					row[i] = std::unique_ptr<result::field>(new result::blob_field(sqlite3_column_blob(stmt, i), sqlite3_column_bytes(stmt, i)));
					break;
				case SQLITE_NULL:
					row[i] = std::unique_ptr<result::field>(new result::null_field());
					break;
				default:
					throw internal_error();
				}
			}
		} else if (status == SQLITE_DONE) {
			break;
		} else {
			throw internal_error();
		}
	}
	r.changes = sqlite3_changes(this->conn->db);
}

result work::exec(const std::string & q) {
	sqlite3_stmt *stmt = NULL;
	result r;

	check_sqlite3(sqlite3_prepare_v2(this->conn->db, q.c_str(), q.length(), &stmt, NULL));
	exec(stmt, r);
	check_sqlite3(sqlite3_finalize(stmt));

	return r;
}

prepare::invocation work::prepared(const std::string & name) {
	if (conn->prepared->count(name) == 0) {
		throw argument_error();
	}
	std::string statement = (* conn->prepared)[name];
	sqlite3_stmt * stmt;
	check_sqlite3(sqlite3_prepare_v2(conn->db, statement.c_str(), statement.length(), &stmt, NULL)); // prepare sqlite3_stmt for each execution.
	return prepare::invocation(this, stmt);
}

}
}

