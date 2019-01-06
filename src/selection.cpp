//
//	File : selection.cpp
//  Project: Exact
//	Author: Ali Jahanshahi
//  Supervisor: Dr. Salehi
//  Copyright (c) 2015 (University of Tehran). All rights reserved.
//


#include "selection.h"
#include <string.h>
using namespace std;

#define SEPARATOR "/"

selection::selection(int** dag,string path,queue<string> sets,queue<double> delay,queue<double> area,map<int,string> nodeNames){
	this->DAG=dag;
	this->path=path;
	this->setSize=countSet(sets);		//count number of sets in queue
	importSets(sets);					//changes queue to array of string
	importSetAreas(area);
	importSetDelays(delay);
	this->nodeNames=nodeNames;
}
void selection::runWMAX(){
	caculateMeritWMAX();
	WMAX();
	drawGraph(true);
}
void selection::runGWMIN(){
	caculateMeritGWMIN();
	GWMIN();
	drawGraph(false);
}
void selection::caculateMeritGWMIN(){
	gwminMerit = new double[setSize];
	for(int i=0;i<setSize;i++){
		string set=sets[i];

		//If there was no area for some node, its area would be 1.
		double area= setArea[i]!=0 ? setArea[i] : 1.0;

		//Hardware delay is ceiling of critical path calculated by exact algorithm
		double HWdelay=ceil(setDelay[i]);

		//Software delay is #node in a set
		double SWdelay=sofwareDelay(set);
		gwminMerit[i]=((SWdelay-HWdelay)/area)/(degree(set)+1);
	}
}
int selection::degree(string set){
	int degree=0;
	for(int i=0;i<setSize;i++){
		if(sets[i]!=set){
			string currentSet=sets[i];
			for(int j=0;j<currentSet.size();j++){
				if(set.at(j)=='1' && currentSet.at(j)=='1'){
					degree++;
					break;
				}
			}
		}
	}
	return degree;
}

