//
//  File: exact.h
//  Project: Exact
//	Author: Ali Jahanshahi
//  Copyright (c) 2014 (University of Tehran). All rights reserved.//

#ifndef __Exact__exact__
#define __Exact__exact__
#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

class exact{

public:

    exact(int **dag,bool*,int,int,int,double*,double*,bool*,map<int, string>,string,string);
	
	void run();						//Runs Exact algorithm on input graph with input configurations
	


	//Sets, delays and areas which are put in queues are ordered
	//ie: first set in SETS queue has delay of first element in SETDELAY queue and area of SETAREA

	queue<string> getSet();			//returns all sets which are found by runnig exact algo. 
	queue<double> getSetDelay();	//returns ciritcal path delay of sets which are found by runnig exact algo. 
	queue<double> getSetArea();		//returns area of sets which are found by runnig exact algo. 

private:
	
	ofstream batch;
	
    string fileName;
    string path;					//Path of files 
    
	
	int generatedFiles;				//number of file which are generated by algorithm in each folder
    int **DAG;						//input graph -> dag
    int size;						//size of input graph
    int numberOfForbiddenNodes;
    int RF_write,RF_read;			//register file input and ouputs


	double *delay;					//delay of each node, used for calculation of delay of each set
    double *area;					//area of each node, used for calculation of area of each set

    
	bool *forbiddenNodes;			//forbidden nodes TODO: there is no forbidden nodes in this version
    bool *visited;					//for DFS algorithm and etc
    bool* inouts;					//indicates whether a node in input or output
    
	
	queue<int> orderedNodes;		//result of topologycal sort

	
	//Sets, delays and areas which are put in queues are ordered
	//ie: first set in SETS queue has delay of first element in SETDELAY queue and area of SETAREA

	queue<double> setDelay,setArea;	//result of Exact algorithm: delay and area of each set
	queue<string> sets;				//result of Exact algorithm: valid sets
    

	map<int, string> nodeNames;		//nodeNames[nodeNumber] = nodeName: mapping each node with a number to coresponding name
    

    void dfs(int);
    void makeFlase(bool*,int);
    void emptyDag(int**);
    bool containForbidden(const bool*);
    double cutCriticalDelay(const bool*);
    bool isConvex(const bool*);
    bool convexityDFS(int,const bool*);
    bool isConnected(const bool*);
    bool permanentInputOK(const bool*);
    bool permanentOutputOK(const bool*);
    bool finalInputCheck(const bool*);
    bool finalOutputCheck(const bool*);
    int nodeNumber(int*);
    double pathDelay(const bool*, int);
    double cutArea(bool*);
    void outputGraph(const bool*,double,double);
	void topologicalSort();
    void printDAG(int**);
    void makeDagTopologic();
    void treeTraversal(int,bool);
	
};
#endif /* defined(__Exact__exact__) */