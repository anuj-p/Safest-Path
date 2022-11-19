main: main.cpp
	clang++ main.cpp -o main.out

tests: tests/tests.cpp
	clang++ tests/tests.cpp -o tests.out

.DEFAULT_GOAL := main