void selection::drawGraph(bool isWmax){
    string nodeInCut = "[style=\"rounded\",shape=\"doublecircle\",color=\"red\"];\n";
    string normalNode = "[style=\"rounded\",shape=\"circle\"];\n";
    string InOut = "[style=\"rounded\",shape=\"none\"];\n";
	int f=0;
	while( (isWmax && !wmax_CIs.empty()) || (!isWmax && !gwmin_CIs.empty())){
		string fileName;
		string set;
		if(isWmax==true){
			set = wmax_CIs.front();
			wmax_CIs.pop();
			fileName=path+SEPARATOR+"WMAX_CI_"+std::to_string(f)+".dot";
		}else{
			set = gwmin_CIs.front();
			gwmin_CIs.pop();
			fileName=path+SEPARATOR+"GWMIN_CI_"+to_string(f)+".dot";
		}
		ofstream file;
		file.open(fileName);
		file<<"digraph{\n";
		for (int i=0; i<set.size(); i++) {
			string node=nodeNames[i+1]+" ";
			if (nodeNames[i+1].find("Input")<=nodeNames[i+1].size() || nodeNames[i+1].find("Output")<=nodeNames[i+1].size()) {
				node+=InOut;
			}
			else if (set.at(i)=='1') {
				node+=nodeInCut;
			}
			else{
				node+=normalNode;
			}
			file<<node;
		}
		for (int i=0; i<set.size(); i++) {
			for (int j=0; j<set.size(); j++) {
				string edges="";
				if (DAG[i][j]==1) {
					edges+= nodeNames[i+1]+" -> "+nodeNames[j+1]+";\n";
				}
				file<<edges;
			}
		}
		file<<"}";
		file.close();
		string batchPath=fileName.substr(fileName.find_last_of("/")+1,fileName.size());
		batchPath=fileName.substr(0,fileName.find_last_of("/"))+batchPath;
		/*file.open(batchPath+".bat");
		file<<"\""+GVPath+SEPARATOR+"bin"+SEPARATOR+"dot.exe\" -Tpng \""+fileName+"\" -o \""+fileName+".png\"\ndel \""+fileName+"\"";
		file.close();*/
		//string cmd="\""+batchPath+".bat\"";
		cout <<"salam";
		//excuteBatch(cmd);
		//cmd="del \""+batchPath+".bat\"";
		//excuteBatch(cmd);
		f++;

	}
}
void selection::GWMIN(){
	string* tmp = new string[setSize];
	for(int i=0;i<setSize;i++)tmp[i]=sets[i];
	bool finished=true;
	while(finished==true){
		int max = -1;
		int index;
		finished=false;
		for(int i=0;i<setSize;i++){
			if(tmp[i].size()!=0){
				finished=true;
				if(gwminMerit[i]>max){
					max=gwminMerit[i];
					index=i;
				}
			}
		}
		if(finished==true){
			insertToGWminSet(tmp,index);
		}
	}
}
void selection::WMAX(){
	string* tmp = new string[setSize];
	for(int i=0;i<setSize;i++)tmp[i]=sets[i];
	bool finished=true;
	while(finished==true){
		int max = -1;
		int index;
		finished=false;
		for(int i=0;i<setSize;i++){
			if(tmp[i].size()!=0){
				finished=true;
				if(wmaxMerit[i]>max){
					max=wmaxMerit[i];
					index=i;
				}
			}
		}
		if(finished==true){
			insertToWmaxSet(tmp,index);
		}
	}
}
void selection::insertToWmaxSet(string* tmp,int index){
	string maxSet=tmp[index];
	for(int i=0;i<setSize;i++){
		if(i!=index && tmp[i]!=""){
			string currentSet=tmp[i];
			for(int j=0;j<currentSet.size();j++){
				if(maxSet.at(j)=='1' && currentSet.at(j)=='1'){
					tmp[i]="";
					break;
				}
			}
		}
	}
	wmax_CIs.push(maxSet);
	tmp[index]="";
}
void selection::insertToGWminSet(string* tmp,int index){
	string maxSet=tmp[index];
	for(int i=0;i<setSize;i++){
		if(i!=index && tmp[i]!=""){
			string currentSet=tmp[i];
			for(int j=0;j<currentSet.size();j++){
				if(maxSet.at(j)=='1' && currentSet.at(j)=='1'){
					tmp[i]="";
					break;
				}
			}
		}
	}
	gwmin_CIs.push(maxSet);
	tmp[index]="";
}

void selection::importSetAreas(queue<double> area){
	this->setArea=new double[setSize];
	queue<double> s=area;
	int f=0;
	while(!s.empty()){
		this->setArea[f]=s.front();
		s.pop();
		f++;
	}
}
void selection::importSetDelays(queue<double> delay){
	this->setDelay=new double[setSize];
	queue<double> s=delay;
	int f=0;
	while(!s.empty()){
		this->setDelay[f]=s.front();
		s.pop();
		f++;
	}
}
double selection::sofwareDelay(string set){
	double swd=0.0;
	for(int i=0;i<set.size();i++){
		if(set.at(i)=='1')swd++;
	}
	return swd;
}
void selection::caculateMeritWMAX(){
	wmaxMerit = new double[setSize];
	for(int i=0;i<setSize;i++){
		string set=sets[i];

		//If there was no area for some node, its area would be 1.
		double area= setArea[i]!=0 ? setArea[i] : 1.0;

		//Hardware delay is ceiling of critical path calculated by exact algorithm
		double HWdelay=ceil(setDelay[i]);

		//Software delay is #node in a set
		double SWdelay=sofwareDelay(set);
		wmaxMerit[i]=(SWdelay-HWdelay)/area;
	}
}
int selection::countSet(queue<string> sets){
	int f=0;
	queue<string> s=sets;
	while(!s.empty()){
		s.pop();
		f++;
	}
	return f;
}
void selection::importSets(queue<string> sets){
	this->sets=new string[setSize];
	queue<string> s=sets;
	int f=0;
	while(!s.empty()){
		this->sets[f]=s.front();
		s.pop();
		f++;
	}
}
