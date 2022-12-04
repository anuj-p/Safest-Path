main: build/main.cpp
	clang++ -std=c++17 tests/tests.cpp visuals/image.cpp graphs/RoadGraph.cpp graphs/KDTree.cpp -include visuals/image.h -include graphs/RoadGraph.h  -include graphs/RoadNode.h -include graphs/Point.h -include graphs/KDTree.h -include graphs/Point.h -include graphs/reader.h -include graphs/RoadEdge.h -o main.out

tests: tests/tests.cpp
	clang++ -std=c++17 tests/tests.cpp visuals/image.cpp graphs/RoadGraph.cpp graphs/KDTree.cpp -include visuals/image.h -include graphs/RoadGraph.h  -include graphs/RoadNode.h -include graphs/Point.h -include graphs/KDTree.h -include graphs/Point.h -include graphs/reader.h -include graphs/RoadEdge.h -o tests.out

.DEFAULT_GOAL := main
