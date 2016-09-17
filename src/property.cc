#include <tiled-reader/property.h>

#include <tiled-reader/exceptions.h>
#include <libjson.h>

namespace tiled {

namespace {
	namespace {
		std::unordered_map<std::string, Property::Type> supported_types = {
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
}

} // namespace tiled