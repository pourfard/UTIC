//
//  File: integtator.h
//  Project: Exact
//	Author: Ali Jahanshahi
//  Copyright (c) 2014 (University of Tehran). All rights reserved.//

#ifndef __Exact__integtator__
#define __Exact__integtator__

#include <iostream>
#include <string>
#include "exact.h"
#include "config.h"
#include "GraphParser.h"
#include "selection.h"

using namespace std;

class integrator{
private:
    queue<string> sets;
	queue<double> setDelay;
	queue<double> setArea;
    string fileNAme;
    string path,configInput,graphParserInput;
    exact* e;
    config* c;
    GraphParser* parser;
	selection* s;
    bool* forbidden;
    bool* inOut;
    double* delay;
    double* area;
    int graphSize;
    int** graph;
	void init();
    void loadDelays();
    void loadAreas();
    void loadForbidden();
    void detemineIOs();
	void findSubset(int,int);
public:
	    integrator(string,string,string);
        void findCI(int,int);
		void runSelection(bool,bool);
};
#endif /* defined(__Exact__integtator__) */
