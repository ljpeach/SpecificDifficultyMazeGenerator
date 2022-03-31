#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <ctime>
#include <math.h>
#include <cmath>
#include <vector>
#include <set>
#include <forward_list>
#include <algorithm>

//Shorthand for directions
enum Direction {North=0, East, South, West, None};
Direction operator ++(Direction& dir, int){
    Direction old = dir;
    if(4 < dir+1) dir = North;
    else dir = static_cast<Direction>(dir+1);
    return old;
}
Direction opp(Direction dir){
    switch(dir){
        case North:
            return South;
        case East:
            return West;
        case South:
            return North;
        case West:
            return East;
        default:
            return None;
    }
}
Direction left(Direction dir){
    switch(dir){
        case North:
            return West;
        case East:
            return North;
        case South:
            return East;
        case West:
            return South;
        default:
            return None;
    }
}
Direction right(Direction dir){
    switch(dir){
        case North:
            return East;
        case East:
            return South;
        case South:
            return West;
        case West:
            return North;
        default:
            return None;
    }
}

class MazeNode{
public:
    //Pointers to neighboring intersection nodes
    //Used for maintaining the full maze's connections
    //Can be used instead of connection vars if using 2D array in full maze
    MazeNode* upNode = NULL;
    MazeNode* rightNode = NULL;
    MazeNode* downNode = NULL;
    MazeNode* leftNode = NULL;

    //determines whether there is a hall to neighboring nodes
    //Used for determining pathing within the maze
    bool upHall = false;
    bool rightHall = false;
    bool downHall = false;
    bool leftHall = false;

    //management variables
    int set;//set 0 is "not in maze," or the empty set.
    bool visited;

    std::tuple<int,int> coordinate;

    //direction management
    Direction entry;

    //display specs
    char display = ' ';

    MazeNode(int st, std::tuple<int,int> coord){
        set = st;
        visited = false;
        coordinate = coord;
    }

    MazeNode* neighbor(Direction dir){
        switch(dir){
            case North:
                return upNode;
            case East:
                return rightNode;
            case South:
                return downNode;
            case West:
                return leftNode;
            default:
                std::cout << "Invalid Direction" <<std::endl;
        }
    }

    void stitchDirection(Direction dir, MazeNode* neighbor){
        switch(dir){
            case North:
                upNode = neighbor;
                if(upNode == NULL) return;
                upNode->downNode = this;
                // printf("stitchUP:%d,%d\n", upNode?std::get<0>(upNode->coordinate):-1,upNode?std::get<1>(upNode->coordinate):-1);
                // printf("THEN:%d,%d\n", upNode->downNode?std::get<0>(upNode->downNode->coordinate):-1,upNode->downNode?std::get<1>(upNode->downNode->coordinate):-1);
                break;
            case East:
                rightNode = neighbor;
                if(rightNode == NULL) return;
                rightNode->leftNode = this;
                break;
            case South:
                downNode = neighbor;
                if(downNode == NULL) return;
                downNode->upNode = this;
                break;
            case West:
                leftNode = neighbor;
                if(leftNode == NULL) return;
                leftNode->rightNode = this;
                break;
            default:
                std::cout << "Invalid Direction" <<std::endl;
        }
    }

    void bridgeDirection(Direction dir) {
        switch(dir){
            case North:
                if(upNode == NULL) return;
                upNode->downHall = true;
                upNode->set = set;
                upNode->entry = South;
                upHall = true;
                break;
            case East:
                if(rightNode == NULL) return;
                rightNode->leftHall = true;
                rightNode->set = set;
                rightNode->entry = West;
                rightHall = true;
                break;
            case South:
                if(downNode == NULL) return;
                downNode->upHall = true;
                downNode->set = set;
                downNode->entry = North;
                downHall = true;
                break;
            case West:
                if(leftNode == NULL) return;
                leftNode->rightHall = true;
                leftNode->set = set;
                leftNode->entry = East;
                leftHall = true;
                break;
            default:
                std::cout << "Invalid Direction" <<std::endl;
        }
    }

    bool wall(Direction dir){
        switch(dir){
            case North:
                return !upHall&&upNode!=NULL;
            case East:
                return !rightHall&&rightNode!=NULL;
            case South:
                return !downHall&&downNode!=NULL;
            case West:
                return !leftHall&&leftNode!=NULL;
            default:
                std::cout << "Invalid Direction" <<std::endl;
        }
    }

