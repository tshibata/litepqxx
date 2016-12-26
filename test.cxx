#include <CppUTest/CommandLineTestRunner.h>
#include "litepqxx.h"

using namespace lite;

TEST_GROUP(GeneralTests)
{
};

TEST(GeneralTests, Connect)
{
	pqxx::connection C(":memory:");
	{
		pqxx::work W(C);
		pqxx::result R(W.exec("DROP TABLE IF EXISTS users;"));
		W.commit();
	}
}

TEST(GeneralTests, Integer)
{
	pqxx::connection C(":memory:");
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("CREATE TABLE numbers (n INTEGER);"));
		pqxx::result R2(W.exec("INSERT INTO numbers (n) values (2), (3), (5);"));
		pqxx::result R3(W.exec("SELECT n FROM numbers;"));

		pqxx::result::const_iterator i = R3.begin();

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		CHECK_EQUAL(2, i[0].as<int>());
		++i;

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		CHECK_EQUAL(3, i[0].as<int>());
		++i;

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		CHECK_EQUAL(5, i[0].as<int>());
		++i;

		CHECK(i == R3.end());

		W.commit();
	}
}

TEST(GeneralTests, Real)
{
	pqxx::connection C(":memory:");
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("CREATE TABLE numbers (f REAL);"));
		pqxx::result R2(W.exec("INSERT INTO numbers (f) values (1.414), (1.732), (2);"));
		pqxx::result R3(W.exec("SELECT f FROM numbers;"));

		pqxx::result::const_iterator i = R3.begin();

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		CHECK_EQUAL(1.414, i[0].as<double>());
		++i;

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		CHECK_EQUAL(1.732, i[0].as<double>());
		++i;

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		CHECK_EQUAL(2, i[0].as<double>());
		++i;

		CHECK(i == R3.end());

		W.commit();
	}
}

TEST(GeneralTests, Text)
{
	pqxx::connection C(":memory:");
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("CREATE TABLE users (name VARCHAR);"));
		pqxx::result R2(W.exec("INSERT INTO users (name) values ('Alice'), ('Bob'), ('Carol');"));
		pqxx::result R3(W.exec("SELECT name FROM users;"));

		pqxx::result::const_iterator i = R3.begin();

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		STRCMP_EQUAL("Alice", i[0].as<std::string>().c_str());
		++i;

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		STRCMP_EQUAL("Bob", i[0].as<std::string>().c_str());
		++i;

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		STRCMP_EQUAL("Carol", i[0].as<std::string>().c_str());
		++i;

		CHECK(i == R3.end());

		W.commit();
	}
}

TEST(GeneralTests, Select)
{
	pqxx::connection C(":memory:");
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("CREATE TABLE users (id INTEGER, name VARCHAR, score FLOAT);"));
		pqxx::result R2(W.exec("INSERT INTO users (id, name, score) values (1, 'Alice', 9.1), (2, 'Bob', 9.2);"));
		pqxx::result R3(W.exec("SELECT id, name, score FROM users;"));

		pqxx::result::const_iterator i = R3.begin();

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		CHECK_EQUAL(1, i[0].as<int>());
		CHECK(! i[1].is_null());
		STRCMP_EQUAL("Alice", i[1].as<std::string>().c_str());
		CHECK(! i[2].is_null());
		CHECK_EQUAL(9.1, i[2].as<double>());
		++i;

		CHECK(i != R3.end());
		CHECK(! i[0].is_null());
		CHECK_EQUAL(2, i[0].as<int>());
		CHECK(! i[1].is_null());
		STRCMP_EQUAL("Bob", i[1].as<std::string>().c_str());
		CHECK(! i[2].is_null());
		CHECK_EQUAL(9.2, i[2].as<double>());
		++i;

		CHECK(i == R3.end());

		W.commit();
	}
}

