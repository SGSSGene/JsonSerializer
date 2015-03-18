# JsonSerializer
Classes and interfaces to easily serialize objects into json objects.
One function for serialization and deserialization. Different function for serialization/deserialization possible if needed.

## Dependencies
	Uses https://github.com/open-source-parsers/jsoncpp . Source is directly integrated
	Uses https://code.google.com/p/googletest/ . Source is directly integrated
	

## Support
Default supported datatypes:
* bool
* uint8_t/int8_t
* uint16_t/int16_t
* uint32_t/int32_t
* float/double
* std::string
* std::array
* std::vector
* std::list
* std::map
* extendable for other data types

## Example
### Function declaration
```c
#include <jsonSerializer/jsonSerializer.h>
class A {
private:
	std::string name;
	uint32_t x;
	std::vector<double> values;
public:
	void serialize(jsonSerializer::Node& node) {
		node["name"]   % name or "unknown"; //(de-)serialize name, if name is not set, set it to "unknown"
		node["x"]      % x or 0;
		node["values"] % values or std::vector<double>{};
	}
};

int main() {
	A a1, a2;
	jsonSerializer::write("file.json", a1);
	jsonSerializer::read("file.json", a2);
	// a1 and a2 are now the same
	return 0;
}
```

