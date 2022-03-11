# SpecificDifficultyMazeGenerator

Code for generating mazes of specific difficulty.
    (currently only generates mazes of specified size and a few traversal styles)
Uses g++ to compile.

Usage:
    testMaze [length] [width] [old node push location low] [old node push location low] [new node push location low] [new node push location low] [pop location low] [pop location high] [left bias] [straight bias] [right bias]

Example run after calling make:
binaries/testMaze 10 10 0 0 0 0 0 0 1 1 1 > outs/maze.txt ; python3 code/mazeVis.py outs/maze.txt


Current issues:
    Intersection ratios are not implemented
    Solution path limited to top left and bottom right corners
    Neural Network not set up
    Difficulty models not written
    Agent functions for data analysis not written
    Overarching UI not implemented
