#include <string.h>
#include "litepqxx.h"

namespace lite
{
namespace pqxx
{

binarystring::binarystring(const std::string & s) {
	length = s.length();
	address = malloc(length);
	memcpy(address, s.c_str(), length);
}

binarystring::binarystring (const void * addr, size_t len) : length(len) {
	address = malloc(length);
	memcpy(address, addr, length);
}

binarystring::~binarystring () {
	free(address);
}

}
}

