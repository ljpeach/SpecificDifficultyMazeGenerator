#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <forward_list>
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
        current->loc->display = '@';
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
            trailComp*=(length*sum+1);
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
    //printf("%f\n", solComp);
    return log(solComp * trailComp);
}

float bellotFun(Maze m, std::tuple<int, int> startCoord, std::tuple<int, int> endCoord){
    float nswalls = 0;
    //find intersections
    //find extremity walls
    //loop through extremity walls, removing each. If adjacent walls is extremity (and wasn't intersection), add to next round

    return nswalls/mcclendonDiff(m, startCoord, endCoord);
}
