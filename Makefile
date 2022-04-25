all: maze difficulty tests
maze: initsetup
	g++ code/maze.cpp -o binaries/mazeGen
difficulty: initsetup
	g++ code/difficulty.cpp -o binaries/mazeDifficulty
tests: initsetup
	g++ code/tests.cpp -o binaries/testSuite
initsetup:
	@if [ ! -d "./binaries" ]; then mkdir binaries; fi
	@if [ ! -d "./outs" ]; then mkdir outs; fi
