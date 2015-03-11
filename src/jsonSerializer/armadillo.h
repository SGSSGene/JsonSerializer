#ifndef JSONSERIALIZER_ARMADILLO_H
#define JSONSERIALIZER_ARMADILLO_H

#include <armadillo>
#include "jsonSerializer.h"

namespace jsonSerializer {
	template<>
	class Converter<arma::mat44> {
	public:
		static void   serialize(Node& node, arma::mat44& x) {
			for (uint i(0); i < x.n_rows; ++i) {
				std::stringstream matrixSS, nameSS;
				matrixSS.precision(10);
				matrixSS << std::fixed << std::setw(17);
				x.row(i).raw_print(matrixSS);
				nameSS << "matrix" << int(i);
				std::string s = matrixSS.str();
				node.getValue()[nameSS.str()] = s;
			}
		}
		static void deserialize(Node& node, arma::mat44& x) {
			x = arma::eye(4, 4);
			for (uint i(0); i < x.n_rows; ++i) {
				std::string s;
				std::stringstream matrixSS, nameSS;
				nameSS << "matrix" << int(i);
				s = node.getValue()[nameSS.str()].asString();
				x.row(i) = arma::rowvec(s);
			}

		}
	};
}


#endif
