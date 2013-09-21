#include <iostream>
#include <getopt.h>
#include <string>
#include <sstream>
#include <vector>
#include "P1Functions.h"
#include "P1Structures.h"

using namespace std;

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
    if (globalArgs.inMode == 'M'){
        int colIndex=0, rowIndex=0, floorIndex=(roomSize*numFloors)-1;
        while (getline(cin, line)){
            if (line[0] == '/'){
                continue;
            }
            istringstream is (line);
            while (is >> c) {
                if (isValidCharacter(c)){
                    point newPoint = {c, false};
                    map[floorIndex/roomSize][rowIndex%roomSize][colIndex%roomSize] = newPoint;
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
    } else if (globalArgs.inMode == 'L'){
        for (int i=0; i<numFloors; i++){
            for (int j=0; j<roomSize; j++){
                for (int k=0; k<roomSize; k++){
                    point temp = {'.', false};
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
                }
            }
        }
    }
    
    for (int i=0; i<globalArgs.numFloors; i++){
        for (int j=0; j<globalArgs.roomSize; j++){
            for (int k=0; k<globalArgs.roomSize; k++){
                cout << map[i][j][k].character;
            }
            cout << endl;
        }
        cout << endl;
    }
    return 0;
}

