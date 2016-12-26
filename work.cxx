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
	int status = sqlite3_exec(conn->db, "BEGIN;", NULL, NULL, NULL);
}

work::~work() {
	if (! done) {
		abort();
	}
}

void work::commit() {
	int status = sqlite3_exec(conn->db, "COMMIT;", NULL, NULL, NULL);
	if (status != SQLITE_OK) {
		// error
	}
	done = true;
}

void work::abort() {
	int status = sqlite3_exec(conn->db, "ROLLBACK;", NULL, NULL, NULL);
	if (status != SQLITE_OK) {
		// error
	}
	done = true;
}

result work::exec(const std::string & q) {
	sqlite3_stmt *stmt = NULL;
	int status;

	status = sqlite3_prepare_v2(this->conn->db, q.c_str(), q.length(), &stmt, NULL);
	if (status != SQLITE_OK) {
		// error
	}

	result r;
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
					break;
					// not implemented yet
				case SQLITE_NULL:
					row[i] = std::unique_ptr<result::field>(new result::null_field());
					break;
				}
			}
		} else if (status == SQLITE_DONE) {
			break;
		} else {
			// error
		}
	}
	r.changes = sqlite3_changes(this->conn->db);

	status = sqlite3_finalize(stmt);
	if (status != SQLITE_OK) {
		// error
	}

	return r;
}

}
}

