//
//  P1.h
//  P1
//
//  Created by Craig Send on 9/21/13.
//  Copyright (c) 2013 Craig Send. All rights reserved.
//

#ifndef P1_P1_h
#define P1_P1_h
#include "P1Functions.cpp"

/*
 prints informative message to standard error if more or less than one --stack or
 --queue flag is found on the command line
 */
void printError1();

/*
 prints informative message to standard error if input character is not a legal map character
 */
void printError2();

/*
 prints informative message to standard error if coordinates given in coordinate input mode are invalid
 */
void printError3();

/*
 checks c to see if c is a legal map character
 */
bool isValidCharacter(char c);

/*
 checks c to see if c is able to be added to the queue
    - i.e. c != '#'
 */
bool isValidQueueChar(char c);


#endif
