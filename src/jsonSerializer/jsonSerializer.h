#ifndef Converter_Converter_H
#define Converter_Converter_H

#include <array>
#include <exception>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "exception.h"
#include <iostream>

Json::Reader* readerPtr { nullptr };

namespace jsonSerializer {

	template<typename ...T>
	class Converter;

	class NodeValue;

	template<typename T>
	class NodeValueDefault;

	class Node {
	protected:
		Json::Value& value;
		bool serialize;
	public:
		Node(Json::Value& _value, bool _serialize)
			: value(_value)
			, serialize(_serialize) {
		}
		bool isSerializing() const {
			return serialize;
		}
		Json::Value& getValue() { return value; }

		NodeValue operator[](std::string const& _s);
	};

	class NodeValue : public Node {
	private:
		bool defaultValueNeeded;
	public:
		NodeValue(Json::Value& _value, bool _serialize, bool _defaultValueNeeded)
			: Node(_value, _serialize)
			, defaultValueNeeded(_defaultValueNeeded) {}
		template<typename T>
		NodeValueDefault<T> operator%(T& x);
	};


	template<typename T>
	class NodeValueDefault {
	private:
		T& t;
		bool needDefaultValue;
	public:
		NodeValueDefault(T& _t, bool _needDefaultValue)
			: t(_t)
			, needDefaultValue(_needDefaultValue) {}

		void operator or(T const& _t) {
			if (needDefaultValue) {
				t = _t;
				needDefaultValue = false;
			}
		}

	};

	inline NodeValue Node::operator[](std::string const& _s) {
		bool defaultValueNeeded = not value.isMember(_s);
		return NodeValue(value[_s], serialize, defaultValueNeeded);
	}

	template<typename T>
	inline NodeValueDefault<T> NodeValue::operator%(T& x) {
		if (serialize) {
			Converter<T>::serialize(*this, x);
			defaultValueNeeded = false;
		} else if (not defaultValueNeeded) {
			Converter<T>::deserialize(*this, x);
		}
		return NodeValueDefault<T>(x, defaultValueNeeded);
	}


	template<>
	class Converter<bool> {
	public:
		static void   serialize(Node& node, bool& x) {
			node.getValue() = x;
		}
		static void deserialize(Node& node, bool& x) {
			if (not node.getValue().isBool()) throw WrongType(node.getValue(), "expected bool");
			x = node.getValue().asBool();
		}
	};

	template<>
	class Converter<uint8_t> {
	public:
		static void   serialize(Node& node, uint8_t& x) {
			node.getValue() = x;
		}
		static void deserialize(Node& node, uint8_t& x) {
			if (not node.getValue().isUInt()) throw WrongType(node.getValue(), "expected uint8_t");
			uint32_t t = node.getValue().asUInt();
			if (t > (1<<8)-1) throw WrongType(node.getValue(), "expected uint8_t");

			x = t;
		}
	};
	template<>
	class Converter<int8_t> {
	public:
		static void   serialize(Node& node, int8_t& x) {
			node.getValue() = x;
		}
		static void deserialize(Node& node, int8_t& x) {
			if (not node.getValue().isInt()) throw WrongType(node.getValue(), "expected int8_t");
			int32_t t = node.getValue().asInt();
			if (t > (1<<7)-1 || t < -128) throw WrongType(node.getValue(), "expected int8_t");

			x = t;
		}
	};
	template<>
	class Converter<uint16_t> {
	public:
		static void   serialize(Node& node, uint16_t& x) {
			node.getValue() = x;
		}
		static void deserialize(Node& node, uint16_t& x) {
			if (not node.getValue().isUInt()) throw WrongType(node.getValue(), "expected uint16_t");
			uint32_t t = node.getValue().asUInt();
			if (t > (1<<16)-1) throw WrongType(node.getValue(), "expected uint16_t");

			x = t;
		}
	};
	template<>
	class Converter<int16_t> {
	public:
		static void   serialize(Node& node, int16_t& x) {
			node.getValue() = x;
		}
		static void deserialize(Node& node, int16_t& x) {
			if (not node.getValue().isInt()) throw WrongType(node.getValue(), "expected int16_t");
			int32_t t = node.getValue().asInt();
			if (t > (1<<15)-1 || t < -32768 ) throw WrongType(node.getValue(), "expected int16_t");

			x = t;
		}
	};
	template<>
	class Converter<uint32_t> {
	public:
		static void   serialize(Node& node, uint32_t& x) {
			node.getValue() = x;
		}
		static void deserialize(Node& node, uint32_t& x) {
			if (not node.getValue().isUInt()) throw WrongType(node.getValue(), "expected uint32_t");
			x = node.getValue().asUInt();
		}
	};

	template<>
	class Converter<int32_t> {
	public:
		static void   serialize(Node& node, int32_t& x) {
			node.getValue() = x;
		}
		static void deserialize(Node& node, int32_t& x) {
			if (not node.getValue().isInt()) throw WrongType(node.getValue(), "expected int32_t");
			x = node.getValue().asInt();
		}
	};
	template<>
	class Converter<float> {
	public:
		static void   serialize(Node& node, float& x) {
			node.getValue() = x;
		}
		static void deserialize(Node& node, float& x) {
			if (not node.getValue().isDouble()) throw WrongType(node.getValue(), "expected float");
			x = node.getValue().asFloat();
		}
	};

