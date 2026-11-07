CXX ?= g++
CXXFLAGS ?= -O2 -std=c++17 -Wall -Wextra
all: kelly test
kelly: src/main.cpp src/kelly_game.hpp ; $(CXX) $(CXXFLAGS) -o $@ src/main.cpp
test_kelly: tests/test_kelly.cpp src/kelly_game.hpp ; $(CXX) $(CXXFLAGS) -o $@ tests/test_kelly.cpp
test: test_kelly ; ./test_kelly
results.csv: kelly ; ./kelly > $@
clean: ; rm -f kelly test_kelly results.csv
.PHONY: all test clean