TEST(GeneralTests, Delete)
{
	pqxx::connection C(":memory:");
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("CREATE TABLE users (id INTEGER, name VARCHAR, score FLOAT);"));
		pqxx::result R2(W.exec("INSERT INTO users (id, name, score) values (1, 'Alice', 9.1), (2, 'Bob', 9.2), (3, 'Carol', 9.3);"));
		pqxx::result R3(W.exec("DELETE FROM users WHERE id = 2;"));
		CHECK_EQUAL(1, R3.affected_rows());
		pqxx::result R4(W.exec("SELECT id, name, score FROM users;"));

		pqxx::result::const_iterator i = R4.begin();

		CHECK(i != R4.end());
		CHECK(! i[0].is_null());
		CHECK_EQUAL(1, i[0].as<int>());
		CHECK(! i[1].is_null());
		STRCMP_EQUAL("Alice", i[1].as<std::string>().c_str());
		CHECK(! i[2].is_null());
		CHECK_EQUAL(9.1, i[2].as<double>());
		++i;

		CHECK(i != R4.end());
		CHECK(! i[0].is_null());
		CHECK_EQUAL(3, i[0].as<int>());
		CHECK(! i[1].is_null());
		STRCMP_EQUAL("Carol", i[1].as<std::string>().c_str());
		CHECK(! i[2].is_null());
		CHECK_EQUAL(9.3, i[2].as<double>());
		++i;

		CHECK(i == R4.end());

		W.commit();
	}
}

TEST(GeneralTests, NullCheck)
{
	pqxx::connection C(":memory:");
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("CREATE TABLE users (id INTEGER, name VARCHAR);"));
		pqxx::result R2(W.exec("INSERT INTO users (id, name) values (NULL, NULL);"));
		pqxx::result R3(W.exec("SELECT id, name FROM users;"));

		pqxx::result::const_iterator i = R3.begin();

		CHECK(i != R3.end());
		CHECK(i[0].is_null());
		CHECK(i[1].is_null());
		++i;

		CHECK(i == R3.end());

		W.commit();
	}
}

TEST(GeneralTests, ExplicitAbort)
{
	pqxx::connection C(":memory:");
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("CREATE TABLE users (id INTEGER, name VARCHAR);"));
		W.commit();
	}
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("INSERT INTO users (name) values ('Alice'), ('Bob'), ('Carol');"));
		W.abort(); // abort!
	}
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("SELECT id, name FROM users;"));
		CHECK(R1.begin() == R1.end());
	}
}

TEST(GeneralTests, ImplicitAbort)
{
	pqxx::connection C(":memory:");
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("CREATE TABLE users (id INTEGER, name VARCHAR);"));
		W.commit();
	}
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("INSERT INTO users (name) values ('Alice'), ('Bob'), ('Carol');"));
		// no abort!
	}
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("SELECT id, name FROM users;"));
		CHECK(R1.begin() == R1.end());
	}
}

TEST(GeneralTests, Iterator)
{
	pqxx::connection C(":memory:");
	{
		pqxx::work W(C);
		pqxx::result R1(W.exec("CREATE TABLE users (name VARCHAR);"));
		pqxx::result R2(W.exec("INSERT INTO users (name) values ('Alice'), ('Bob');"));
		pqxx::result R3(W.exec("SELECT name FROM users;"));

		pqxx::result::const_iterator i = R3.begin();

		CHECK(i != R3.end());

		CHECK(! i[0].is_null());
		STRCMP_EQUAL("Alice", i[0].as<std::string>().c_str());
		CHECK(i != (++ i)); // prefixed ++

		CHECK(i != R3.end());

		CHECK(! i[0].is_null());
		STRCMP_EQUAL("Bob", i[0].as<std::string>().c_str());
		CHECK(i == (i++)); // suffixed ++

		CHECK(i == R3.end());

		W.commit();
	}
}

int main(int argc, char** argv)
{
	return CommandLineTestRunner::RunAllTests(argc, argv);
}

