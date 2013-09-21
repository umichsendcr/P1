//
//  P1Structures.h
//  P1
//
//  Created by Craig Send on 9/21/13.
//  Copyright (c) 2013 Craig Send. All rights reserved.
//

#ifndef P1_P1Structures_h
#define P1_P1Structures_h

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
    char character;
    bool beenVisited;
};

#endif
