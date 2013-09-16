#include <iostream>
#include <getopt.h>
#include <string>
#include <sstream>

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
};

// HELPER FUNCTIONS == == == == == == == == == == == == == == == == == == ==

/* 
 prints informative message to standard error if more or less than one --stack or
 --queue flag is found on the command line
*/
void printError1(){
    cerr << "you must choose between stack routing scheme or queue routing scheme, and you may not choose both or choose one twice. abort program." << '\n';
    return;
}

/*
 prints informative message to standard error if input character is not a legal map character
*/
void printError2(){
    cerr << "there is an invalid character on the map. abort program" << '\n';
    return;
}

/*
 checks c to see if c is a legal map character
*/
bool isValidCharacter(char c){
    switch (c) {
        case '.':
        case '#':
        case 'S':
        case 'H':
        case 'E':
            return true;
        default:
            return false;
    }
}

// ==========================================================================
int main(int argc, char **argv)
{
    // initialize global arguments
    globalArgs.stack = false, globalArgs.queue = false;
    globalArgs.inMode = 0;
    globalArgs.outMode = 'M';
    globalArgs.roomSize = 0;
    globalArgs.numFloors = 0;
    
    // get command line arguments
    int index = 0, opt = 0;
    while ((opt = getopt_long(argc, argv, "sqo:h", longOpts, &index)) != -1) {
        switch (opt) {
            case 's':
                if (!globalArgs.queue && !globalArgs.stack){
                    globalArgs.stack = true;
                } else {
                    printError1();
                    return 1;
                }
                break;
            case 'q':
                if (!globalArgs.stack && !globalArgs.queue){
                    globalArgs.queue = true;
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
    if (!globalArgs.stack && !globalArgs.queue){
        printError1();
        return 1;
    }
    
    // read input file
    cin >> globalArgs.inMode;
    cin >> globalArgs.roomSize;
    cin >> globalArgs.numFloors;
    string line;
    char c;
    point board[globalArgs.roomSize][globalArgs.roomSize][globalArgs.numFloors];
    int rowIndex = 0; int colIndex = 0; int roomIndex = ((globalArgs.roomSize * globalArgs.numFloors) - 1);
    while (getline(cin, line)){
        if (line[0] == '/'){
            continue;
        }
        istringstream is (line);
        while (is >> c) {
            if (isValidCharacter(c)){
                board[colIndex%globalArgs.roomSize][rowIndex%globalArgs.roomSize][roomIndex%globalArgs.roomSize].character = c;
                colIndex++;
            } else {
                printError2();
                return 1;
            }
        }
        rowIndex++;
        roomIndex--;
    }
    
    
    return 0;
}

