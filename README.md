# litepqxx
Litepqxx is (planned to be) a wrapper library to access SQLite3 via API compatible with Libpqxx

## Usecases
* For quick prototyping with SQLite3 and smooth migration to Postgresql.
* To use SQLite3 as a stub db when you test your project with Postgresql.
* Just as a better, object-oriented API for SQLite3.

## Known gaps
* It does not (yet?) cover the whole API.
* Sqlite3 has "BLOB" instead of "BYTEA".
* affected_rows() works properly only in case of INSERT, UPDATE or DELETE. (In other cases, it returns value left by the last execution of such statements)
* It throws syntax_error instead of the subclasses, i.e undefined_column, undefined_function and undefined_table.
* It throws integrity_constraint_violation instead of the subclasses, i.e. check_violation, foreign_key_violation, not_null_violation, restrict_violation and unique_violation.

