main: build/main.cpp
	clang++ -std=c++17 build/main.cpp graphs/utils.cpp graphs/KDTree.cpp data/reader.cpp visuals/image.cpp graphs/RoadGraph.cpp -include graphs/utils.h -include graphs/KDTree.h -include data/reader.h -include visuals/image.h -include graphs/RoadGraph.h -o main.out

test: tests/tests.cpp
	clang++ -std=c++17 tests/tests.cpp graphs/utils.cpp graphs/KDTree.cpp data/reader.cpp visuals/image.cpp graphs/RoadGraph.cpp -include graphs/utils.h -include graphs/KDTree.h -include data/reader.h -include visuals/image.h -include graphs/RoadGraph.h -o tests.out

.DEFAULT_GOAL := main
