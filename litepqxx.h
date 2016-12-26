#include <memory>
#include <list>
#include <vector>
#include <sqlite3.h>
namespace lite
{
namespace pqxx
{

class work;

class connection {
friend work;
public:
	connection(const std::string & options);
	~connection() noexcept; // to be virtual?
private:
	sqlite3 * db;
};

class result {
friend work;
public:
	typedef unsigned long size_type;
	class field {
	public:
		virtual ~field() noexcept;
		template<typename T> T as () const;
		virtual int as_integer() const;
		virtual double as_float() const;
		virtual std::string as_text() const;
		//virtual unsigned char * as_blob() const;
		virtual bool is_null() const;
	};
	class integer_field : public field {
	private:
		int value;
	public:
		integer_field(int value);
		int as_integer() const;
	};
	class float_field : public field {
	private:
		double value;
	public:
		float_field(double value);
		double as_float() const;
	};
	class text_field : public field {
	private:
		std::string value;
	public:
		text_field(const unsigned char * value);
		std::string as_text() const;
	};
	class null_field : public field {
	public:
		bool is_null() const;
	};
	class const_iterator {
	friend result;
	public:
		//~const_iterator() noexcept;		
		const_iterator operator ++ ();
		const_iterator operator ++ (int);
		bool operator == (const const_iterator & i) const;
		bool operator != (const const_iterator & i) const;
		field & operator[] (int i) const;
	private:
		std::list<std::vector<std::unique_ptr<result::field>>>::iterator iterator;
		const_iterator(std::list<std::vector<std::unique_ptr<result::field>>>::iterator i);
	};
public:
	result();
	//~result() noexcept;
	size_type affected_rows() const;
	const_iterator begin() const;
	const_iterator end() const;
private:
	int changes;
	std::shared_ptr<std::list<std::vector<std::unique_ptr<result::field>>>> rows;
};

class work {
friend connection;
private:
	const connection * conn;
	bool done;
public:
	work(const connection & c);
	~work() noexcept ; // to be virtual?
	void commit();
	void abort();
	result exec(const std::string & q);
};

}
}

