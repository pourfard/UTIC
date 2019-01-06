//
//  File: GraphParser.h
//  Project: Exact
//	Author: Ali Jahanshahi
//  Copyright (c) 2014 (University of Tehran). All rights reserved.//

#ifndef __Exact__GraphParser__
#define __Exact__GraphParser__

#include <iostream>
#include <string>
#include <fstream>
#include <map>

using namespace std;
class GraphParser{
public:
    
    GraphParser(string);
    void extractGraph();
    int getGraphSize();
    int** getGraph();
	map<int,string> getNodeNames();
	map<string,int> getNodeNumbers();
private:
    string file;
	map<string,int> nodeNumbers;
    map<int,string> nodeName;
    
    int graphSize;
    int** graph;
    void parseFile();
    void insertNode(string);
    void generateGraph();
    void printGraph();
    void printNodes();
};

#endif /* defined(__Exact__GraphParser__) */
