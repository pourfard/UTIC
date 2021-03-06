//
//  File: integtator.cpp
//  Project: Exact
//	Author: Ali Jahanshahi
//  Supervisor: Dr. Salehi
//  Copyright (c) 2015 (University of Tehran). All rights reserved.
//

#include <cstdlib>
#include "integrator.h"
#define SEPARATOR "/"

integrator::integrator(string path,string configAddr,string fileName){
	//Delete All previous data (if exist)
	char a[200];
	string cmd="rm -rf \""+path+SEPARATOR+fileName.substr(0,fileName.find(".gv"))+"\"\n";
	
	for(int i=0;i<cmd.size();i++)a[i]=cmd.at(i);
	a[cmd.size()]='\0';
	system(a);
	//Initializations
	this->configInput=configAddr;
	this->graphParserInput=fileName;
    this->fileNAme  = fileName;
    this->path = path;
}
void integrator::init(){
	//
	//Read Config File
	//
	c = new config(configInput);
	c->extract();
	//
	//read graphviz file and convert to a int**
	//
	parser = new GraphParser(graphParserInput);
    parser->extractGraph();
    this->graphSize = parser->getGraphSize();
	//
	//Load all neccessory data extracted by parser and config
	//
	//Determine I/Os
	detemineIOs();
	//Load area and delay of each nnode
    loadAreas();
    loadDelays();
	//Load forbidden nodes -> not used yet
    loadForbidden();
}
void integrator::findCI(int readPort,int writePort){
	//Generate all possible subsets from graph
	
	//2-1,			2-2,		2-3,		...,	2-writePort
	//3-1,			3-2,		3-3,		...,	3-writePort
	//4-1,			4-2,		4-3,		...,	4-writePort
	//...,			...,		...,		...,	...
	//readPort-1,	readPort-2,	readPort-3,	...,	readPort-writePort

	for(int i=2;i<=readPort;i++)
		for(int j=1;j<=writePort;j++)
			findSubset(i,j);
}
void integrator::runSelection(bool runwmax,bool rungwmin){
	parser->extractGraph();
	string newPath=path+SEPARATOR+fileNAme.substr(0,fileNAme.find(".gv"));
	s = new selection(parser->getGraph(),newPath,this->sets,this->setDelay,this->setArea,parser->getNodeNames());
	if(runwmax==true)
		s->runWMAX();
	if(rungwmin==true)
		s->runGWMIN();
}
void integrator::findSubset(int readPorts, int writePorts){
	init();
    string newPath=path+SEPARATOR+fileNAme.substr(0,fileNAme.find(".gv"));
	//make a path for output files
	string cmd="mkdir \""+newPath+"\"";;
	char cmdChar[200];
	for(int i=0;i<cmd.size();i++)cmdChar[i]=cmd.at(i);
	cmdChar[cmd.size()] = '\0';
	system(cmdChar);
	//make a paht for subSets outputs
    newPath+= SEPARATOR+to_string(readPorts)+"R_"+to_string(writePorts)+"W";
	char cmdChar2[200];
	string cmd2="mkdir \""+newPath+"\"\n";
	for(int i=0;i<cmd2.size();i++)cmdChar2[i]=cmd2.at(i);
	cmdChar2[cmd2.size()] = '\0';
	system(cmdChar2);
	//run exact
	e = new exact(parser->getGraph(),inOut,graphSize,writePorts,readPorts,delay,area,forbidden,parser->getNodeNames(),newPath,fileNAme);
    e->run();
	
	//get subsets generated by exact algorithm
	queue<string> t=e->getSet();
	while(!t.empty()){
		this->sets.push(t.front());
		t.pop();
	}
	//get delay of each subset calculated by exact algorithm
	queue<double> delays=e->getSetDelay();
	while(!delays.empty()){
		this->setDelay.push(delays.front());
		delays.pop();
	}
	//get area of each subset calculated by exact algorithm
	queue<double> areas=e->getSetArea();
	while(!areas.empty()){
		this->setArea.push(areas.front());
		areas.pop();
	}
	//run batch file to convert output of exact which is .gv to picture format
	//this batch generated in exact algorithm
	/*string cmd3 = "\""+newPath+".bat\"";
	char cmdChar3[200];
	for(int i=0;i<cmd3.size();i++)cmdChar3[i]=cmd3.at(i);
	cmdChar3[cmd3.size()]='\0';
	system(cmdChar3);*/
	
	//delete batch file to convert output of exact which is .gv to picture format
	//this batch generated in exact algorithm
	/*string cmd4 = "\"del "+newPath+".bat\"";
	char cmdChar4[200];
	for(int i=0;i<cmd4.size();i++)cmdChar4[i]=cmd4.at(i);
	cmdChar4[cmd4.size()]='\0';
	system(cmdChar4);*/
}
//Identify Inputs and outputs : if(node[i]=input || node[i]==output) then inOut[i]=true
void integrator::detemineIOs(){
    inOut=new bool[graphSize];
    for (int i=0; i<graphSize; i++) {
        inOut[i]=false;
    }
    
    for (int i=0; i<graphSize; i++) {
        bool notInput=false;
        for (int j=0; j<graphSize; j++) {
            if (i!=j) {
				notInput |= (bool) parser->getGraph()[j][i];
            }
        }
        if (notInput==false) {
            inOut[i]=true;
        }
    }
    
    for (int i=0; i<graphSize; i++) {
        bool notOutput=false;
        for (int j=0; j<graphSize; j++) {
            if (i!=j) {
				notOutput |= (bool) parser->getGraph()[i][j];
            }
        }
        if (notOutput==false) {
            inOut[i]=true;
        }
    }

}

string myop(string op)
{
	char ch = op[1];
	switch(ch)
	{
		case '*':return "\"*\"";break;
		case '+':return "\"+\"";break;
		case '-':return "\"-\"";break;
		case '/':return "\"/\"";break;
		case '<':return "\"<<\"";break;
		case '>':return "\">>\"";break;
		case '|':return "\"|\"";break;
		case '&':return "\"&\"";break;
		default: return "N";
	}
}

void integrator::loadAreas(){
    area = new double[graphSize];
	string operation;
    for (int i=1; i<graphSize+1; i++) {
	operation = parser->getNodeNames()[i];
	operation = myop(operation);
		area[i-1] = (double)c->area[operation];
    }
}
void integrator::loadDelays(){
    delay = new double[graphSize];
	string operation;
    for (int i=1; i<graphSize+1; i++) 
{
	operation = parser->getNodeNames()[i];
	operation = myop(operation);
		delay[i-1] = (double)c->delay[operation];
    }
}
//TODO
//all nodes are valid nodes, identify forbiden nodes!
void integrator::loadForbidden()
{
    forbidden = new bool[graphSize];
    for (int i=0; i<graphSize; i++) 
{
        forbidden[i]=false;
    }
}
