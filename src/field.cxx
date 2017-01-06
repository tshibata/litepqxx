#include <string.h>
#include "litepqxx.h"

namespace lite
{
namespace pqxx
{

result::field::~field() {
}

bool result::field::is_null () const {
	return false;
}

template<typename T> T result::field::as () const {
	throw failure(); // overridden, or unknown type
}

template<> int result::field::as () const {
	return as_integer();
}

template<> double result::field::as () const {
	return as_float();
}

template<> std::string result::field::as () const {
	return as_text();
}

int result::field::as_integer() const {
	throw failure(); // overridden, or incompatible type
}

double result::field::as_float() const {
	throw failure(); // overridden, or incompatible type
}

std::string result::field::as_text() const {
	throw failure(); // overridden, or incompatible type
}

result::integer_field::integer_field(int value) {
	this->value = value;
}

int result::integer_field::as_integer() const {
	return value;
}

result::float_field::float_field(double value) {
	this->value = value;
}

double result::float_field::as_float() const {
	return value;
}

result::text_field::text_field(const unsigned char * value) {
	this->value = std::string((const char *) value);
}

std::string result::text_field::as_text() const {
	return value;
}

result::blob_field::blob_field(const void * addr, size_t len) : length(len) {
	address = malloc(length);
	memcpy(address, addr, length);
}

result::blob_field::~blob_field() {
	free(address);
}

std::string result::blob_field::as_text() const {
	return std::string((char *) address, length);
}

bool result::null_field::is_null () const {
	return true;
}

}
}

