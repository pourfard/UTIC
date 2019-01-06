#ifndef SELECTION_H
#define SELECTION_H
//
//  File: selection.h
//  Project: Exact
//	Author: Ali Jahanshahi
//  Copyright (c) 2014 (University of Tehran). All rights reserved.

#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <map>
#include <cmath>

using namespace std;
class selection
{
	int** DAG;
	string path;
	string* sets;
	queue<string> wmax_CIs,gwmin_CIs;
	double *wmaxMerit,*gwminMerit;
	double* setDelay,*setArea;
	map<int,string> nodeNames;
	map<string,double> delay,area;
	int setSize;
	void importSets(queue<string>);
	void importSetAreas(queue<double>);
	void importSetDelays(queue<double>);
	int countSet(queue<string>);
	void caculateMeritWMAX();
	void caculateMeritGWMIN();
	double sofwareDelay(string);
	int degree(string);
	void insertToWmaxSet(string*,int);
	void insertToGWminSet(string*,int);
	void WMAX();
	void GWMIN();
	void drawGraph(bool);
public:
	selection(int**,string,queue<string>,queue<double>,queue<double>,map<int,string>);
	void runWMAX();
	void runGWMIN();
	~selection(void);
};
#endif
