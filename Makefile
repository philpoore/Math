SRC=main.cpp
BIN=math

CPP_OPTS=-g -DDEBUG
CPP_OPTS_RELEASE=-O3

all: debug

debug:
	g++ $(SRC) -o $(BIN) $(CPP_OPTS) 

release:
	g++ $(SRC) -o $(BIN) $(CPP_OPTS_RELEASE)

format:
	clang-format -i ./src/*.cpp ./src/*.hpp

test: release
	./test/test.sh tests.txt

test-fuzz:
	./test/test.sh fuzz_test_01.txt

test-hard:
	./test/test.sh fuzz_test_hard.txt

test-all: test test-fuzz


check:
	./test/test.sh tests.txt fuzz_test_01.txt