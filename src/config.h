//
//  File: config.h
//  Project: Exact
//	Author: Ali Jahanshahi
//  Copyright (c) 2014 (University of Tehran). All rights reserved.//

#ifndef __Exact__config__
#define __Exact__config__

#include <iostream>
#include <string>
#include <map>
using namespace std;

class config{

private :
    string file;
	
public:
    map<string, double> delay;		//Map of delay of each node in FILE file
    map<string, double> area;		//Map of area of each node in FILE file

    config(string file){
        this->file = file;
    }

    void extract();					//Extracts area and delay of each node from FILE and put them in a map
};
#endif /* defined(__Exact__config__) */