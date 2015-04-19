#include <jsonSerializer/jsonSerializer.h>
#include <gtest/gtest.h>

#include <iostream>

TEST(ReadWrite, testBool1) {

	bool t1 { false }, t2 { false };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_FALSE(t1);
	EXPECT_EQ(t1, false);
}
TEST(ReadWrite, testBool2) {

	bool t1 { true }, t2 { false };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_TRUE(t1);
	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testBool3) {

	bool t1 { false }, t2 { true };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_FALSE(t1);
	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testBool4) {

	bool t1 { true }, t2 { true };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_TRUE(t1);
	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testUInt8) {
	uint8_t t1 { 42 }, t2 { 0 };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testInt8) {
	int8_t t1 { -42 }, t2 { 0 };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testUInt16) {
	uint16_t t1 { 42 }, t2 { 0 };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testInt16) {
	int16_t t1 { -42 }, t2 { 0 };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testUInt32) {
	uint32_t t1 { 42 }, t2 { 0 };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testInt32) {
	int32_t t1 { -42 }, t2 { 0 };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}

TEST(ReadWrite, testFloat) {
	float t1 { 4.2 }, t2 { 0 };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testDouble) {
	double t1 { 4.2 }, t2 { 0 };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testString) {
	std::string t1 { "42" }, t2 { "" };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testVectorInt) {
	std::vector<int> t1 { 0, 1, 2, 3 }, t2 { 0 };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testListInt) {
	std::list<int> t1 { 0, 1, 2, 3 }, t2 { 0 };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testArrayInt) {
	std::array<int, 4> t1 {{ 0, 1, 2, 3 }}, t2;
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testMapInt) {
	std::map<int, int> t1 { {100, 0}, {200, 1}, {300, 2}, {400, 3} }, t2 { {100, 0}, {200, 0}, {500, 10} };
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}
TEST(ReadWrite, testSetInt) {
	std::set<int> t1 {{ 0, 1, 2, 3 }}, t2 {{ 0, 1, 2, 3 }};
	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1, t2);
}


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
	bool operator!=(C const& c) const {
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

TEST(ReadWrite, Complex) {
	A a1 = { "a1", 17, {0., 10.}};
	T t1 { a1, { "b1", a1, {a1, a1, a1}}, { 7 }};
	T t2;

	jsonSerializer::write("file.json", t1);
	jsonSerializer::read("file.json", t2);

	EXPECT_EQ(t1.a, t2.a);
	EXPECT_EQ(t1.b, t2.b);
	EXPECT_NE(t1.c, t1.c);
}
