#include <iostream>
#include <ctime>
#include "maze.hpp"
#include "difficulty.hpp"
#include "agents.hpp"

int main(int argc, const char* argv[]){
    //setup
    std::srand(std::time(0));
    int length, width, i, j;
    length = width = atoi(argv[1]);
    Maze* overworked;
    std::tuple<MazeNode*, MazeNode*> entex;
    std::tuple<int,int> coords[] = {std::make_tuple(0,0), std::make_tuple(0,width-1), std::make_tuple(length/2, width/2), std::make_tuple(length-1,0), std::make_tuple(length-1,width-1)};
    float mazeParam[6];
    float biases[3];
    float solRank = 0;
    float avg = 0;
    float mcclend;
    float avgFun = 0;
    float temp = 0;
    double avgTime = 0;
    int avgExpandBfs = 0;
    int avgGeneratedBfs = 0;
    int avgExpandDfs = 0;
    int avgGeneratedDfs = 0;
    int avgExpandAstar = 0;
    int avgGeneratedAstar = 0;
    int avgExpandGreedy = 0;
    int avgGeneratedGreedy = 0;
    struct DiffOutput out;

    //tests
    printf("Start Coord Tests\n");
    mazeParam[0] = 0;
    mazeParam[1] = 1;
    mazeParam[2] = 0;
    mazeParam[3] = 1;
    mazeParam[4] = 0;
    mazeParam[5] = 1;
    biases[0] = 1;
    biases[1] = 1;
    biases[2] = 1;
    for(i=0; i<5; i++){
        for(j=0; j<1000; j++){
            overworked = new Maze(length, width);
            overworked->buildMaze(i+1, coords, mazeParam, biases);
            entex = overworked->solutionNodes(solRank);
            mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
            avg += mcclend;
            avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
            out = bfs(entex);
            avgExpandBfs += out.nodeExpand;
            avgGeneratedBfs += out.nodeGen;
            out = dfs(entex);
            avgExpandDfs += out.nodeExpand;
            avgGeneratedDfs += out.nodeGen;
            out = astar(entex);
            avgExpandAstar += out.nodeExpand;
            avgGeneratedAstar += out.nodeGen;
            out = greedy(entex);
            avgExpandGreedy += out.nodeExpand;
            avgGeneratedGreedy += out.nodeGen;
        }
        avg/=1000;
        avgFun/=1000;
        avgExpandBfs/=1000;
        avgGeneratedBfs/=1000;
        avgExpandDfs/=1000;
        avgGeneratedDfs/=1000;
        avgExpandAstar/=1000;
        avgGeneratedAstar/=1000;
        avgExpandGreedy/=1000;
        avgGeneratedGreedy/=1000;
        printf("\tAverages with Coord set number %d:\n", i);
        printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
        avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
        avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);


        avg = 0;
        avgFun = 0;
        temp = 0;
        avgTime = 0;
        avgExpandBfs = 0;
        avgGeneratedBfs = 0;
        avgExpandDfs = 0;
        avgGeneratedDfs = 0;
        avgExpandAstar = 0;
        avgGeneratedAstar = 0;
        avgExpandGreedy = 0;
        avgGeneratedGreedy = 0;
    }
    printf("\nStart Frontier Tests\n");
    mazeParam[0] = 0;
    mazeParam[1] = 1;
    mazeParam[2] = 0;
    mazeParam[3] = 1;
    mazeParam[4] = 0;
    mazeParam[5] = 1;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with Random Expansion Order:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    mazeParam[0] = 0;
    mazeParam[1] = 0.1;
    mazeParam[2] = 0;
    mazeParam[3] = 0.1;
    mazeParam[4] = 1;
    mazeParam[5] = 1;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with BFS Expansion Order:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    mazeParam[0] = 0;
    mazeParam[1] = 0.1;
    mazeParam[2] = 0;
    mazeParam[3] = 0.1;
    mazeParam[4] = 0;
    mazeParam[5] = 0.1;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with DFS Expansion Order:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;


    printf("\nStart Bias Tests\n");
    mazeParam[0] = 0;
    mazeParam[1] = 1;
    mazeParam[2] = 0;
    mazeParam[3] = 1;
    mazeParam[4] = 0;
    mazeParam[5] = 1;
    biases[0] = 0;
    biases[1] = 0;
    biases[2] = 1;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with Sharp Turn Bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    biases[0] = 0;
    biases[1] = 1;
    biases[2] = 0;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with Straight Bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    biases[0] = 1;
    biases[1] = 0;
    biases[2] = 1;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with Some Turn Bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    biases[0] = 1;
    biases[1] = 1;
    biases[2] = 1;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with No Turn Bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;


    printf("\nStart Solution Tests\n");
    biases[0] = 1;
    biases[1] = 1;
    biases[2] = 1;
    for(solRank = 0; solRank<=1; solRank+=0.2){
        for(j=0; j<1000; j++){
            overworked = new Maze(length, width);
            overworked->buildMaze(1, coords, mazeParam, biases);
            entex = overworked->solutionNodes(solRank);
            mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
            avg += mcclend;
            avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
            out = bfs(entex);
            avgExpandBfs += out.nodeExpand;
            avgGeneratedBfs += out.nodeGen;
            out = dfs(entex);
            avgExpandDfs += out.nodeExpand;
            avgGeneratedDfs += out.nodeGen;
            out = astar(entex);
            avgExpandAstar += out.nodeExpand;
            avgGeneratedAstar += out.nodeGen;
            out = greedy(entex);
            avgExpandGreedy += out.nodeExpand;
            avgGeneratedGreedy += out.nodeGen;
        }
        avg/=1000;
        avgFun/=1000;
        avgExpandBfs/=1000;
        avgGeneratedBfs/=1000;
        avgExpandDfs/=1000;
        avgGeneratedDfs/=1000;
        avgExpandAstar/=1000;
        avgGeneratedAstar/=1000;
        avgExpandGreedy/=1000;
        avgGeneratedGreedy/=1000;
        printf("\tAverages with Solution Rank Percentile %f:\n", solRank);
        printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
        avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
        avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
        avg = 0;
        avgFun = 0;
        temp = 0;
        avgTime = 0;
        avgExpandBfs = 0;
        avgGeneratedBfs = 0;
        avgExpandDfs = 0;
        avgGeneratedDfs = 0;
        avgExpandAstar = 0;
        avgGeneratedAstar = 0;
        avgExpandGreedy = 0;
        avgGeneratedGreedy = 0;

    }
    solRank = 0;


    printf("\nStart Special Pattern Tests\n");
    coords[1] = std::make_tuple(length-1, width-1);

    mazeParam[0] = 0;
    mazeParam[1] = 0.1;
    mazeParam[2] = 0;
    mazeParam[3] = 0.1;
    mazeParam[4] = 1;
    mazeParam[5] = 1;
    biases[0] = 0;
    biases[1] = 1;
    biases[2] = 0;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(2, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with corner seeding, BFS, and straight bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    mazeParam[0] = 0;
    mazeParam[1] = 0.1;
    mazeParam[2] = 0;
    mazeParam[3] = 0.1;
    mazeParam[4] = 0;
    mazeParam[5] = 0.1;
    biases[0] = 1;
    biases[1] = 0;
    biases[2] = 0;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(2, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with corner seeding, DFS, and left bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    mazeParam[0] = 0;
    mazeParam[1] = 0.1;
    mazeParam[2] = 0;
    mazeParam[3] = 0.1;
    mazeParam[4] = 0;
    mazeParam[5] = 0.1;
    biases[0] = 0;
    biases[1] = 1;
    biases[2] = 0;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(2, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with corner seeding, DFS, and straight bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    coords[0] = coords[2];
    mazeParam[0] = 0;
    mazeParam[1] = 0.1;
    mazeParam[2] = 0;
    mazeParam[3] = 0.1;
    mazeParam[4] = 1;
    mazeParam[5] = 1;
    biases[0] = 0;
    biases[1] = 1;
    biases[2] = 0;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with center seeding, BFS, and straight bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    mazeParam[0] = 0;
    mazeParam[1] = 0.1;
    mazeParam[2] = 0;
    mazeParam[3] = 0.1;
    mazeParam[4] = 1;
    mazeParam[5] = 1;
    biases[0] = 0;
    biases[1] = 0;
    biases[2] = 1;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with center seeding, BFS, and right bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    mazeParam[0] = 0;
    mazeParam[1] = 0.1;
    mazeParam[2] = 0;
    mazeParam[3] = 0.1;
    mazeParam[4] = 0;
    mazeParam[5] = 0.1;
    biases[0] = 0;
    biases[1] = 0;
    biases[2] = 1;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with center seeding, DFS, and right bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

    mazeParam[0] = 0;
    mazeParam[1] = 0.1;
    mazeParam[2] = 0;
    mazeParam[3] = 0.1;
    mazeParam[4] = 0;
    mazeParam[5] = 0.1;
    biases[0] = 0;
    biases[1] = 1;
    biases[2] = 0;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        mcclend = mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        avg += mcclend;
        avgFun += bellotFun(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate, mcclend);
        out = bfs(entex);
        avgExpandBfs += out.nodeExpand;
        avgGeneratedBfs += out.nodeGen;
        out = dfs(entex);
        avgExpandDfs += out.nodeExpand;
        avgGeneratedDfs += out.nodeGen;
        out = astar(entex);
        avgExpandAstar += out.nodeExpand;
        avgGeneratedAstar += out.nodeGen;
        out = greedy(entex);
        avgExpandGreedy += out.nodeExpand;
        avgGeneratedGreedy += out.nodeGen;
    }
    avg/=1000;
    avgFun/=1000;
    avgExpandBfs/=1000;
    avgGeneratedBfs/=1000;
    avgExpandDfs/=1000;
    avgGeneratedDfs/=1000;
    avgExpandAstar/=1000;
    avgGeneratedAstar/=1000;
    avgExpandGreedy/=1000;
    avgGeneratedGreedy/=1000;
    printf("\tAverages with center seeding, DFS, and straight bias:\n");
    printf("\t\tAvg McClendon: %f\n\t\tAvg Fun: %f\n\t\tAvg BFS Expanded: %d\n\t\tAvg BFS Generated: %d\n\t\tAvg DFS Expanded: %d\n\t\tAvg DFS Generated: %d\n\t\tAvg A* Expanded: %d\n\t\tAvg A* Generated: %d\n\t\tAvg Greedy Expanded: %d\n\t\tAvg Greedy Generated: %d\n",
    avg, avgFun, avgExpandBfs, avgGeneratedBfs, avgExpandDfs, avgGeneratedDfs, avgExpandAstar,
    avgGeneratedAstar, avgExpandGreedy, avgGeneratedGreedy);
    avg = 0;
    avgFun = 0;
    temp = 0;
    avgTime = 0;
    avgExpandBfs = 0;
    avgGeneratedBfs = 0;
    avgExpandDfs = 0;
    avgGeneratedDfs = 0;
    avgExpandAstar = 0;
    avgGeneratedAstar = 0;
    avgExpandGreedy = 0;
    avgGeneratedGreedy = 0;

}
