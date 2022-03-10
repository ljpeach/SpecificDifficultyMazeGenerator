maze: initsetup
	g++ code/maze.cc -o binaries/testMaze
initsetup:
	if [ ! -d "./binaries" ]; then mkdir binaries; fi
	if [ ! -d "./outs" ]; then mkdir outs; fi
