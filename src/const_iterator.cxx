#include "litepqxx.h"

namespace lite
{
namespace pqxx
{

result::const_iterator::const_iterator(std::list<std::vector<std::unique_ptr<result::field>>>::iterator i) {
	iterator = i;
}

//result::const_iterator::~const_iterator() {
//}

result::const_iterator result::const_iterator::operator ++ () {
	return (iterator++);
}

result::const_iterator result::const_iterator::operator ++ (int) {
	return (++ iterator);
}

bool result::const_iterator::operator == (const result::const_iterator & i) const {
	return (iterator == i.iterator);
}

bool result::const_iterator::operator != (const result::const_iterator & i) const {
	return (iterator != i.iterator);
}

result::field & result::const_iterator::operator[] (int i) const {
	return * (* iterator)[i];
}

}
}

