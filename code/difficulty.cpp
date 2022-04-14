#include <iostream>
#include "difficulty.hpp"

int main(int argc, const char *argv[]){
    std::srand(std::time(0));
    Maze testMaze = Maze(atoi(argv[1]),atoi(argv[2]));
    MazeNode* c = testMaze.corner;
    std::tuple<int,int> a[] = {std::make_tuple(atoi(argv[1])/2,atoi(argv[2])/2)};//std::make_tuple(0,0),
    float mazeParam[6];
    float biases[3];
    int i;
    for(i=0; i<6; i++){
        mazeParam[i] = atof(argv[i+3]);
    }
    for(i = 0; i<3; i++){
        biases[i] = atof(argv[i+9]);
    }
    testMaze.buildMaze(1, a, mazeParam, biases);
    if(argc<13){
        char* txt = testMaze.toString();
        printf("%s",txt);
    }
    else{
        float solutionRankRatio = atof(argv[12]);
        std::tuple<MazeNode*, MazeNode*> entex = testMaze.solutionNodes(solutionRankRatio);
        char* txt = testMaze.toString();
        printf("%s",txt);
        float mcd = mcclendonDiff(testMaze, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        float bfun = bellotFun(testMaze, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        //txt = testMaze.toString();
        printf("Difficulty: %f\nFunness: %f\n", mcd, bfun);
    }
}
