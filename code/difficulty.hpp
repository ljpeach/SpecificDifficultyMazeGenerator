#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <forward_list>
#include <deque>
#include <math.h>
#include <set>
#include "maze.hpp"

struct ExplrNode{
    struct ExplrNode* parent;
    struct ExplrNode* children[3];
    MazeNode* loc;

    ExplrNode(){
        parent = NULL;
        loc = NULL;
        children[0] = NULL;
        children[1] = NULL;
        children[2] = NULL;
    }

    ExplrNode(struct ExplrNode* par, MazeNode* pos){
        parent = par;
        loc = pos;
        children[0] = NULL;
        children[1] = NULL;
        children[2] = NULL;
    }

    Direction parentDir(){
        if(parent == NULL) return None;
        for(Direction dir = North; dir!=None; dir++){
            if(loc->neighbor(dir) != NULL && parent->loc->coordinate == loc->neighbor(dir)->coordinate){
                return dir;
            }
        }
        return None;
    }

};

class WallFrame{
public:
    MazeNode *frameUL, *frameUR, *frameBL, *frameBR;
    WallFrame(){
        frameUL = NULL;
        frameUR = NULL;
        frameBL = NULL;
        frameBR = NULL;
    }
    WallFrame(Maze m){
        reposition(m.corner);
    }
    WallFrame(MazeNode* ul, MazeNode* ur, MazeNode* bl, MazeNode* br){
        frameUL = ul;
        frameUR = ur;
        frameBL = bl;
        frameBR = br;
    }

    //frame navigation
    bool canUp(){
        return frameUL->neighbor(North)!=NULL;
    }
    void up(){
        frameBL = frameUL;
        frameBR = frameUR;
        frameUL = frameUL->neighbor(North);
        frameUR = frameUR->neighbor(North);
    }
    bool canDown(){
        return frameBL->neighbor(South)!=NULL;
    }
    void down(){
        frameUL = frameBL;
        frameUR = frameBR;
        frameBL = frameBL->neighbor(South);
        frameBR = frameBR->neighbor(South);

    }
    bool canLeft(){
        return frameUL->neighbor(West)!=NULL;
    }
    void left(){
        frameUR = frameUL;
        frameBR = frameBL;
        frameUL = frameUL->neighbor(West);
        frameBL = frameBL->neighbor(West);
    }
    bool canRight(){
        return frameUR->neighbor(East)!=NULL;
    }
    void right(){
        frameUL = frameUR;
        frameBL = frameBR;
        frameUR = frameUR->neighbor(East);
        frameBR = frameBR->neighbor(East);

    }
    void reposition(MazeNode* ul){
        frameUL = ul;
        frameUR = frameUL->neighbor(East);
        frameBL = frameUL->neighbor(South);
        frameBR = frameUR->neighbor(South);
    }
    bool canDir(Direction dir){
        switch(dir){
            case North:
                return canUp();
            case East:
                return canRight();
            case South:
                return canDown();
            case West:
                return canLeft();
            default:
                std::cout << "Invalid Direction" <<std::endl;
                return false;
        }
    }
    void direct(Direction dir){
        switch(dir){
            case North:
                up();
                return;
            case East:
                right();
                return;
            case South:
                down();
                return;
            case West:
                left();
                return;
            default:
                std::cout << "Invalid Direction" <<std::endl;
        }
    }


    //wall details
    int numWalls(){
        return frameUL->wall(East)+frameUL->wall(South)+frameBR->wall(West)+frameBR->wall(North);
    }
    bool isExtWall(){
        return numWalls()==1;
    }
    bool isIntWall(){
        return numWalls()>2;
    }
    Direction extDir(){
        if(!isExtWall()) return None;
        if(!frameUL->hall(East)){return North;}
        else if(!frameUL->hall(South)){return West;}
        else if(!frameBR->hall(West)){return South;}
        else{return East;}
    }
    Direction extDir(std::set<std::tuple<MazeNode*, MazeNode*>> removedWalls){
        if(isIntWall())return None;
        Direction badWall = None;
        if(!frameUL->hall(East) && removedWalls.count(std::make_tuple(frameUL, frameUR))!=0)
        {badWall = North;}
        else if(!frameUL->hall(South) && removedWalls.count(std::make_tuple(frameUL, frameBL))!=0)
        {badWall = West;}
        else if(!frameBR->hall(West) && removedWalls.count(std::make_tuple(frameBL, frameBR))!=0)
        {badWall = South;}
        else{badWall = East;}

        if(badWall!= North && !frameUL->hall(East)){return North;}
        else if(badWall!= West && !frameUL->hall(South)){return West;}
        else if(badWall!= South && !frameBR->hall(West)){return South;}
        else{return East;}
    }
    std::tuple<MazeNode*, MazeNode*> wallPair(Direction dir){
        switch(dir){
            case North:
                return std::make_tuple(frameUL, frameUR);
            case East:
                return std::make_tuple(frameUR, frameBR);
            case South:
                return std::make_tuple(frameBL, frameBR);
            case West:
                return std::make_tuple(frameUL, frameBL);
            default:
                std::cout << "Invalid Direction" <<std::endl;
                return std::make_tuple(frameUL, frameUL);
        }
    }
};

