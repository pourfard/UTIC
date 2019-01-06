//
//  File: GraphParser.cpp
//  Project: Exact
//	Author: Ali Jahanshahi
//  Supervisor: Dr. Salehi
//  Copyright (c) 2015 (University of Tehran). All rights reserved.
//
#include "GraphParser.h"
#define LINE_LENGHT 200
#define STRING_MAX_RANGE 18446744073709551610

GraphParser::GraphParser(string file){
    this->file=file;
    this->graphSize=1;
    nodeNumbers.clear();
    nodeName.clear();
}
map<string,int> GraphParser::getNodeNumbers(){
	return this->nodeNumbers;
}
map<int,string> GraphParser::getNodeNames(){
	return this->nodeName;
}
void GraphParser::extractGraph(){
    parseFile();
    generateGraph();
}

void GraphParser::printNodes(){
    cout<<"size: "<<graphSize-1<<endl;

    for (int i=1; i<graphSize; i++) {
        cout<<nodeName[i]<<" is "<<nodeNumbers[nodeName[i]]-1<<endl;
    }
}
void GraphParser::printGraph(){
    for (int i=0; i<graphSize-1; i++) {
        for (int j=0; j<graphSize-1; j++) {
            cout<<graph[i][j]<<"\t";
        }
        cout<<endl;
    }
}
void GraphParser::parseFile(){
    ifstream f;
    f.open(file);
    string line;
    if (f.is_open()) {
        while (!f.eof()) {
            getline(f,line);
            if (line.find("->")<LINE_LENGHT) {
                line = line.substr(0,line.find(";"));
                //cout<<line<<endl;
                string node1,node2;
                //Extract first node
                node1 = line.substr(0,line.find(" -> "));
                //Extract second node
                node2 = line.substr(line.find(" -> ")+4,line.size());
                insertNode(node1);
                insertNode(node2);
            }
        }
    }else{
        cerr<<"GraphParser::parseFile: "+file+" cann't be opened!\n";
    }
    f.close();
}


void GraphParser::insertNode(string nodeName){
    if (nodeNumbers[nodeName]!=0) {
        return;
    }
    nodeNumbers[nodeName]=graphSize;
    this->nodeName[graphSize]=nodeName;
    graphSize++;
}


void GraphParser::generateGraph(){

    graph = new int*[graphSize-1];
    for (int i=0; i<graphSize-1; i++) {
        graph[i] = new int[graphSize-1];
    }
    for (int i=0; i<graphSize-1; i++) {
        for (int j=0; j<graphSize-1; j++) {
            graph[i][j]=0;
        }
    }
    
    ifstream f;
    f.open(file);
    string line;
    if (f.is_open()) {
        while (!f.eof()) {
            getline(f,line);
            if (line.find("->")<LINE_LENGHT) {
                line = line.substr(0,line.find(";"));
                //cout<<line<<endl;
                string node1,node2;
                //Extract first node
                node1 = line.substr(0,line.find(" -> "));
                //Extract second node
                node2 = line.substr(line.find(" -> ")+4,line.size());
                graph[nodeNumbers[node1]-1][nodeNumbers[node2]-1]=1;
                graph[nodeNumbers[node1]-1][nodeNumbers[node1]-1]=nodeNumbers[node1]+1;
                graph[nodeNumbers[node2]-1][nodeNumbers[node2]-1]=nodeNumbers[node2]+1;
            }
        }
    }else{
        cerr<<"GraphParser::generatGraph: "+file+" cann't be opened!\n";
    }
    f.close();
}

int GraphParser::getGraphSize(){
    return graphSize-1;
}

int** GraphParser::getGraph(){
    return this->graph;
}