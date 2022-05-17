#include <iostream>
#include <ctime>
#include "agents.hpp"

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
        std::get<0>(entex)->display = '@';
        std::get<1>(entex)->display = '@';
        char* txt = testMaze.toString();
        // printf("%sDifficulty: %f\n",txt , mcd);
        // printf("%s",txt);
        struct DiffOutput out;
        out = bfs(entex);
        printf("BFS performance:\n\tTime: %f\n\tNodes Expanded: %d\n\tNodes Generated: %d\n", out.duration, out.nodeExpand, out.nodeGen);
        out = dfs(entex);
        printf("DFS performance:\n\tTime: %f\n\tNodes Expanded: %d\n\tNodes Generated: %d\n", out.duration, out.nodeExpand, out.nodeGen);
        out = astar(entex);
        printf("A* performance:\n\tTime: %f\n\tNodes Expanded: %d\n\tNodes Generated: %d\n", out.duration, out.nodeExpand, out.nodeGen);
        out = greedy(entex);
        printf("Greedy performance:\n\tTime: %f\n\tNodes Expanded: %d\n\tNodes Generated: %d\n", out.duration, out.nodeExpand, out.nodeGen);

    }
    // delete testMaze;
}
