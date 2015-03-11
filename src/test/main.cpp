#include <jsonSerializer/jsonSerializer.h>

#include <iostream>

class A {
public:
	std::string name;
	uint32_t x;
	std::vector<double> values;

	void serialize(jsonSerializer::Node& node) {
		node["name"]   % name or "unknown"; //(de-)serialize name, if name is not set, set it to "unknown"
		node["x"]      % x or 0;
		node["values"] % values;
	}
	bool operator==(A const& a) const {
		return name   == a.name &&
		       x      == a.x &&
		       values == a.values;
	}
};
class B {
public:
	std::string name;
	A singleValue;
	std::list<A> listOfAs;

	void serialize(jsonSerializer::Node& node) {
		node["name"]        % name or "unknown";
		node["singleValue"] % singleValue;
		node["listOfAs"]    % listOfAs;
	}
	bool operator==(B const& b) const {
		return name        == b.name &&
		       singleValue == b.singleValue &&
		       listOfAs    == b.listOfAs;
	}
};
class C {
public:
	int x;

	void serialize(jsonSerializer::Node& node) {
		node["x"] % x;
	}
	void deserialize(jsonSerializer::Node& node) {
		x = 0; // always set x to zero, never deserialize
	}
	bool operator==(C const& c) const {
		return x == c.x;
	}
};


int main(int, char**) {
	A a1 { "a1", 17, {0., 10.}};
	B b1 { "b1", a1, {a1, a1, a1}};
	C c1 { 7 };
	A a2;
	B b2;
	C c2;

	{
		jsonSerializer::Writer writer("file.json", 0);
		writer["a"] % a1;
		writer["b"] % b1;
		writer["c"] % c1;

	}

	{
		jsonSerializer::Reader reader("file.json");
		reader["a"] % a2;
		reader["b"] % b2;
		reader["c"] % c2;
	}
	std::cout<<std::boolalpha<<"a1==a2:"<<(a1==a2)<<std::endl;
	std::cout<<std::boolalpha<<"b1==b2:"<<(b1==b2)<<std::endl;
	std::cout<<std::boolalpha<<"c1==c2:"<<(c1==c2)<<std::endl;
}
