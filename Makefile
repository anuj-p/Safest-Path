main: build/main.cpp
	clang++ -std=c++17 build/main.cpp graphs/utils.cpp graphs/KDTree.cpp data/reader.cpp visuals/image.cpp visuals/RoadImage.cpp visuals/lodepng.cpp graphs/RoadGraph.cpp -include graphs/utils.h -include graphs/KDTree.h -include data/reader.h -include visuals/image.h -include graphs/RoadGraph.h -include visuals/RoadImage.h -include visuals/lodepng.h -o main.out

tests: tests/tests.cpp
	clang++ -std=c++17 tests/tests.cpp graphs/utils.cpp graphs/KDTree.cpp data/reader.cpp visuals/image.cpp visuals/RoadImage.cpp visuals/lodepng.cpp graphs/RoadGraph.cpp -include graphs/utils.h -include graphs/KDTree.h -include data/reader.h -include visuals/image.h -include graphs/RoadGraph.h -include visuals/RoadImage.h -include visuals/lodepng.h -o tests.out

.DEFAULT_GOAL := main
