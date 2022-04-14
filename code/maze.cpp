#include <iostream>
#include "maze.hpp"

//g++ maze.cc -o testMaze;./testMaze
//./testMaze [len] [wid] 0 0 0 0 0 0 1 1 1 0 - Creates stack-like/DFS behavior
//./testMaze [len] [wid] 1 1 1 1 0 0 1 1 1 0 - Creates queue-like/BFS behavior
//./testMaze [len] [wid] 0 1 0 1 0 1 1 1 1 0 - Creates random behavior
int main(int argc, const char *argv[]){
    //use atoi to cast from arg to int
    std::srand(std::time(0));
    Maze testMaze = Maze(atoi(argv[1]),atoi(argv[2]));
    MazeNode* c = testMaze.corner;
    std::tuple<int,int> a[] = {std::make_tuple(atoi(argv[1])/2,atoi(argv[2])/2)};//std::make_tuple(0,0),
    //char* txt = testMaze->toString();
    //printf("Test Maze:\n%s",txt);
    float mazeParam[6];
    float biases[3];
    int i;
    for(i=0; i<6; i++){
        mazeParam[i] = atof(argv[i+3]);
    }
    for(i = 0; i<3; i++){
        biases[i] = atof(argv[i+9]);
    }
    //printf("%f\n", biases[0]);
    testMaze.buildMaze(1, a, mazeParam, biases);
    if(argc<13){
        char* txt = testMaze.toString();
        printf("%s",txt);
    }
    else{
        float solutionRankRatio = atof(argv[12]);
        std::tuple<MazeNode*, MazeNode*> entex = testMaze.solutionNodes(solutionRankRatio);
        std::get<0>(entex)->display = '@';
        std::get<1>(entex)->display = '@';
        char* txt = testMaze.toString();
        printf("%s",txt);
    }
}
