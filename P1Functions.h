//
//  P1.h
//  P1
//
//  Created by Craig Send on 9/21/13.
//  Copyright (c) 2013 Craig Send. All rights reserved.
//

#ifndef P1_P1_h
#define P1_P1_h
#include "P1Structures.h"
using namespace std;

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
    cerr << "there is an invalid character on the map. abort program." << '\n';
    return;
}

/*
 prints informative message to standard error if coordinates given in coordinate input mode are invalid
 */
void printError3(){
    cerr << "one of the coordinates listed in the input file is invalid. abort program." << '\n';
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

#endif
