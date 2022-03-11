# SpecificDifficultyMazeGenerator
## Project Description
I am working on creating a system for generating mazes of a specified difficulty using difficulty models. While there has been research into ranking existing maze generation algorithms by average resultant difficulty, there has been little in regards to using a single algorithm to generate mazes based on a difficulty input. To achieve this, I will be writing a maze algorithm with a number of parameters that will be used together to influence maze generation. A neural network will take the desired difficulty of the maze as an input, and output the parameters for the maze. Difficulty models will be used to analyze the maze and train the neural network. Finally, the final sets of mazes will have “real” difficulty measured by a number of agent functions.

Code for generating mazes of specific difficulty.
    (currently only generates mazes of specified size and a few traversal styles)
Uses g++ to compile.

## Usage:
    testMaze [length] [width] [old node push location low] [old node push location low] [new node push location low] [new node push location low] [pop location low] [pop location high] [left bias] [straight bias] [right bias]

### Example run after calling make:
binaries/testMaze 10 10 0 0 0 0 0 0 1 1 1 > outs/maze.txt ; python3 code/mazeVis.py outs/maze.txt

## Architecture Diagram
![Architecture Diagram](SoftwareArchitectureDiagram.jpg)

## Current issues:
- Intersection ratios are not implemented
- Solution path limited to top left and bottom right corners
- Neural Network not set up
- Difficulty models not written
- Agent functions for data analysis not written
- Overarching UI not implemented
