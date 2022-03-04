#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <ctime>
#include <math.h>
#include <cmath>

//Shorthand for directions
enum Direction {North=0, East, South, West, None};
Direction operator ++(Direction& dir, int){
    Direction old = dir;
    if(4 < dir+1) dir = North;
    else dir = static_cast<Direction>(dir+1);
    return old;
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
    bool inMaze;

    std::tuple<int,int> coordinate;

    //direction management
    Direction entry;

    //display specs
    char display = ' ';

    MazeNode(int st, std::tuple<int,int> coord){
        set = st;
        inMaze = false;
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
                upHall = true;
                break;
            case East:
                if(rightNode == NULL) return;
                rightNode->leftHall = true;
                rightNode->set = set;
                rightHall = true;
                break;
            case South:
                if(downNode == NULL) return;
                downNode->upHall = true;
                downNode->set = set;
                downHall = true;
                break;
            case West:
                if(leftNode == NULL) return;
                leftNode->rightHall = true;
                leftNode->set = set;
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

    int numHalls(){
        return upHall+rightHall+downHall+leftHall;
    }

    int numValidWalls(){
        int wallCount = 0;
        for(Direction compass=North; compass<None; compass++){
            if(wall(compass) && neighbor(compass)!=NULL && neighbor(compass)->set==0){
                wallCount++;
            }
        }
        return wallCount;
    }

    Direction randomHall(){
        if(numHalls() == 0) return None;
        int pos = std::rand()%numHalls();
        for(Direction compass=North; compass<None; compass++){
            if(!wall(compass)&&!pos--){return compass;}
        }
        return None;
    }
    Direction randomWall(){
        if(numValidWalls() == 0) return None;
        int pos = std::rand()%numValidWalls();
        for(Direction compass=North; compass<None; compass++){
            if(wall(compass) && neighbor(compass)!=NULL && neighbor(compass)->set==0&&!pos--)
                return compass;
        }
        return None;
    }

    Direction biasedHall(){
        return randomHall();
    }
    Direction biasedWall(){
        return randomWall();
    }
};

class Maze{
public:
    // class Frontier{
    // public:
    //     class Node{
    //     public:
    //         MazeNode* node;
    //         Node* next;
    //         Node* prev;
    //         Node(MazeNode* n, Node* nxt, Node* previous){
    //             node = n;
    //             next = nxt;
    //             prev = previous;
    //         }
    //     };
    //     Node* head;
    //     Node* tail;
    //     int len;
    //     Frontier(int n){
    //         head = NULL;
    //         tail = NULL;
    //         len = 0;
    //     }
    //     void add(MazeNode* insert, int position){
    //         if(head == NULL){
    //             head = new Node(insert, NULL, NULL);
    //             tail = head;
    //         }
    //         else{
    //             tail->next =new Node(insert, NULL, tail);
    //             tail = tail->next;
    //         }
    //         len++;
    //     }
    //     MazeNode* pop(int pos){
    //         if(size() == 0){
    //             std::cout << "Cannot pop empty" <<std::endl;
    //             return NULL;
    //         }
    //
    //         MazeNode* out = tail->node;
    //         tail = tail->prev;
    //         if(tail==NULL){
    //             head = NULL;
    //         }
    //         else tail->next = NULL;
    //         len--;
    //         return out;
    //     }
    //     bool empty(){
    //         return size()<=0;
    //     }
    //     int size(){
    //         return len;
    //     }
    // };

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
    float popLo, popHi;
    float pushLoOld, pushHiOld;
    float pushLoNew, pushHiNew;
    float dirBias;
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
//inOld, float inNew, float popLoc
    void buildMaze(int starts, std::tuple<int,int>* startCells){
        pushLoNew = 0;
        pushHiNew = 1;
        pushLoOld = 0;
        pushHiOld = 1;
        popLo = 0;
        popHi = 1;
        Frontier* field = new Frontier(length*width, (pushHiOld+pushLoOld)/2, (pushHiNew+pushLoNew)/2, (popHi+popLo)/2);
        MazeNode* current;
        Direction wall = None;
        for(int i = 0; i<starts; i++){
            current = getNode(startCells[i]);
            current->set = i+1;//0 is default "not in maze" state
            insertFrontier(field, current, pushLoNew, pushHiNew);
        }
        while(!field->empty()){
            current = popFrontier(field, popLo, popHi);
            wall = current->biasedWall();
            if(wall==None){continue;}
            current->bridgeDirection(wall);
            if(current->numValidWalls()){
                insertFrontier(field, current, pushLoOld, pushHiOld);
            }
            if(current->neighbor(wall)->numValidWalls()){
                insertFrontier(field, current->neighbor(wall), pushLoNew, pushHiNew);
            }
        }
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
        else return solutionNodes(0);
    }

    int ratioToIndex(float ratio, int size){
        return static_cast<int>(floor(size*ratio));
    }
};

//g++ maze.cc -o testMaze;./testMaze
int main(int argc, const char *argv[]){
    //use atoi to cast from arg to int
    std::srand(std::time(0));
    Maze* testMaze = new Maze(10,10);
    MazeNode* c = testMaze->corner;
    std::tuple<int,int> a[] = {std::make_tuple(0,0)};
    //char* txt = testMaze->toString();
    //printf("Test Maze:\n%s",txt);
    testMaze->buildMaze(1, a);
    std::tuple<MazeNode*, MazeNode*> entex = testMaze->solutionNodes(0);
    char* txt = testMaze->toString();
    printf("Test Maze:\n%s",txt);
}
