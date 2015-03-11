#ifndef JSONSERIALIZER_GENERICFACTORY_H
#define JSONSERIALIZER_GENERICFACTORY_H
#include <genericFactory/genericFactory.h>
#include "jsonSerializer.h"

namespace jsonSerializer {
	template<typename T>
	class Converter<genericFactory::Item<T>> {
	public:
		static void   serialize(Node& node, genericFactory::Item<T>& x) {
			Json::Value& value = node.getValue();
			value["type"] = x.getType();
			NodeValue newNode(value["data"], true, node.getFileFormatVersion(), false);
			newNode % (*x);
		}
		static void deserialize(Node& node, genericFactory::Item<T>& x) {
			std::string type;
			node["type"] % type;
			x = genericFactory::Item<T>(type);
			node["data"] % (*x);
		}
	};
}
#endif
