all:
	clang++ -std=c++11 \
	        -O0 \
	        -ggdb \
	        -Wall \
	        -Wextra \
	        -fmessage-length=0 \
	        -pthread \
	        -Isrc \
	        -o test \
	        libs/gtest-1.7.0/src/gtest-all.cc \
	        libs/gtest-1.7.0/src/gtest_main.cc \
	        -isystem libs/gtest-1.7.0/ \
	        -isystem libs/gtest-1.7.0/include \
	        -isystem libs/armadillo/include \
	        src/json/jsoncpp.cpp \
	        src/test/main.cpp \
	        src/test/armadilloTest.cpp