	template<>
	class Converter<double> {
	public:
		static void   serialize(Node& node, double& x) {
			node.getValue() = x;
		}
		static void deserialize(Node& node, double& x) {
			if (not node.getValue().isDouble()) throw WrongType(node.getValue(), "expected double");
			x = node.getValue().asDouble();
		}
	};

/*	template<>
	class Converter<Second> {
	public:
		static void   serialize(Node& node, Second& x) {
			node.getValue() = x.value();
		}
		static void deserialize(Node& node, Second& x) {
			x = node.getValue().asDouble() * seconds;
		}
	};*/

	template<>
	class Converter<std::string> {
	public:
		static void   serialize(Node& node, std::string& x) {
			node.getValue() = x;
		}
		static void deserialize(Node& node, std::string& x) {
			if (not node.getValue().isString()) throw WrongType(node.getValue(), "expected string");
			x = node.getValue().asString();
		}
	};

	template<typename T>
	class Converter<std::vector<T>> {
	public:
		static void   serialize(Node& node, std::vector<T>& x) {
			node.getValue() = Json::arrayValue;
			for (auto& e : x) {
				Json::Value value;
				NodeValue newNode(value, node.isSerializing(), false);
				newNode % e;
				node.getValue().append(value);
			}
		}
		static void deserialize(Node& node, std::vector<T>& x) {
			if (not node.getValue().isArray()) throw WrongType(node.getValue(), "expected array");
			x.clear();
			for (uint i(0); i<node.getValue().size(); ++i) {
				x.push_back(T());
				NodeValue newNode(node.getValue()[i], node.isSerializing(), false);
				newNode % x.back();
			}
		}
	};
	template<typename T>
	class Converter<std::list<T>> {
	public:
		static void   serialize(Node& node, std::list<T>& x) {
			node.getValue() = Json::arrayValue;
			for (auto e : x) {
				Json::Value value;
				NodeValue newNode(value, node.isSerializing(), false);
				newNode % e;
				node.getValue().append(value);
			}
		}
		static void deserialize(Node& node, std::list<T>& x) {
			if (not node.getValue().isArray()) throw WrongType(node.getValue(), "expected array");
			x.clear();
			for (uint i(0); i<node.getValue().size(); ++i) {
				x.push_back(T());
				NodeValue newNode(node.getValue()[i], node.isSerializing(), false);
				newNode % x.back();
			}
		}
	};
	template<typename T, std::size_t N>
	class Converter<std::array<T, N>> {
	public:
		static void   serialize(Node& node, std::array<T, N>& x) {
			node.getValue() = Json::arrayValue;
			for (auto& e : x) {
				Json::Value value;
				NodeValue newNode(value, node.isSerializing(), false);
				newNode % e;
				node.getValue().append(value);
			}
		}
		static void deserialize(Node& node, std::array<T, N>& x) {
			if (not node.getValue().isArray()) throw WrongType(node.getValue(), "expected array");
			for (uint i(0); i<node.getValue().size(); ++i) {
				NodeValue newNode(node.getValue()[i], node.isSerializing(), false);
				newNode % x[i];
			}
		}
	};


	template<typename T1, typename T2>
	class Converter<std::map<T1, T2>> {
	public:
		static void   serialize(Node& node, std::map<T1, T2>& x) {
			node.getValue() = Json::arrayValue;
			for (auto& e : x) {
				T1  key   = e.first;
				T2& value = e.second;
				Json::Value jsonValue;
				NodeValue(jsonValue["key"], true, false)   % key;
				NodeValue(jsonValue["value"], true, false) % value;
				node.getValue().append(jsonValue);
			}
		}
		static void deserialize(Node& node, std::map<T1, T2>& x) {
			if (not node.getValue().isArray()) throw WrongType(node.getValue(), "expected array");
			x.clear();
			for (uint i(0); i<node.getValue().size(); ++i) {
				T1 key;
				T2 value;
				NodeValue(node.getValue()[i]["key"], false, false)   % key;
				NodeValue(node.getValue()[i]["value"], false, false) % value;
				x[key] = std::move(value);
			}
		}
	};

	template<typename ...T>
	class ConverterSplit;

	template<typename T>
	class Converter<T> {
	public:
		static void serialize(Node& node, T& x) {
			x.serialize(node);
		}
		static void deserialize(Node& node, T& x) {
			 _deserialize(node, x, 0);
		}
	};

	template<typename T>
	auto _deserialize(Node& node, T& x, int)
		-> decltype(x.deserialize(node)) {
		x.deserialize(node);
	}
	template<typename T>
	auto _deserialize(Node& node, T& x, long)
		-> decltype(x.serialize(node)){
		x.serialize(node);
	}



	template<typename T>
	void read(std::string const& _file, T& _data) {
		Json::Value root;
		NodeValue node(root, false, false);
		std::ifstream ifs(_file);
		if (ifs.fail()) {
			throw Exception("Opening file failed");
		}
		std::stringstream strStream;
		strStream << ifs.rdbuf();
		Json::Reader reader;
		readerPtr = &reader;
		if (not reader.parse(strStream.str(), root)) {
			throw Exception("Parsing file failed: "+reader.getFormattedErrorMessages());
		}
		try {
			node % _data;
		} catch(WrongType const& type) {
			reader.pushError(type.getValue(), type.what(), type.getValue());
			throw Exception("Parsing failed: "+ reader.getFormattedErrorMessages());
		}
	}
	template<typename T>
	void write(std::string const& _file, T& _data) {
		Json::Value root;
		NodeValue node(root, true, false);
		node % _data;
		Json::StyledWriter jsonWriter;
		std::string jsonString = jsonWriter.write(root);
		std::ofstream oFile(_file);
		if (oFile.fail()) {
			throw Exception("Opening file failed");
		}
		oFile << jsonString;
		oFile.close();
		if (oFile.fail()) {
			throw Exception("Writing to file failed");
		}
	}
}
#endif
