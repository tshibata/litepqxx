#include "litepqxx.h"

namespace lite
{
namespace pqxx
{

result::result() {
	rows = std::shared_ptr<std::list<std::vector<std::unique_ptr<result::field>>>>(new std::list<std::vector<std::unique_ptr<result::field>>>());
}

//result::~result() {
//}

result::size_type result::affected_rows() const {
	// valid only after INSERT, UPDATE, or DELETE
	return changes;
}

result::const_iterator result::begin() const {
	return const_iterator(rows->begin());
}

result::const_iterator result::end() const {
	return const_iterator(rows->end());
}

}
}

