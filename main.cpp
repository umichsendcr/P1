#include <iostream>
#include <getopt.h>
#include <string>

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

// HELPER FUNCTIONS == == == == == == == == == == == == == == == == == == ==

/* 
 prints informative message to standard error if more or less than one --stack or
 --queue flag is found on the command line
*/
void printError1(){
    cerr << "you must choose either stack routing scheme or queue routing scheme, abort program." << '\n';
    return;
}

int main(int argc, char **argv)
{
    // initialize global arguments
    globalArgs.stack = false, globalArgs.queue = false;
    globalArgs.inMode = 0;
    globalArgs.outMode = 'M';
    globalArgs.roomSize = 0;
    globalArgs.numFloors = 0;
    
    // parse in command line arguments
    int index = 0, opt = 0;
    while ((opt = getopt_long(argc, argv, "sqo:h", longOpts, &index)) != -1) {
        switch (opt) {
            case 's':
                if (!globalArgs.queue){
                    globalArgs.stack = true;
                } else {
                    printError1();
                    return 1;
                }
                break;
            case 'q':
                if (!globalArgs.stack){
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
    
    
    return 0;
}

