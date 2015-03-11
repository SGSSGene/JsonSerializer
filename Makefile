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
	        src/json/jsoncpp.cpp \
	        src/test/main.cpp