    bool hall(Direction dir){
        switch(dir){
            case North:
                return upHall;
            case East:
                return rightHall;
            case South:
                return downHall;
            case West:
                return leftHall;
            default:
                std::cout << "Invalid Direction" <<std::endl;
        }
    }

    int numHalls(){
        return upHall+rightHall+downHall+leftHall;
    }

    int numValidWalls(int targetSet){
        int wallCount = 0;
        for(Direction compass=North; compass<None; compass++){
            if(wall(compass) && neighbor(compass)!=NULL && neighbor(compass)->set==targetSet){
                wallCount++;
            }
        }
        return wallCount;
    }

    int numNeighborSets(std::set<int>* ignoreSets){
        Direction dir = North;
        int count = 0;
        while(dir!=None){
            if(wall(dir) && ignoreSets->find(neighbor(dir)->set) == ignoreSets->end() && neighbor(dir)->set!=set){
                count++;
            }
            dir++;
        }
        return count;
    }

    Direction randomHall(){
        if(numHalls() == 0) return None;
        int pos = std::rand()%numHalls();
        for(Direction compass=North; compass<None; compass++){
            if(!wall(compass)&&!pos--){return compass;}
        }
        return None;
    }
    Direction randomWall(int targetSet){
        if(numValidWalls(targetSet) == 0) return None;
        int pos = std::rand()%numValidWalls(targetSet);
        for(Direction compass=North; compass<None; compass++){
            if(wall(compass) && neighbor(compass)!=NULL && neighbor(compass)->set==targetSet&&!pos--)
                return compass;
        }
        return None;
    }

    Direction biasedHall(float leftBias, float straightBias, float rightBias){
        if(leftBias == -1) return randomHall();
        return North;
    }
    Direction biasedWall(float leftBias, float straightBias, float rightBias, int targetSet){
        if(leftBias == -1 || (leftBias==straightBias && straightBias==rightBias && leftBias == rightBias)) return randomWall(targetSet);
        bool relNorthWall = wall(opp(entry)) && neighbor(opp(entry))->set==targetSet;
        bool relEastWall = wall(left(entry)) && neighbor(left(entry))->set==targetSet;
        bool relWestWall = wall(right(entry)) && neighbor(right(entry))->set==targetSet;
        if(relNorthWall+relEastWall+relWestWall == 0){
            return None;
        }
        //Of valid walls, remaining biases are at 0.
        //Must increase by 1 so that a choice is possible despite weight of 0.
        //essentially will only chose when no other choice.
        if(relNorthWall*straightBias+relEastWall*leftBias+relWestWall*rightBias == 0){
            straightBias++;
            leftBias++;
            rightBias++;
        }
        int randWeighted = std::rand()%(static_cast<int>(relWestWall*rightBias*100
            + relNorthWall*straightBias*100 + relEastWall*leftBias*100));
        if(relNorthWall && randWeighted <= (int)(straightBias*100)) return opp(entry);
        else if (relWestWall && randWeighted - relNorthWall*(int)(straightBias*100) <= (int)(rightBias*100)) return right(entry);
        else if (relEastWall && randWeighted - relNorthWall*(int)(straightBias*100) - relWestWall*(int)(rightBias*100) <= (int)(leftBias*100)) return left(entry);
        return None;
    }
};

class Maze{
public:
    class Frontier{
    public:
        class Node{
        public:
            MazeNode* node;
            Node* next;
            Node* prev;
            Node(MazeNode* n, Node* previous, Node* nxt){
                node = n;
                next = nxt;
                prev = previous;
            }
        };
        struct FrontierPointer{
            Node* location;
            int index;
            float goalRatio;

            FrontierPointer(){
                location = NULL;
            }

            FrontierPointer(Node* loc, int ind, float ratio){
                location = loc;
                index = ind;
                goalRatio = ratio;
            }

            void update(Node* newLoc, int newInd){
                location = newLoc;
                index = newInd;
            }
        };

