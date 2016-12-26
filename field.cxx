#include <iostream>
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
	return (T)0; // error
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
	return 0; // error
}

double result::field::as_float() const {
	return 0; // error
}

std::string result::field::as_text() const {
	return ""; // error
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

bool result::null_field::is_null () const {
	return true;
}

}
}

