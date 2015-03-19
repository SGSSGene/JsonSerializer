#include <jsonSerializer/armadillo.h>
#include <gtest/gtest.h>

TEST(ReadWrite, testMatList) {

	std::vector<arma::mat> vec1, vec2;
	vec1.push_back(arma::colvec{1., 2., 3.});

	jsonSerializer::write("file.json", vec1);
	jsonSerializer::read("file.json", vec2);

	EXPECT_EQ(vec1.size(), vec2.size());
	for (size_t i(0); i<vec1.size(); ++i) {
		EXPECT_EQ(vec1[i].n_rows, vec2[i].n_rows);
		EXPECT_EQ(vec1[i].n_cols, vec2[i].n_cols);

		for (uint row(0); row<vec1[i].n_rows; ++row) {
			for (uint col(0); col<vec1[i].n_cols; ++col) {
				EXPECT_EQ(vec1[i](row, col), vec2[i](row, col));
			}
		}
	}
}