        Node* head;
        Node* tail;
        struct FrontierPointer insertOld;
        struct FrontierPointer insertNew;
        struct FrontierPointer popPoint;
        int len;
        //ins and pop are mid point bewtween ranges. (n/2 access whithin range)
        //Must be between 0 and 1 inclusive.
        Frontier(int n, float inOld, float inNew, float popLoc){
            head = NULL;
            tail = NULL;
            insertOld = FrontierPointer(NULL, 0, inOld);
            insertNew = FrontierPointer(NULL, 0, inNew);
            popPoint = FrontierPointer(NULL, 0, popLoc);
            len = 0;
        }
        void add(MazeNode* insert, int position){
            if(position<0 || position>size()){
                return;
            }
            if(size()<1){
                head = new Node(insert, NULL, NULL);
                tail = head;
                insertOld.update(head, 0);
                insertNew.update(head, 0);
                popPoint.update(head, 0);
                len++;
                return;
            }
            if(position == size()){
                tail->next = new Node(insert, tail, NULL);
                tail = tail->next;
            }
            else if(position == 0){
                head->prev = new Node(insert, NULL, head);
                head = head->prev;
            }
            else{
                Node* pos = fastGet(position);
                Node* add = new Node(insert, pos->prev, pos);
                pos->prev = add;
                add->prev->next = add;
            }
            addUpdate(&insertOld, position);
            addUpdate(&insertNew, position);
            addUpdate(&popPoint, position);
            len++;
        }
        MazeNode* pop(int position){
            if(size()==0 || position>=size() || position<0){
                return NULL;
            }
            MazeNode* out;
            if(size() == 1){
                out = head->node;
                delete head;
                head = NULL;
                tail = NULL;
                insertOld.update(NULL, -1);
                insertNew.update(NULL, -1);
                popPoint.update(NULL, -1);
                len--;
                return out;
            }
            if(position == 0){
                out = head->node;
                head = head->next;
                remUpdate(&insertOld, position);
                remUpdate(&insertNew, position);
                remUpdate(&popPoint, position);
                delete head->prev;
                head->prev = NULL;
            }
            else if(position == size()-1){
                out = tail->node;
                tail = tail->prev;
                remUpdate(&insertOld, position);
                remUpdate(&insertNew, position);
                remUpdate(&popPoint, position);
                delete tail->next;
                tail->next = NULL;
            }
            else{
                Node* pos = fastGet(position);
                out = pos->node;
                pos->prev->next = pos->next;
                pos->next->prev = pos->prev;
                remUpdate(&insertOld, position);
                remUpdate(&insertNew, position);
                remUpdate(&popPoint, position);
                delete pos;
            }
            len--;
            return out;
        }
        bool empty(){
            return size()<=0;
        }
        int size(){
            return len;
        }
        Node* fastGet(int position){
            Node* slider;
            int count;
            int origin;
            int oldDist = distance(insertOld.index, position);
            int newDist = distance(insertNew.index, position);
            if(position <= oldDist && position <= newDist && position <= (size()-position-1)){
                slider = head;
                count = position;
                origin = 0;
            }
            else if(oldDist <= position && oldDist <= newDist && oldDist <= size()-position-1){
                slider = insertOld.location;
                count = oldDist;
                origin = insertOld.index;
            }
            else if(newDist <= position && newDist <= oldDist && newDist <= size()-position-1){
                slider = insertNew.location;
                count = newDist;
                origin = insertOld.index;
            }
            else{
                slider = tail;
                count = size()-position-1;
                origin = size()-1;
            }
            for(int i = 0; i<count; i++){
                if(origin < position) slider = slider->next;
                else slider = slider->prev;
            }
            return slider;
        }
        int distance(int a, int b){
            return std::abs(a-b);
        }
        void addUpdate(struct FrontierPointer* point, int position){
            int goal = ratioToIndex(point->goalRatio, size());
            if(point->index >= position){
                if(goal==point->index) point->location = point->location->prev;
                else point->index++;
            }
            else if(goal!=point->index){
                point->location = point->location->next;
                point->index++;
            }
        }

        void remUpdate(struct FrontierPointer* point, int position){
            int goal = ratioToIndex(point->goalRatio, size()-2);
            if(point->index==position){
                if(goal == position) point->location = point->location->next;
                else {
                    point->location = point->location->prev;
                    point->index--;
                }
            }
            else if(point->index>position){
                if(goal == point->index) point->location = point->location->next;
                else point->index--;
            }
            else if(goal!=point->index){
                point->location = point->location->prev;
                point->index--;
            }
        }
        int ratioToIndex(float ratio, int size){
            return static_cast<int>(floor(size*ratio));
        }
    };

