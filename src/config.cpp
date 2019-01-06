//
//  File: config.cpp
//  Project: Exact
//	Author: Ali Jahanshahi
//  Supervisor: Dr. Salehi
//  Copyright (c) 2015 (University of Tehran). All rights reserved.
//

#include <cstdlib>
#include "config.h"
#include <fstream>
#include <string.h>
using namespace std;

void config::extract(){
    ifstream f;
    string line;
    f.open(file.c_str());
    getline (f,line);
    getline (f,line);
    while(!f.eof()){ 
        if (line.size()<4) {
            cerr<<"Something is wrong in config file format!\n";
            continue;
        }
        string::size_type pos1,pos2;
        pos1 = line.find(" ",1);									//finds first spase
        pos2 = line.find(" ",line.find(" ")+1);						//finds second space
        if (pos1>=pos2 || pos1>line.size() || pos2>line.size()) {	//check for format
            cerr<<"Something is wrong in config file format!\n";
            continue;
        }
        string op = line.substr(0,pos1);							//extract node name
        double d = atof( line.substr(pos1+1, pos2-2).c_str());		//extract delay, convert to double
        double a = atof( line.substr(pos2+1,line.size()).c_str());	//extract area, convert to double
		//put in the map
        delay[op]=d;
	cout <<"opdelay: "<<op <<" " << d << endl;
        area[op]=a;
	getline (f,line);
    }
}
