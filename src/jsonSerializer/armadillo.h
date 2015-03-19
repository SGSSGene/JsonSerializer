#ifndef JSONSERIALIZER_ARMADILLO_H
#define JSONSERIALIZER_ARMADILLO_H

#include <armadillo>
#include "jsonSerializer.h"
#include <sstream>
#include <iomanip>

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
	template<>
	class Converter<arma::mat> {
	public:
		static void   serialize(Node& node, arma::mat& x) {
			node.getValue()["rows"] = uint16_t(x.n_rows);
			node.getValue()["cols"] = uint16_t(x.n_cols);
			for (uint row(0); row < x.n_rows; ++row) {
				for (uint col(0); col < x.n_cols; ++col) {
					std::string name = std::string("m") + std::to_string(row) + "x" + std::to_string(col);
					node.getValue()[name] = x(row, col);
				}
			}
		}
		static void deserialize(Node& node, arma::mat& x) {
			uint16_t rows;
			uint16_t cols;
			rows = node.getValue()["rows"].asUInt();
			cols = node.getValue()["cols"].asUInt();
			x = arma::zeros(rows ,cols);

			for (uint row(0); row < x.n_rows; ++row) {
				for (uint col(0); col < x.n_cols; ++col) {
					std::string name = std::string("m") + std::to_string(row) + "x" + std::to_string(col);
					x(row, col) = node.getValue()[name].asDouble();
				}
			}
		}
	};

}


#endif
