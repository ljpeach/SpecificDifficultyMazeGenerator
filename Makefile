all: maze difficulty
maze: initsetup
	g++ code/maze.cpp -o binaries/mazeGen
difficulty: initsetup
	g++ code/difficulty.cpp -o binaries/mazeDifficulty
initsetup:
	@if [ ! -d "./binaries" ]; then mkdir binaries; fi
	@if [ ! -d "./outs" ]; then mkdir outs; fi
