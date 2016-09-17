#ifndef TILED_PROPERTY_H_
#define TILED_PROPERTY_H_

#include <memory>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <cstdint>

class JSONNode;

namespace tiled {

class Property {
public:
	enum class Type {
		STRING,
		INT,
		BOOL,
		FLOAT
	};
	Property(const JSONNode& value, const JSONNode& type);

	int int_value() const;
	bool bool_value() const;
	float float_value() const;
	const std::string& string_value() const;


private:
	union Value {
		int int_;
		bool bool_;
		float float_;
		std::string string_;
	};

	Property::Type type_;
	Value value_;
};

} // namespace tiled

#endif // TILED_PROPERTY_H_