    //Board
    MazeNode* corner = NULL;
    int length;//Vertical
    int width;//Horizontal

    /*Traversal*/
    float dirBias; //below 1/3 is left, 1/3 - 2/3 is straight, 2/3-3/3 is right
    //interstection ratios
    float ratio0, ratioU, ratioR, ratioD, ratioL, ratioUR, ratioUD, ratioUL,
    ratioRD, ratioRL, ratioDR, ratioURD, ratioURL, ratioUDL, ratioRDL, ratio4;

    /*Solution constraints*/
    //int solLenRank;//0 is default solution tiles. 1 is longest solution, 2 is 2nd, etc.

    Maze(int len, int wid){//,int argc, char* argv){
        length = len;
        width = wid;
        corner = new MazeNode(0,std::make_tuple(0,0));
        MazeNode* row = corner;
        MazeNode* up = NULL;
        MazeNode* left = corner;
        MazeNode* current;
        for(int i = 0; i<length; i++){
            if(i!=0){
                row = new MazeNode(0,std::make_tuple(i,0));
                row->stitchDirection(North, up);
                left = row;
                if(up) up = up->neighbor(East);
            }
            for(int j = 1; j<width; j++){
                current = new MazeNode(0,std::make_tuple(i,j));
                current->stitchDirection(West, left);
                current->stitchDirection(North, up);
                left = current;
                if(up) up = up->neighbor(East);
            }
            left = NULL;
            up = row;
        }
    }

    char* toString(){
        //9 cells for each node, plus height for new lines
        char* fullBoard = new char[length*width*9+3*length+1];
        MazeNode* row = corner;
        MazeNode* current;
        std::tuple<int,int> coord;
        int nodeBlock;
        int j = 0;
        int i;
        while(row){
            for(i = 0; i<2; i++){//2; i++){
                current = row;
                while(current){
                    if(i==0){
                        fullBoard[j] = '#';
                        fullBoard[j+1] = current->upHall?' ':'#';
                        //fullBoard[j+2] = '#';
                    }
                    else if(i==1){
                        fullBoard[j] = current->leftHall?' ':'#';
                        fullBoard[j+1] = current->display;
                        //fullBoard[j+2] = current->rightHall?' ':'#';
                    }
                    // else if(i==2){
                    //     fullBoard[j] = '#';
                    //     fullBoard[j+1] = current->downHall?' ':'#';
                    //     fullBoard[j+2] = '#';
                    // }
                    j+=2;//3;
                    current = current->neighbor(East);
                }
                fullBoard[j] = '#';
                fullBoard[j+1] = '\n';
                j+=2;
            }
            row = row->neighbor(South);
        }
        for(i = 0; i<width*2+1; i++){
            fullBoard[j] = '#';
            j++;
        }
        fullBoard[j] = '\n';
        fullBoard[j+1] = '\0';
        return fullBoard;
    }
    //if multiple starting coords, does not join sets.
    void buildMaze(int starts, std::tuple<int,int>* startCells, float* fparams, float* biases){
        float pushLoOld = fparams[0];
        float pushHiOld = fparams[1];
        float pushLoNew = fparams[2];
        float pushHiNew = fparams[3];
        float popLo = fparams[4];
        float popHi = fparams[5];
        Direction directSelect[] = {North, East, South, West};
        Frontier* field = new Frontier(length*width, (pushHiOld+pushLoOld)/2, (pushHiNew+pushLoNew)/2, (popHi+popLo)/2);
        MazeNode* current;
        Direction wall = None;
        MazeNode* setStarts[starts];
        for(int i = 0; i<starts; i++){
            current = getNode(startCells[i]);
            setStarts[i]  = current;
            current->set = i+1;//0 is default "not in maze" state
            current->entry = directSelect[std::rand()%4];
            insertFrontier(field, current, pushLoNew, pushHiNew);
        }
        while(!field->empty()){
            current = popFrontier(field, popLo, popHi);
            wall = current->biasedWall(biases[0], biases[1], biases[2], 0);
            if(wall==None){continue;}
            current->bridgeDirection(wall);
            if(current->numValidWalls(0)){
                insertFrontier(field, current, pushLoOld, pushHiOld);
            }
            if(current->neighbor(wall)->numValidWalls(0)){
                insertFrontier(field, current->neighbor(wall), pushLoNew, pushHiNew);
            }
        }
        joinSets(starts, setStarts, biases);
    }

