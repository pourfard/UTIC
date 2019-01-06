//
//  File: main.cpp
//  Project: Exact, Exact main part
//	Author: Ali Jahanshahi
//  Supervisor: Dr. Salehi
//  Copyright (c) 2015 (University of Tehran). All rights reserved.
//


#include <iostream>
#include <string>
#include "integrator.h"
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        cout << "Please Enter Argumans (DFG.gv, Config.conf)";
    }

    string graphFile= argv[1];
	const string strgwmin = "gwmin";
	const string strwmax = "wmax";
    string line;
    int readPorts = 0,writePorts=0;
    bool wmax=false, gwmin=false;
    ifstream ConfigFile;

    ConfigFile.open(argv[2]);

    getline(ConfigFile,line);
    string path = line;

	getline(ConfigFile,line);
	cout << line <<endl;
	if(line==strgwmin)
{
		gwmin = true; wmax = false;
}
	else if(line==strwmax)
{
		wmax=true; gwmin = false;
}
	else
{
		gwmin = true; wmax = false;
}

    getline(ConfigFile, line);
    string configFile = line;

    ConfigFile >> readPorts >> writePorts;

    ConfigFile.close();

    cout << path<<endl;
    cout << readPorts <<" "<<writePorts<<endl;
    cout << wmax << " "<<gwmin<<endl;
cout << configFile <<endl;
	integrator *integ = new integrator(path,configFile,graphFile);
	cout <<"integrator"<<endl;
	integ->findCI(readPorts,writePorts);
		cout <<"find ci"<<endl;
	integ->runSelection(wmax,gwmin);
	cout <<"runselect"<<endl;
	cout <<"finished";
	return 0;
}
