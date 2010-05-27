#ifndef _LOADER_H_
#define _LOADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "model.h"

//Toggle for debug statements
//#define MODEL_DEBUG

using namespace std;
using namespace mdl;

//Definitions
#define MAX_MODEL_LINE_LENGTH 255

namespace loader
{
    extern char line[MAX_MODEL_LINE_LENGTH];

    //Function taken from stackoverflow.com
    //Checks a string for a specified ending string
    extern bool hasEnding(std::string const &modelFile,
        std::string const &extension);

    //Reads in triangle data from a .obj file.
    extern void readFile(const char* filename);
}

#endif