    MazeNode* getNode(std::tuple<int,int> coord){
        int row = std::get<0>(coord), col = std::get<1>(coord);
        if(row >= length || col >= width || row < 0 || col < 0){
            std::cout << "Invalid Coord" <<std::endl;
            return NULL;
        }
        MazeNode* pos = corner;
        int i;
        for(i = 0; i<row; i++){
            pos = pos->neighbor(South);
        }
        for(i = 0; i<col; i++){
            pos = pos->neighbor(East);
        }
        return pos;
    }

    void insertFrontier(Frontier* f, MazeNode* node, float inLow, float inHi){
        int low = ratioToIndex(inLow, f->size());
        int hi = ratioToIndex(inHi, f->size());
        f->add(node, ((std::rand()%(hi-low+1))+low));
    }

    MazeNode* popFrontier(Frontier* f, float outLow, float outHi){
        int low = ratioToIndex(outLow, f->size()-1);
        int hi = ratioToIndex(outHi, f->size()-1);
        MazeNode* placeHold = f->pop(((std::rand()%(hi-low+1))+low));
        return placeHold;
    }

    std::tuple<MazeNode*, MazeNode*> solutionNodes(int solRank){
        if(solRank==0){
            return std::make_tuple(corner, getNode(std::make_tuple(length-1,width-1)));
        }
        char digits[] = {'0','1','2','3','4','5','6','7','8','9'};
        int i,j;
        int partharray[length*width];
        for(i=0; i<length*width; i++) partharray[i] = -1;
        std::vector<std::tuple<int,std::tuple<MazeNode*,MazeNode*>>> solutionHeap;
        struct solvNode* edgeNodes[2*(length-1)+2*(width-1)];
        int edges = 0;
        int pathLen;
        //built the tree, insert perimeter nodes into the
        struct solvNode* root, *current, *addition, *current2;
        root = new solvNode(0, NULL, corner);
        edgeNodes[edges] = root;
        //root->loc->display = digits[root->depth];
        edges++;
        current = root;
        std::tuple<int, int> addCoord;
        Direction dir;
        std::forward_list<solvNode*> treeTrav;
        //Build tree structure
        treeTrav.push_front(current);
        while(!treeTrav.empty()){
            current = treeTrav.front();
            treeTrav.pop_front();
            for(dir = North; dir!=None; dir++){
                if(current->loc->hall(dir) && (current->parent == NULL ||
                    current->loc->neighbor(dir)->coordinate!=current->parent->loc->coordinate))  {
                    addition = new solvNode(current->depth+1, current, current->loc->neighbor(dir));
                    //addition->loc->display = digits[current->depth+1];
                    treeTrav.push_front(addition);
                    addCoord = addition->loc->coordinate;
                    if(std::get<0>(addCoord) == 0 || std::get<0>(addCoord) == length-1 ||
                       std::get<1>(addCoord) == 0 || std::get<1>(addCoord) == width-1){
                           edgeNodes[edges] = addition;
                           edges++;
                       }
                }
            }
        }
        //Calculate distance between all pairs of perimeter nodes
        for(i = 0; i<(2*(length-1)+2*(width-1)-1); i++){
            for(j=i+1; j<(2*(length-1)+2*(width-1)); j++){
                current = edgeNodes[i];
                current2 = edgeNodes[j];
                pathLen = 0;
                while(current->loc->coordinate!=current2->loc->coordinate){
                    if(current->depth == 0 || current2->depth == 0){
                        pathLen += current->depth + current2->depth;
                        break;
                    }
                    else if(current->depth > current2->depth){
                        current = current->parent;
                        pathLen++;
                    }
                    else if(current->depth < current2->depth){
                        current2 = current2->parent;
                        pathLen++;
                    }
                    else{
                        current = current->parent;
                        current2 = current2->parent;
                        pathLen+=2;
                    }
                }
                solutionHeap.push_back(std::make_tuple(pathLen, std::make_tuple(edgeNodes[i]->loc, edgeNodes[j]->loc)));
                std::push_heap(solutionHeap.begin(), solutionHeap.end(), tupleComp);
            }
        }

        //Select ranked item
        std::tuple<MazeNode*, MazeNode*> ans;
        for(i=0; i<solRank; i++){
            ans = std::get<1>(solutionHeap.front());
            std::pop_heap(solutionHeap.begin(), solutionHeap.end(), tupleComp);
            solutionHeap.pop_back();
        }
        return ans;
    }

