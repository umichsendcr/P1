//
//  P1.h
//  P1
//
//  Created by Craig Send on 9/21/13.
//  Copyright (c) 2013 Craig Send. All rights reserved.
//

#ifndef P1_P1_h
#define P1_P1_h

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


#endif