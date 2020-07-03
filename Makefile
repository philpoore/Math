SRC=main.cpp
BIN=math

CPP_OPTS=-g -DDEBUG
CPP_OPTS_RELEASE=-O3

all: debug

debug:
	g++ $(SRC) -o $(BIN) $(CPP_OPTS) 

release:
	g++ $(SRC) -o $(BIN) $(CPP_OPTS_RELEASE)

test: release
	./test/test.sh

test-fuzz:
	./test/test.sh fuzz_test_01.txt

test-all: test test-fuzz