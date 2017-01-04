#include "litepqxx.h"

namespace lite
{
namespace pqxx
{

void check_sqlite3(int code) {
	switch (code) {
	case SQLITE_OK:
		return;
	case SQLITE_FULL:
		throw disk_full();
	case SQLITE_NOMEM:
		throw out_of_memory();
	case SQLITE_CONSTRAINT:
		throw integrity_constraint_violation();
	case SQLITE_ERROR:
		throw syntax_error();
	case SQLITE_INTERNAL:
	case SQLITE_PERM:
	case SQLITE_ABORT:
	case SQLITE_BUSY:
	case SQLITE_LOCKED:
	case SQLITE_READONLY:
	case SQLITE_INTERRUPT:
	case SQLITE_IOERR:
	case SQLITE_CORRUPT:
	case SQLITE_NOTFOUND:
	case SQLITE_CANTOPEN:
	case SQLITE_PROTOCOL:
	case SQLITE_EMPTY:
	case SQLITE_SCHEMA:
	case SQLITE_TOOBIG:
	case SQLITE_MISMATCH:
	case SQLITE_MISUSE:
	case SQLITE_NOLFS:
	case SQLITE_AUTH:
	case SQLITE_FORMAT:
	case SQLITE_RANGE:
	case SQLITE_NOTADB:
	case SQLITE_NOTICE:
	case SQLITE_WARNING:
	case SQLITE_ROW:
	case SQLITE_DONE:
	default:
		throw internal_error();
	}
}

}
}

