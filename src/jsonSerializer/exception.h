#ifndef JSONSERIALIZER_EXCEPTION_H
#define JSONSERIALIZER_EXCEPTION_H

#include <json/json.h>

namespace jsonSerializer {
	class Exception : public std::exception {
	std::string error;
	public:

		Exception(std::string const& _error)
			: error(_error) {
		}

		char const* what() const noexcept {
			return error.c_str();
		}
	};
	class WrongType : public Exception {
		Json::Value value;
	public:
		WrongType() : Exception("expected different type") {}
		WrongType(Json::Value const& _value, std::string const& _str)
			: Exception(_str)
			, value(_value) {}
		Json::Value const& getValue() const { return value; }
	};
}

#endif

