#pragma once
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <forward_list>
#include <deque>
#include <math.h>
#include <set>
#include <ctime>
#include "maze.hpp"
#include "difficulty.hpp"

struct DiffOutput{
    int nodeGen;
    int nodeExpand;
    double duration;

    DiffOutput(){
        nodeGen=0;
        nodeExpand=0;
        duration = 0;
    }
    DiffOutput(int nodeG, int nodeE, double dur){
        nodeGen = nodeG;
        nodeExpand = nodeE;
        duration = dur;
    }
};

void deleteAll(ExplrNode* current){
    if(current==NULL){
        return;
    }
    ExplrNode* toDelete[4];
    int count=0, i;
    if(current->parent!=NULL){
        toDelete[count++] = current->parent;
        for(i=0; i<3; i++){
            if(current->parent->children[i]==NULL) continue;
            if(current->parent->children[i]->loc->coordinate == current->loc->coordinate){
                current->parent->children[i] = NULL;
            }
        }
        current->parent = NULL;
    }
    for(i=0; i<3; i++){
        if(current->children[i]==NULL){
            continue;
        }
        toDelete[count++] = current->children[i];
        current->children[i]->parent = NULL;
        current->children[i] = NULL;
    }
    current->loc = NULL;
    delete current;
    for(i=0; i<count; i++){
        deleteAll(toDelete[i]);
    }
}

int manhattan(MazeNode* left, MazeNode* right){
    return abs(std::get<0>(left->coordinate)-std::get<0>(right->coordinate))+abs(std::get<1>(left->coordinate)-std::get<1>(right->coordinate));
}

static bool tupleComp(std::tuple<int, int, ExplrNode*> a, std::tuple<int, int, ExplrNode*> b){
    return std::get<0>(a)+std::get<1>(a) > std::get<0>(b)+std::get<1>(b);
}

struct DiffOutput astar(std::tuple<MazeNode*, MazeNode*> entex){
    MazeNode* start = std::get<0>(entex);
    MazeNode* end = std::get<1>(entex);
    int nodeGen = 1;
    int nodeExpand = 0;
    int newNodes, i, currentDepth;
    clock_t timer = clock();
    std::vector<std::tuple<int, int, ExplrNode*>> astarHeap;
    ExplrNode* current = new ExplrNode(NULL, start);
    astarHeap.push_back(std::make_tuple(0, manhattan(current->loc, end), current));
    while (!astarHeap.empty()) {
        current = std::get<2>(astarHeap.front());
        currentDepth = std::get<0>(astarHeap.front());
        std::pop_heap(astarHeap.begin(), astarHeap.end(), tupleComp);
        astarHeap.pop_back();
        if(current->loc->coordinate==end->coordinate) break;
        newNodes = current->expand();
        nodeExpand++;
        nodeGen+=newNodes;
        for(i=0; i<newNodes; i++){
            astarHeap.push_back(std::make_tuple(currentDepth+1, manhattan(current->children[i]->loc, end), current->children[i]));
            std::push_heap(astarHeap.begin(), astarHeap.end(), tupleComp);
        }
    }
    struct DiffOutput result = DiffOutput(nodeGen, nodeExpand, double(clock()-timer));
    deleteAll(current);
    return result;
}

struct DiffOutput greedy(std::tuple<MazeNode*, MazeNode*> entex){
    MazeNode* start = std::get<0>(entex);
    MazeNode* end = std::get<1>(entex);
    int nodeGen = 1;
    int nodeExpand = 0;
    int newNodes, i;
    clock_t timer = clock();
    std::vector<std::tuple<int, int, ExplrNode*>> astarHeap;
    ExplrNode* current = new ExplrNode(NULL, start);
    astarHeap.push_back(std::make_tuple(0, manhattan(current->loc, end), current));
    while (!astarHeap.empty()) {
        current = std::get<2>(astarHeap.front());
        std::pop_heap(astarHeap.begin(), astarHeap.end(), tupleComp);
        astarHeap.pop_back();
        if(current->loc->coordinate==end->coordinate) break;
        newNodes = current->expand();
        nodeExpand++;
        nodeGen+=newNodes;
        for(i=0; i<newNodes; i++){
            astarHeap.push_back(std::make_tuple(0, manhattan(current->children[i]->loc, end), current->children[i]));
            std::push_heap(astarHeap.begin(), astarHeap.end(), tupleComp);
        }
    }
    struct DiffOutput result = DiffOutput(nodeGen, nodeExpand, double(clock()-timer));
    deleteAll(current);
    return result;
}

struct DiffOutput dfs(std::tuple<MazeNode*, MazeNode*> entex){
    MazeNode* start = std::get<0>(entex);
    MazeNode* end = std::get<1>(entex);
    int nodeGen = 1;
    int nodeExpand = 0;
    int newNodes, i;
    clock_t timer = clock();
    std::deque<ExplrNode *> frontier;
    ExplrNode* current = new ExplrNode(NULL, start);
    frontier.push_back(current);
    while(!frontier.empty()){
        current = frontier.back();
        frontier.pop_back();
        if(current->loc->coordinate==end->coordinate) break;
        newNodes = current->expand();
        nodeExpand++;
        nodeGen+=newNodes;
        for(i=0; i<newNodes; i++){
            frontier.push_back(current->children[i]);
        }
    }
    struct DiffOutput result = DiffOutput(nodeGen, nodeExpand, double(clock()-timer));
    deleteAll(current);
    return result;
}

struct DiffOutput bfs(std::tuple<MazeNode*, MazeNode*> entex){
    MazeNode* start = std::get<0>(entex);
    MazeNode* end = std::get<1>(entex);
    int nodeGen = 1;
    int nodeExpand = 0;
    int newNodes, i;
    clock_t timer = clock();
    std::deque<ExplrNode *> frontier;
    ExplrNode* current = new ExplrNode(NULL, start);
    frontier.push_back(current);
    while(!frontier.empty()){
        current = frontier.front();
        frontier.pop_front();
        if(current->loc->coordinate==end->coordinate) break;
        newNodes = current->expand();
        nodeExpand++;
        nodeGen+=newNodes;
        for(i=0; i<newNodes; i++){
            frontier.push_back(current->children[i]);
        }
    }
    struct DiffOutput result = DiffOutput(nodeGen, nodeExpand, double(clock()-timer));
    deleteAll(current);
    return result;
}
