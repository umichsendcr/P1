//
//  File.cpp
//  P1
//
//  Created by Craig Send on 9/22/13.
//  Copyright (c) 2013 Craig Send. All rights reserved.
//

#include <iostream>
#include "P1Functions.h"

using namespace std;

void printError1(){
    cerr << "you must choose between stack routing scheme or queue routing scheme, and you may not choose both or choose one twice. abort program." << '\n';
    return;
}

void printError2(){
    cerr << "there is an invalid character on the map. abort program." << '\n';
    return;
}

void printError3(){
    cerr << "one of the coordinates listed in the input file is invalid. abort program." << '\n';
}

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

bool isValidQueueChar(char c){
    switch (c) {
        case '.':
        case 'E':
        case 'S':
        case 'H':
            return true;
        default:
            return false;
    }
}

