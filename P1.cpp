#include <iostream>
#include <getopt.h>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include "P1Functions.h"

using namespace std;

// create global argument structure & define getopt_long option structure
struct globalArgs_t{
    bool stack;
    bool queue;
    char inMode;
    char outMode;
    int roomSize;
    int numFloors;
} globalArgs;

struct option longOpts[] = {
    {"stack", no_argument, NULL, 's'},
    {"queue", no_argument, NULL, 'q'},
    {"output", required_argument, NULL, 'o'},
    {"help", no_argument, NULL, 'h'}
};

// create "point" structure template for each coordinate
struct point {
    int row;
    int col;
    int level;
    char character;
    char queuedFrom;
    bool beenQueued;
};

/*
 checks whether a coordinate is valid given the room size and number of levels of a map
 */
bool isValidCoordinate(int x, int y, int z){
    if (x<0 || x>=globalArgs.roomSize){
        return false;
    }else if (y<0 || y>=globalArgs.roomSize){
        return false;
    }else if (z<0 || z>=globalArgs.numFloors){
        return false;
    } else {
        return true;
    }
}

//======================= MAIN ===================================

int main(int argc, char **argv)
{
    // initialize global arguments
    globalArgs.stack = false, globalArgs.queue = false;
    globalArgs.inMode = 0;
    globalArgs.outMode = 'M';
    globalArgs.roomSize = 0;
    globalArgs.numFloors = 0;
    bool &stack = globalArgs.stack;
    bool &queue = globalArgs.queue;
    int &roomSize = globalArgs.roomSize;
    int &numFloors = globalArgs.numFloors;
    
    // get command line arguments
    int index = 0, opt = 0;
    while ((opt = getopt_long(argc, argv, "sqo:h", longOpts, &index)) != -1) {
        switch (opt) {
            case 's':
                if (!queue && !stack){
                    stack = true;
                } else {
                    printError1();
                    return 1;
                }
                break;
            case 'q':
                if (!stack && !queue){
                    queue = true;
                } else {
                    printError1();
                    return 1;
                }
                break;
            case 'o':
                globalArgs.outMode = *optarg;
                break;
            case 'h':
                cout << "This program finds the shortest path out of the space station and back to the mothership!" << '\n'
                << "Command line arguments are as follows: " << '\n'
                << "'-s' or '--stack': if this switch is set, use stack based routing scheme." << '\n'
                << "'-q' or '--queue': if this switch is set, use queue based routing scheme." << '\n'
                << "'-o' or '--output' (M|L): indicates whether you'd like to use map or coordinate output." << '\n'
                << "'-h' or '--help': prints this message and ends the program." << '\n';
                return 0;
            default:
                break;
        }
    }
    if (!stack && !queue){
        printError1();
        return 1;
    }
    
    // initialize 3D vector container
    cin >> globalArgs.inMode;
    cin >> roomSize;
    cin >> numFloors;
    string line;
    char c;
    vector<vector<vector<point> > > map;
    map.resize(numFloors);
    for (int i=0; i<numFloors; i++){
        map[i].resize(roomSize);
        for (int j=0; j<roomSize; j++){
            map[i][j].resize(roomSize);
        }
    }
    
    // read input file (map mode)
    int startRow, startCol, startLevel = 0;
    if (globalArgs.inMode == 'M'){
        int colIndex=0, rowIndex=0, floorIndex=(roomSize*numFloors)-1;
        while (getline(cin, line)){
            if (line[0] == '/'){
                continue;
            }
            istringstream is (line);
            while (is >> c) {
                if (isValidCharacter(c)){
                    point newPoint = {rowIndex%roomSize, colIndex%roomSize, floorIndex/roomSize, c, 0, false};
                    map[floorIndex/roomSize][rowIndex%roomSize][colIndex%roomSize] = newPoint;
                    if (c == 'S'){
                        startRow = rowIndex%roomSize;
                        startCol = colIndex%roomSize;
                        startLevel = floorIndex/roomSize;
                    }
                    colIndex++;
                    if (colIndex>0 && (colIndex%4 == 0)){
                        rowIndex++;
                        floorIndex--;
                    }
                } else {
                    printError2();
                    return 1;
                }
            }
        }
        
    // read input file (coordinate mode)
    } else {
        for (int i=0; i<numFloors; i++){
            for (int j=0; j<roomSize; j++){
                for (int k=0; k<roomSize; k++){
                    point temp = {j, k, i, '.', 0, false};
                    map[i][j][k] = temp;
                }
            }
        }
        int x,y,z;
        char junk, c;
        while (getline(cin, line)){
            if (line[0] == '/'){
                continue;
            }
            istringstream is (line);
            while (is >> junk >> x >> junk >> y >> junk >> z >> junk >> c){
                if (!isValidCoordinate(x, y, z)){
                    printError3();
                    return 1;
                } else if (!isValidCharacter(c)){
                    printError2();
                    return 1;
                } else {
                    map[z][x][y].character = c;
                    if (c == 'S'){
                        startRow = x;
                        startCol = y;
                        startLevel = z;
                    }
                }
            }
        }
    }
    
    // sorting algorithms
    deque<point> deq;
    deq.push_back(map[startLevel][startRow][startCol]);
    map[startLevel][startRow][startCol].beenQueued = true;
    int hangerRow, hangerCol, hangerLevel = 0;
    bool foundHanger = false;
    
    // sorting algorithm (stack-based)
    if (stack){
        while (!foundHanger && !deq.empty()){
            point temp = deq.back();
            deq.pop_back();
            /*
             checks whether coordinate about to be queued is
             - A: a valid point on the map
             - B: a valid character (not '#')
             - C: a point that has not previously been queued
             - D: is the hanger
                - if yes, stores coordinates
             */
            if (temp.row > 0 && isValidQueueChar(map[temp.level][temp.row-1][temp.col].character) && !map[temp.level][temp.row-1][temp.col].beenQueued){
                deq.push_back(map[temp.level][temp.row-1][temp.col]);
                map[temp.level][temp.row-1][temp.col].beenQueued = true;
                map[temp.level][temp.row-1][temp.col].queuedFrom = 'n';
                if (deq.back().character == 'H'){
                    foundHanger = true;
                    hangerRow = deq.back().row;
                    hangerCol = deq.back().col;
                    hangerLevel = deq.back().level;
                    continue;
                }
            }
            if (temp.col < roomSize-1 && isValidQueueChar(map[temp.level][temp.row][temp.col+1].character) && !map[temp.level][temp.row][temp.col+1].beenQueued){
                deq.push_back(map[temp.level][temp.row][temp.col+1]);
                map[temp.level][temp.row][temp.col+1].beenQueued = true;
                map[temp.level][temp.row][temp.col+1].queuedFrom = 'e';
                if (deq.back().character == 'H'){
                    foundHanger = true;
                    hangerRow = deq.back().row;
                    hangerCol = deq.back().col;
                    hangerLevel = deq.back().level;
                    continue;
                }
            }
            if (temp.row < roomSize-1 && isValidQueueChar(map[temp.level][temp.row+1][temp.col].character) && !map[temp.level][temp.row+1][temp.col].beenQueued){
                deq.push_back(map[temp.level][temp.row+1][temp.col]);
                map[temp.level][temp.row+1][temp.col].beenQueued = true;
                map[temp.level][temp.row+1][temp.col].queuedFrom = 's';
                if (deq.back().character == 'H'){
                    foundHanger = true;
                    hangerRow = deq.back().row;
                    hangerCol = deq.back().col;
                    hangerLevel = deq.back().level;
                    continue;
                }
            }
            if (temp.col > 0 && isValidQueueChar(map[temp.level][temp.row][temp.col-1].character) && !map[temp.level][temp.row][temp.col-1].beenQueued){
                deq.push_back(map[temp.level][temp.row][temp.col-1]);
                map[temp.level][temp.row][temp.col-1].beenQueued = true;
                map[temp.level][temp.row][temp.col-1].queuedFrom = 'w';
                if (deq.back().character == 'H'){
                    foundHanger = true;
                    hangerRow = deq.back().row;
                    hangerCol = deq.back().col;
                    hangerLevel = deq.back().level;
                    continue;
                }
            }
            if (temp.character == 'E'){
                for (int i=0; i<numFloors; i++){
                    if (i != temp.level && map[i][temp.row][temp.col].character == 'E' && !map[i][temp.row][temp.col].beenQueued){
                        deq.push_back(map[i][temp.row][temp.col]);
                        map[i][temp.row][temp.col].beenQueued = true;
                        map[i][temp.row][temp.col].queuedFrom = 48+temp.level;
                    }
                }
            }
        }
    // sorting algorithm (queue-based)
    } else {
        while (!foundHanger && !deq.empty()){
            point temp = deq.front();
            deq.pop_front();
            /*
             checks whether coordinate about to be queued is
             - A: a valid point on the map
             - B: a valid character (not '#')
             - C: a point that has not previously been queued
             - D: is the hanger
                - if yes, stores coordinates
             */
            if (temp.row > 0 && isValidQueueChar(map[temp.level][temp.row-1][temp.col].character) && !map[temp.level][temp.row-1][temp.col].beenQueued){
                deq.push_back(map[temp.level][temp.row-1][temp.col]);
                map[temp.level][temp.row-1][temp.col].beenQueued = true;
                map[temp.level][temp.row-1][temp.col].queuedFrom = 'n';
                if (deq.back().character == 'H'){
                    foundHanger = true;
                    hangerRow = deq.back().row;
                    hangerCol = deq.back().col;
                    hangerLevel = deq.back().level;
                    continue;
                }
            }
            if (temp.col < roomSize-1 && isValidQueueChar(map[temp.level][temp.row][temp.col+1].character) && !map[temp.level][temp.row][temp.col+1].beenQueued){
                deq.push_back(map[temp.level][temp.row][temp.col+1]);
                map[temp.level][temp.row][temp.col+1].beenQueued = true;
                map[temp.level][temp.row][temp.col+1].queuedFrom = 'e';
                if (deq.back().character == 'H'){
                    foundHanger = true;
                    hangerRow = deq.back().row;
                    hangerCol = deq.back().col;
                    hangerLevel = deq.back().level;
                    continue;
                }
            }
            if (temp.row < roomSize-1 && isValidQueueChar(map[temp.level][temp.row+1][temp.col].character) && !map[temp.level][temp.row+1][temp.col].beenQueued){
                deq.push_back(map[temp.level][temp.row+1][temp.col]);
                map[temp.level][temp.row+1][temp.col].beenQueued = true;
                map[temp.level][temp.row+1][temp.col].queuedFrom = 's';
                if (deq.back().character == 'H'){
                    foundHanger = true;
                    hangerRow = deq.back().row;
                    hangerCol = deq.back().col;
                    hangerLevel = deq.back().level;
                    continue;
                }
            }
            if (temp.col > 0 && isValidQueueChar(map[temp.level][temp.row][temp.col-1].character) && !map[temp.level][temp.row][temp.col-1].beenQueued){
                deq.push_back(map[temp.level][temp.row][temp.col-1]);
                map[temp.level][temp.row][temp.col-1].beenQueued = true;
                map[temp.level][temp.row][temp.col-1].queuedFrom = 'w';
                if (deq.back().character == 'H'){
                    foundHanger = true;
                    hangerRow = deq.back().row;
                    hangerCol = deq.back().col;
                    hangerLevel = deq.back().level;
                    continue;
                }
            }
            if (temp.character == 'E'){
                for (int i=0; i<numFloors; i++){
                    if (i != temp.level && map[i][temp.row][temp.col].character == 'E' && !map[i][temp.row][temp.col].beenQueued){
                        deq.push_back(map[i][temp.row][temp.col]);
                        map[i][temp.row][temp.col].beenQueued = true;
                        map[i][temp.row][temp.col].queuedFrom = 48+temp.level;
                    }
                }
            }
        }
    }
    
    // backwards routing (from hanger to start)
    if (foundHanger){
        deq.clear();
        bool foundPath = false;
        point temp = map[hangerLevel][hangerRow][hangerCol];
        char tempC = temp.queuedFrom;
        while (!foundPath){
            if (tempC == 'n'){
                if (map[temp.level][temp.row+1][temp.col].character == 'S'){
                    foundPath = true;
                }
                map[temp.level][temp.row+1][temp.col].character = 'n';
                temp = map[temp.level][temp.row+1][temp.col];
            } else if (tempC == 'e'){
                if (map[temp.level][temp.row][temp.col-1].character == 'S'){
                    foundPath = true;
                }
                map[temp.level][temp.row][temp.col-1].character = 'e';
                temp = map[temp.level][temp.row][temp.col-1];
            } else if (tempC == 's'){
                if (map[temp.level][temp.row-1][temp.col].character == 'S'){
                    foundPath = true;
                }
                map[temp.level][temp.row-1][temp.col].character = 's';
                temp = map[temp.level][temp.row-1][temp.col];
            } else if (tempC == 'w'){
                if (map[temp.level][temp.row][temp.col+1].character == 'S'){
                    foundPath = true;
                }
                map[temp.level][temp.row][temp.col+1].character = 'w';
                temp = map[temp.level][temp.row][temp.col+1];
            } else {
                map[tempC][temp.row][temp.col].character = temp.level+48;
                temp = map[tempC][temp.row][temp.col];
            }
            tempC = temp.queuedFrom;
            deq.push_back(temp);
        }
    }
    
    // output (map mode)
    if (globalArgs.outMode == 'M'){
        cout << globalArgs.roomSize << '\n' << globalArgs.numFloors << '\n';
        for (int i=globalArgs.numFloors-1; i>=0; i--){
            cout << "//level " << i << '\n';
            for (int j=0; j<globalArgs.roomSize; j++){
                for (int k=0; k<globalArgs.roomSize; k++){
                    cout << map[i][j][k].character;
                }
                cout << '\n';
            }
        }
    // output (coordinate mode)
    } else {
        cout << globalArgs.roomSize << '\n' << globalArgs.numFloors << '\n' << "//path taken" << '\n';
        while (!deq.empty()) {
            point temp = deq.back();
            deq.pop_back();
            cout << "(" << temp.row << "," << temp.col << "," << temp.level << "," << temp.character << ")" << '\n';
        }
    }
    
    return 0;
}

