main: main.cpp
	clang++ -std=c++17 build/main.cpp -o main.out

tests: tests/tests.cpp
	clang++ -std=c++17 tests/tests.cpp -o tests.out

.DEFAULT_GOAL := main
