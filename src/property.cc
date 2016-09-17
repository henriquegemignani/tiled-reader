#include <tiled-reader/property.h>

#include <tiled-reader/exceptions.h>
#include <libjson.h>
#include <cassert>

namespace tiled {

namespace {
	namespace {
		std::unordered_map<json_string, Property::Type> supported_types = {
			{ "bool", Property::Type::BOOL },
			{ "string", Property::Type::STRING },
			{ "int", Property::Type::INT },
			{ "float", Property::Type::FLOAT },
		};
	}
}

Property::Property(const JSONNode& value, const JSONNode& type) {
	try {
		type_ = supported_types.at(type.as_string());
	}
	catch (std::out_of_range) {
		throw tiled::BaseException("Invalid type.");
	}
	switch (type_)
	{
	case Type::STRING:
		value_.string_ = new std::string(value.as_string().c_str());
		break;
	case Type::INT:
		value_.int_ = value.as_int();
		break;
	case Type::BOOL:
		value_.bool_ = value.as_bool();
		break;
	case Type::FLOAT:
		value_.float_ = value.as_float();
		break;
	}
}

Property::~Property() {
	if (type_ == Type::STRING)
		delete value_.string_;
}

int Property::int_value() const {
	assert(type_ == Property::Type::INT);
	return value_.int_;
}

bool Property::bool_value() const {
	assert(type_ == Property::Type::BOOL);
	return value_.bool_;
}

float Property::float_value() const {
	assert(type_ == Property::Type::FLOAT);
	return value_.float_;
}

const std::string& Property::string_value() const {
	assert(type_ == Property::Type::STRING);
	return *value_.string_;
}

} // namespace tiled