    void joinSets(int sets, MazeNode** setStarts, float* biases){
        //contianer setup
        std::forward_list<MazeNode*>* edgeList = new std::forward_list<MazeNode*>();
        std::set<int>* inSets = new std::set<int>();
        //initial set
        inSets->insert(setStarts[0]->set);
        setEdges(setStarts[0], edgeList, inSets);

        int pos, remPos, nSets, newAddition;
        Direction dir;
        std::forward_list<MazeNode*>::iterator ptr;
        while(!edgeList->empty()){
            remPos = std::rand()%std::distance(edgeList->begin(),edgeList->end());
            for(ptr = edgeList->begin(), pos = 0; pos<remPos; ptr++, pos++);
            nSets = std::rand()%(*ptr)->numNeighborSets(inSets);
            dir = North;
            while(dir!=None){
                if((*ptr)->wall(dir) && inSets->find((*ptr)->neighbor(dir)->set)==inSets->end()){
                    if(nSets!=0){
                        nSets--;
                    }
                    else{
                        newAddition = (*ptr)->neighbor(dir)->set;
                        break;
                    }
                }
                dir++;
            }
            dir = (*ptr)->biasedWall(biases[0], biases[1], biases[2], newAddition);
            (*ptr)->bridgeDirection(dir);
            inSets->insert(newAddition);
            edgeList->remove_if([inSets](MazeNode* mn){return mn->numNeighborSets(inSets)==0;});
            setEdges((*ptr)->neighbor(dir), edgeList, inSets);
        }

    }

    void setEdges(MazeNode* root, std::forward_list<MazeNode*>* edgeList, std::set<int>* inSets){
        std::vector<MazeNode*> unexplored;
        unexplored.push_back(root);
        Direction directSelect[] = {North, East, South, West};
        int i;
        bool in;
        MazeNode* current;
        while(!unexplored.empty()){
            current = unexplored.back();
            current->visited = true;
            unexplored.pop_back();
            in = false;
            for(i = 0; i<4; i++){
                if(!in && current->wall(directSelect[i]) && current->set != current->neighbor(directSelect[i])->set
                    && inSets->find(current->neighbor(directSelect[i])->set)==inSets->end()){
                    edgeList->push_front(current);
                    in = true;
                }
                if(current->hall(directSelect[i]) && !current->neighbor(directSelect[i])->visited){
                    unexplored.push_back(current->neighbor(directSelect[i]));
                }
            }
        }
    }

    int ratioToIndex(float ratio, int size){
        return static_cast<int>(floor(size*ratio));
    }

    static bool tupleComp(std::tuple<int,std::tuple<MazeNode*,MazeNode*>> a, std::tuple<int,std::tuple<MazeNode*,MazeNode*>> b){
        return std::get<0>(a) < std::get<0>(b);
    }

    struct solvNode{
        int depth;
        struct solvNode* parent;
        MazeNode* loc;

        solvNode(){
            depth = 0;
            parent = NULL;
            loc = NULL;
        }

        solvNode(int dep, struct solvNode* par, MazeNode* pos){
            depth = dep;
            parent = par;
            loc = pos;
        }
    };
};

//g++ maze.cc -o testMaze;./testMaze
//./testMaze [len] [wid] 0 0 0 0 0 0 - Creates stack-like/DFS behavior
//./testMaze [len] [wid] 1 1 1 1 0 0 - Creates queue-like/BFS behavior
//./testMaze [len] [wid] 0 1 0 1 0 1 - Creates random behavior
int main(int argc, const char *argv[]){
    //use atoi to cast from arg to int
    std::srand(std::time(0));
    Maze* testMaze = new Maze(atoi(argv[1]),atoi(argv[2]));
    MazeNode* c = testMaze->corner;
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
    testMaze->buildMaze(1, a, mazeParam, biases);
    char* txt = testMaze->toString();
    printf("Test Maze:\n%s",txt);
    // std::tuple<MazeNode*, MazeNode*> entex = testMaze->solutionNodes(1);
    // std::get<0>(entex)->display = '@';
    // std::get<1>(entex)->display = '@';
    // txt = testMaze->toString();
    // printf("Test Maze:\n%s",txt);
}
