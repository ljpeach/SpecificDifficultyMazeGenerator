# SpecificDifficultyMazeGenerator

Code for generating mazes of specific difficulty.
    (currently only generates mazes of specified size and a few traversal styles)
Uses g++ to compile.

Example run after calling make:
binaries/testMaze 10 10 0 0 0 0 0 0 1 1 1 > outs/maze.txt ; python3 code/mazeVis.py outs/maze.txt


Current issues:
    Sets are not joined when using multiple starting coordinates
    Intersection ratios are not implemented
    Solution path limited to top left and bottom right corners
    Neural Network not set up
    Difficulty models not written
    Agent functions for data analysis not written
    Overarching UI not implemented
