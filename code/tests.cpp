#include <iostream>
#include <ctime>
#include "maze.hpp"
#include "difficulty.hpp"

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
    float temp = 0;

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
            avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        }
        avg/=1000;
        printf("\tavgW/Coord%d: %f\n", i, avg);
        avg = 0;
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
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgRandom: %f\n", avg);
    avg = 0;

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
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavg BFS: %f\n", avg);
    avg = 0;

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
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgDFS: %f\n", avg);
    avg = 0;


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
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgSharpTurnBias: %f\n", avg);
    avg = 0;

    biases[0] = 0;
    biases[1] = 1;
    biases[2] = 0;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgStraightBias: %f\n", avg);
    avg = 0;

    biases[0] = 1;
    biases[1] = 0;
    biases[2] = 1;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgTurnBias: %f\n", avg);
    avg = 0;

    biases[0] = 1;
    biases[1] = 1;
    biases[2] = 1;
    for(j=0; j<1000; j++){
        overworked = new Maze(length, width);
        overworked->buildMaze(1, coords, mazeParam, biases);
        entex = overworked->solutionNodes(solRank);
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgNoBias: %f\n", avg);
    avg = 0;


    printf("\nStart Solution Tests\n");
    biases[0] = 1;
    biases[1] = 1;
    biases[2] = 1;
    for(solRank = 0; solRank<=1; solRank+=0.2){
        for(j=0; j<1000; j++){
            overworked = new Maze(length, width);
            overworked->buildMaze(1, coords, mazeParam, biases);
            entex = overworked->solutionNodes(solRank);
            avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
        }
        avg/=1000;
        printf("\tavgSolRankPercent%f: %f\n", solRank, avg);
        avg = 0;

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
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgCornerSeed+BFS+StrtBias: %f\n", avg);
    avg = 0;

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
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgCornerSeed+DFS+leftBias: %f\n", avg);
    avg = 0;

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
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgCornerSeed+DFS+strtBias: %f\n", avg);
    avg = 0;

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
        avg+=  mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgCenterSeed+BFS+strtBias: %f\n", avg);
    avg = 0;

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
        avg+= mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgCenterSeed+BFS+rightBias: %f\n", avg);
    avg = 0;

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
        avg+= mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgCenterSeed+DFS+rightBias: %f\n", avg);
    avg = 0;

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
        avg+= mcclendonDiff(overworked, std::get<0>(entex)->coordinate, std::get<1>(entex)->coordinate);
    }
    avg/=1000;
    printf("\tavgCenterSeed+DFS+strtBias: %f\n", avg);
    avg = 0;

}
