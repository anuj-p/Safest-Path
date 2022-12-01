main: build/main.cpp
	clang++ -std=c++17 build/main.cpp visuals/image.cpp -include visuals/image.h -o main.out

tests: tests/tests.cpp
	clang++ -std=c++17 tests/tests.cpp visuals/image.cpp -include visuals/image.h -o tests.out

.DEFAULT_GOAL := main
