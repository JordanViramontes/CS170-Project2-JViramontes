# SOURCES := ./src/main.cpp ./src/graph.cpp ./src/board.cpp ./src/ui.cpp

SOURCES := ./src/main.cpp ./src/graph.cpp ./src/nearestNeighbor.cpp

make : $(SOURCES)
	g++ -g -Wall -o a.exe $(SOURCES)