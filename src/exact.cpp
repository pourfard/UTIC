//
//  File: exact.cpp
//  Project: Exact
//	Author: Ali Jahanshahi
//  Supervisor: Dr. Salehi
//  Copyright (c) 2015 (University of Tehran). All rights reserved.
//

#include "exact.h"
#include <string>
using namespace std;

#define SEPARATOR "/"
#define MAX_NODE 1000

exact::exact(int **dag,bool* inouts,int size, int RF_write, int RF_read,double* delay,double *area,bool *forbiddenNodes,map<int, string> nodeNames,string path,string fileName){
	/*batch.open(path+".bat",ios_base::app);
    batch<<"@echo off"<<"\n";
	batch.close();*/
    this->fileName=fileName;
    this->path = path;
    this->nodeNames = nodeNames;
    this->inouts=inouts;
    this->delay = delay;
    this->area=area;
    this->RF_write = RF_write;
    this->RF_read = RF_read;
    this->size=size;
    this->forbiddenNodes=forbiddenNodes;
    this->DAG = dag;
    this->generatedFiles=0;
    this->visited=NULL;
}
//Fills DAG with zero
void exact::emptyDag(int **dag){
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            dag[i][j]=0;
        }
    }
}
//changes all nodes VISITED flag to FLASE
void exact::makeFlase(bool *arr,int size){
    for (int i=0; i<size; i++) {
        arr[i]=false;
    }
}
//BackTrack makes tree of DAG
void exact::treeTraversal(int i,bool value){
    static bool cut[MAX_NODE]={false};
    if (i==size ) {
        return;
    }
    cut[i]=value;
    //To make sure no Input and Outputs are going to be added to cut
    if (inouts[nodeNumber(DAG[i])]==true && value==true) return;
    if (value==true) {
        if (containForbidden(cut))return;
        if (!isConvex(cut))return;
        if (finalInputCheck(cut) && finalOutputCheck(cut)) {
			int nodes=0;
			for(int k=0;k<size;k++){
				if(cut[k]==true){
					nodes++;
				}
			}
            double delay = cutCriticalDelay(cut);
			double area = cutArea(cut);
            if (delay>1) return;
			if (nodes>1){
				bool* set= new bool[size];
				for(int k=0;k<size;k++)set[k]=false;
				for(int k=0;k<size;k++){
					if(cut[k]==true)
						set[nodeNumber(DAG[k])]=true;
				}
				string newSet="";
				for(int k=0;k<size;k++){
					if(set[k]==true)
						newSet.push_back('1');
					else
						newSet.push_back('0');
				}
				setArea.push(area);
				setDelay.push(delay);
				sets.push(newSet);
				//batch.open(path+".bat",ios_base::app);
		        outputGraph(cut, delay, area);
				//batch.close();
			}
        }
    }
    treeTraversal(i+1, true);
    treeTraversal(i+1, false);
}
queue<double> exact::getSetArea(){
	return setArea;
}
queue<double> exact::getSetDelay(){
	return setDelay;
}
void exact::outputGraph(const bool* cut,double delay,double area){

    string nodeInCut = "[style=\"rounded\",shape=\"doublecircle\",color=\"red\"];\n";
    string normalNode = "[style=\"rounded\",shape=\"circle\"];\n";
    string InOut = "[style=\"rounded\",shape=\"none\"];\n";
	
    string fileN= path+"/"+std::to_string(RF_read)+"R_"+std::to_string(RF_write)+"W_"+to_string(generatedFiles++)+".dot";
    ofstream file;
    file.open(fileN);
    file<<"digraph{\n";
    for (int i=0; i<size; i++) {
        string node=nodeNames[nodeNumber(DAG[i])+1]+" ";
        if (inouts[nodeNumber(DAG[i])]==true) {
            node+=InOut;
        }
        else if (cut[i]==true) {
            node+=nodeInCut;
        }
        else{
            node+=normalNode;
        }
        file<<node;
    }
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            string edges="";
            if (DAG[i][j]==1) {
                edges+= nodeNames[nodeNumber(DAG[i])+1]+" -> "+nodeNames[nodeNumber(DAG[j])+1]+";\n";
            }
            file<<edges;
        }
    }
    file<<"}";
}
double exact::cutArea(bool *cut){
    double area=0;
    for (int i=0; i<size; i++) {
        if (cut[i]==true) {
            area+=this->area[nodeNumber(DAG[i])];
        }
    }
    return area;
}
//checks for input ports of cut for violation from register file specs
bool exact::finalInputCheck(const bool *cut){
    int numberOfInputs=0;
    visited = new bool[size];
    makeFlase(visited, size);
    for (int i=0; i<size; i++) {
        if (cut[i]==true) {
            for (int j=0; j<size; j++) {
                if (DAG[j][i]==1 && cut[j]==false) {
                    visited[j]=true;
                }
            }
        }
    }
    for (int i=0; i<size; i++) {
        if (visited[i]==true) {
            numberOfInputs++;
        }
    }
    delete visited;
    visited=NULL;
    if (numberOfInputs!=RF_read) {
        return false;
    }
    return true;
}
//calculates speedUp
double exact::cutCriticalDelay(const bool *cut){
    double del =0;
    for (int i=0; i<size; i++) {
        if (cut[i]==true) {
            double d= pathDelay(cut,i);
            if (del<d) {
                del=d;
            }
        }
    }
    return del;
}
double exact::pathDelay(const bool *cut, int node){
    double del = 0;
    for (int i=0 ; i<size; i++) {
        if (DAG[node][i]==1) {
            if (cut[i]==true) {
                double t =pathDelay(cut,i);
                if(del<t ){
                    del=t;
                }
            }

        }
    }
    del+=delay[nodeNumber(DAG[node])];
    return del;
}
//checks where choosen cut is connected
bool exact::isConnected(const bool *cut){
    int nodes=0;
    for (int i=0; i<size; i++) {
        if (cut[i]==true) {
            nodes++;
        }
    }
    if (nodes==1) {
        return true;
    }
    visited = new bool[size];
    for (int i=0; i<size; i++) {
        visited[i]=false;
    }
    for (int i=0; i<size; i++) {
        if (cut[i]==true) {
            for (int j=0; j<size; j++) {
                if (cut[j]==true) {
                    if (DAG[i][j]==1 || DAG[j][i]==1) {
                        visited[i]=true;
                        visited[j]=true;
                    }
                }
            }
        }
    }
    
    for (int i=0; i<size; i++) {
        if (cut[i]==true) {
            if ( visited[i]==false) {
                return false;                
            }

        }
    }
    delete visited;
    visited=NULL;
    return true;
}
//checks convexity from all nodes
bool exact::isConvex(const bool *cut){
    for (int i=size; i>=0; i--) {
        if (cut[i]==true) {
            for (int j=0; j<size; j++) {
                if (DAG[i][j]==1 && cut[j]==false) {
                    if (convexityDFS(j,cut)==false)
                        return false;
                }
            }

        }
    }
    return true;
}
//Checks for convexity by DFS algorithm
bool exact::convexityDFS(int node,const bool *cut){
    if (cut[node]==true) {
        return false;
    }
    bool f=true;
    for (int i=0; i<size; i++) {
        if (DAG[node][i]==1) {
            f&= convexityDFS(i,cut);
        }
    }
    return f;
}
//Check whether node is forbiddeen or not!
bool exact::containForbidden(const bool *cut){
    for (int i=0; i<size; i++) {
        if (cut[i]==true){
            if (forbiddenNodes[nodeNumber(DAG[i])]==true) {
                return true;
            }
        }
    }
    return false;
}
bool exact::finalOutputCheck(const bool* cut){
    int numberOfoutputs=0;
    for (int i=0; i<size; i++) {
        if (cut[i]==true) {
            bool first=true;
            for (int j=0; j<size; j++) {
                if (cut[j]==false && DAG[i][j]==1) {
                    if (first==true) {
                        numberOfoutputs++;
                        first=false;
                    }
                }
            }
        }
    }
    if (numberOfoutputs!=RF_write) {
        return false;
    }
    return true;
}
//Finds All nodes without any inputs
//Starts topological sort from nodes without inputs
void exact::topologicalSort(){
    //Finds All no input nodes
    queue<int> noInputNodes;
    bool *noInput= new bool[size];
    for (int i=0; i<size; i++) {
        noInput[i]=true;
    }
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            if (DAG[i][j]==1) {
                noInput[j]=false;
            }
        }
    }
    
    for (int i=0; i<size; i++) {
        if (noInput[i]==true) {
            noInputNodes.push(i);
        }
    }
    //Do topologicl sort
    visited = new bool[size];
    makeFlase(visited, size);
    while (!noInputNodes.empty()) {
        dfs(noInputNodes.front());
        noInputNodes.pop();
    }
    delete visited;
    visited=NULL;
}

