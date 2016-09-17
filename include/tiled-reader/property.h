#ifndef TILED_PROPERTY_H_
#define TILED_PROPERTY_H_

#include <string>
#include <unordered_map>

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
	~Property();

	int int_value() const;
	bool bool_value() const;
	float float_value() const;
	const std::string& string_value() const;


private:
	union Value {
		int int_;
		bool bool_;
		float float_;
		std::string* string_;
	};

	Type type_;
	Value value_;
};

using PropertyMap = std::unordered_map<std::string, Property>;

} // namespace tiled

#endif // TILED_PROPERTY_H_
