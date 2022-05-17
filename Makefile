all: maze difficulty tests agents
maze: initsetup
	g++ code/maze.cpp -o binaries/mazeGen
difficulty: initsetup
	g++ code/difficulty.cpp -o binaries/mazeDifficulty
tests: initsetup
	g++ code/tests.cpp -o binaries/testSuite
agents: initsetup
		g++ code/agent.cpp -o binaries/agents
initsetup:
	@if [ ! -d "./binaries" ]; then mkdir binaries; fi
	@if [ ! -d "./outs" ]; then mkdir outs; fi