//Kind of DFS algorithm for topological sort
void exact::dfs(int n){
    visited[n]=true;
    for (int i=0; i<size; i++) {
        if (DAG[n][i]==1 && visited[i]==false) {
            dfs(i);
        }
    }
    orderedNodes.push(n);
}


//Prints DAG
void exact::printDAG(int** dag){
    cout<<"DAG:\n";
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            cout<<dag[i][j]<<",";
        }
        cout <<endl;
    }
    cout<<endl;
}

//Makes DAG topological in order to
void exact::makeDagTopologic(){
    int **tmp = new int*[size];
    for (int i=0; i<size; i++) {
        tmp[i] = new int[size];
    }
    int n=0;
    while (!orderedNodes.empty()) {
        int node = orderedNodes.front();
        for (int i=0; i<size; i++) {
            tmp[n][i]=DAG[node][i];
        }
        n++;
        orderedNodes.pop();
    }
    emptyDag(DAG);
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            if (tmp[i][j]==1) {
                for (int k=0; k<size; k++) {
                    if (nodeNumber(tmp[k])==j) {
                        DAG[i][k]=1;
                    }
                }
            }
            else if (tmp[i][j]!=1 && tmp[i][j]!=0){
                DAG[i][i]=tmp[i][j];
            }
        }
    }
    return;
}

int exact::nodeNumber(int *row){
    for (int i=0; i<size; i++) {
        if(row[i]!=1 && row[i]!=0){
            return row[i]-2;
        }
    }
    return -1;
}
queue<string> exact::getSet(){
	return sets;
}
void exact::run()
{
	topologicalSort();
    makeDagTopologic();
    treeTraversal(0, true);
    treeTraversal(0, false);
}