float mcclendonDiff(Maze maze, std::tuple<int, int> startCoord, std::tuple<int, int> endCoord){
    //Determine solution path
    std::forward_list<struct ExplrNode*> travLst;
    struct ExplrNode* goal,* gate;
    struct ExplrNode* current,* hallSlide;
    Direction dir, entrance;
    int child, run, length;
    float sum, solComp, trailComp;
    std::set<std::tuple<int,int>> solNodes;

    goal = new ExplrNode(NULL, maze.getNode(endCoord));
    travLst.push_front(goal);
    while(!travLst.empty()){
        current = travLst.front();
        //printf("current coord: %p\n", current->parent);
        travLst.pop_front();
        child = 0;
        if(current->loc->coordinate == startCoord){
            gate = current;
        }
        for(dir = North; dir!=None; dir++){
            if(current->loc->hall(dir) && (current->parent == NULL ||
            current->loc->neighbor(dir)->coordinate!=current->parent->loc->coordinate)){
                travLst.push_front(new ExplrNode(current, current->loc->neighbor(dir)));
                current->children[child] = travLst.front();
                child++;
            }
        }
    }

    //walk through solution path, adding intersection points as start points for new trails. Also collect complexity data
    current = gate;
    entrance = None;
    run = 0;
    length = -1;
    sum = 0;
    while(current!=NULL){
        //current->loc->display = '@';
        solNodes.insert(current->loc->coordinate);
        if(current->loc->numHalls()>2){//intersection
            travLst.push_front(current);
        }
        if(current->loc->numHalls()==1 && current->parent!=NULL){//not last node, also dead end
            run++;
        }
        else if(current->loc->numHalls()==2 &&
        ((current->loc->hall(North)&&current->loc->hall(South))
        ||(current->loc->hall(East)&&current->loc->hall(West)))){//hall, but straight
            run++;
        }
        else if(entrance == current->parentDir()){//intersection, but straight passage
            run++;
        }
        else{//turn of any sort. includes final position in trail.
            sum+=(1.0/(2*run));
            run = 1;
        }
        length++;
        entrance = current->parentDir();
        current = current->parent;
    }
    solComp = length * sum;
    //go through trail by trail, adding new intersections as starting points. Accumulate complexity data for each, start summing
    trailComp = 1;
    while(!travLst.empty()){
        current = travLst.front();
        travLst.pop_front();
        for(child = 0; child<current->loc->numHalls()-1; child++){
            if(solNodes.count(current->children[child]->loc->coordinate)>0) continue;
            run = length = 1;
            sum = 0;
            hallSlide = current->children[child];
            while(1 < hallSlide->loc->numHalls() && hallSlide->loc->numHalls() < 3){
                if(hallSlide->children[0]->parentDir()==hallSlide->parentDir()){
                    run++;
                }
                else{
                    sum+=(1.0/(2*run));
                    run = 1;
                }
                hallSlide = hallSlide->children[0];
            }
            sum+=(1.0/(2*run));
            if(hallSlide->loc->numHalls() >= 3){
                travLst.push_front(hallSlide);
            }
            trailComp+=log(length*sum+1);
        }
    }
    //delete allocated memory for tree
    travLst.push_front(goal);
    while(!travLst.empty()){
        current = travLst.front();
        travLst.pop_front();
        for(child = 0; child<3 && current->children[child]!=NULL; child++){
            travLst.push_front(current->children[child]);
        }
        delete current;
    }
    return log(solComp) + trailComp;
}

float bellotFun(Maze maze, std::tuple<int, int> startCoord, std::tuple<int, int> endCoord){
    if(maze.length < 2 && maze.width < 2){
        return 0;
    }
    int nswalls = 0;
    WallFrame frame = WallFrame(maze);
    bool typewriter = true;
    std::set<std::tuple<MazeNode*, MazeNode*>> deletedWalls;
    std::deque<MazeNode*> currentWalls;
    int i;
    Direction dir;
    //find initial extremity walls
    while(true){
        if(typewriter){
            while(true){
                if(frame.isExtWall()){
                    currentWalls.push_back(frame.frameUL);
                }
                if(frame.canRight()) frame.right();
                else break;
            }
        }
        else{
            while(true){
                if(frame.isExtWall()){
                    currentWalls.push_back(frame.frameUL);
                }
                if(frame.canLeft()) frame.left();
                else break;
            }
        }
        typewriter = !typewriter;
        if(frame.canDown()) frame.down();
        else break;
    }
    //loop through extremity walls, removing each. If adjacent walls is extremity (and wasn't intersection), add to next round
    while(!currentWalls.empty()){
        nswalls++;
        frame.reposition(currentWalls.front());
        currentWalls.pop_front();
        if(frame.isExtWall()){
            dir = frame.extDir();
        }
        else{
            dir = frame.extDir(deletedWalls);
        }
        deletedWalls.insert(frame.wallPair(dir));
        if(frame.canDir(dir)){
            frame.direct(dir);
            if(!frame.isIntWall()){
                currentWalls.push_back(frame.frameUL);
            }
        }
    }
    printf("NS walls: %d\n", nswalls);
    return nswalls/mcclendonDiff(maze, startCoord, endCoord);
}
