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
		node["values"] % values or std::vector<double> {};
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
		node["listOfAs"]    % listOfAs or std::list<A>{};
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
	void deserialize(jsonSerializer::Node&) {
		x = 0; // always set x to zero, never deserialize
	}
	bool operator==(C const& c) const {
		return x == c.x;
	}
};

struct T {
	A a;
	B b;
	C c;
	void serialize(jsonSerializer::Node& node) {
		node["a"] % a;
		node["b"] % b;
		node["c"] % c;
	}
};


int main(int, char**) {
	A a1 = { "a1", 17, {0., 10.}};
	T t1 { a1, { "b1", a1, {a1, a1, a1}}, { 7 }};
	T t2;

	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	std::cout<<std::boolalpha<<"a1==a2:"<<(t1.a==t2.a)<<std::endl;
	std::cout<<std::boolalpha<<"b1==b2:"<<(t1.b==t2.b)<<std::endl;
	std::cout<<std::boolalpha<<"c1==c2:"<<(t1.c==t2.c)<<std::endl;
	try
	{
		struct {
			int i { 10 };
			void serialize(jsonSerializer::Node& node) {
				node["data"] % i;
			}
		} i;
		jsonSerializer::read("file2.json", i);
		std::cout<<i.i<<std::endl;

	} catch(jsonSerializer::Exception const& e) {
		std::cout << e.what() << std::endl;
	}
}
