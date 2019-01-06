//
//  File: Parser.cpp
//  Project: Exact, Parser main part
//	Author: Mahdi Mohammad Pour Fard
//  Supervisor: Dr. Salehi
//  Copyright (c) 2015 (University of Tehran). All rights reserved.
//


#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct BasicBlock
{
	int count;
	string line[100];
};

string startwith(string line,int count)
{
	return line.substr(0,count);
}

string endwith(string line,int count)
{
	return line.substr(line.length()-count,count);
}

bool checkbbEnd(string line)
{
	if(startwith(line,6)=="  if (")
		return true;
	else if(startwith(line,7)=="  goto ")
		return true;
	else if(startwith(line,9)=="  return ")
		return true;
	else if(line=="")
		return true;

	return false;
}


bool checkloadstor(string line)
{
	return true;
}

struct linevar
{
	int count;
	string variable[10];
};

linevar spliteLine(string line)
{
	string variable = "";
	linevar result;
	result.count = 0;
	int start = 0;
	while(true)
	{
		if(line[start]==' ')
			start++;
		else
			break;
	}

	for(int i = start;i<line.length();i++)
	{
		if(line[i]==' ' || line[i]==';')
		{
			result.variable[result.count] = variable;
			result.count++;
			variable="";
			continue;
		}
		variable = variable +line[i];
	}

	return result;
}

bool isnumber(string str)
{

	bool result = true;
	for(int i=0;i<str.length();i++)
	{
	    char ch = str[i];
	    if((ch >=48 && ch <= 57) || (ch=='-'));
	    else
	    {
	        result = false;
	        break;
	    }
	}

	return result;
}

struct leftside
{
	int number;
	string var[100];
	string op[100];
};

struct nodes
{
	string in1,in2,op;
	int opType;//0 arithmatic,1 load, 2 store, 3 mov
	bool isOut, isLast,isIn1,isIn2;
};

struct CustomBB
{
	int nodeNumber;
	nodes Nodes[100];
};

BasicBlock bbs[100];
int bbnumber = 0;

bool isOut(string var, int bbnum)
{
	for(int i =0;i<bbnumber;i++)
	{
		if(i==bbnum)
			continue;

		for(int j = 0;j<bbs[i].count;j++)
		{
			linevar myvar = spliteLine(bbs[i].line[j]);
			if(myvar.variable[2]==var || myvar.variable[4]==var)
				return true;
		}
	}

	return false;
}
bool isLast(string var,int bbnum,int line)
{
	for(int i =line+1;i<bbs[bbnum].count;i++)
	{
		linevar myline = spliteLine(bbs[bbnum].line[i]);
		if(myline.variable[0]== var)
			return false;
	}

	return true;
}
CustomBB mybb[100];
int bbnode = 0;

void createDotty()
{
	ofstream output;
	string styleNode = "[style=\"rounded\",shape=\"ellipse\"];";
	string styleInOut = "[style=\"rounded\",shape=\"none\"];";
	int outputs = 0;

	ifstream infile;
	infile.open("bit.conf");
	string path;
	getline(infile,path);
	infile.close();

	char cmdChar[200];
	string cmd="mkdir BasicBlocks\n";
	for(int i=0;i<cmd.size();i++)cmdChar[i]=cmd.at(i);
	cmdChar[cmd.size()] = '\0';
	system(cmdChar);

	cmdChar[200];
	cmd="mkdir "+path+"\n";
	for(int i=0;i<cmd.size();i++)cmdChar[i]=cmd.at(i);
	cmdChar[cmd.size()] = '\0';
	system(cmdChar);

	cmdChar[200];
	cmd="mkdir "+path+"/BasicBlocks\n";
	for(int i=0;i<cmd.size();i++)cmdChar[i]=cmd.at(i);
	cmdChar[cmd.size()] = '\0';
	system(cmdChar);

	for(int i =0;i<bbnode;i++)
	{
		output.open("BasicBlocks/bb"+to_string(i)+".gv");
		output <<"digraph mygraph {\n";
		outputs = 0;

		for(int j =0; j<mybb[i].nodeNumber;j++)
		{
			nodes mynode = mybb[i].Nodes[j];
			if(mynode.opType==0)
			{
			    output <<"\""+mynode.op+"\" "<<styleNode<<endl;
                if(mynode.isIn1)
                    output <<"\""+mynode.in1+"\" "<<styleInOut<<endl;
                if(mynode.isIn2)
                    output <<"\""+mynode.in2+"\" "<<styleInOut<<endl;
                output <<"\""+mynode.in1+"\" -> \""<<mynode.op<<"\";"<<endl;

                output <<"\""+mynode.in2+"\" -> \""<<mynode.op<<"\";"<<endl;

                bool isUsed = false;

                for(int k = j+1;k<mybb[i].nodeNumber;k++)
                {
                    nodes another = mybb[i].Nodes[k];
                    if(another.in1 == mynode.op || another.in2 == mynode.op)
                        isUsed = true;
                }

                if((mynode.isOut==true) && (mynode.isLast==true) || (isUsed == false))
                {
                    output <<"\"Out"+to_string(outputs)+"\" "<<styleInOut<<endl;
                    output <<"\""+mynode.op+"\" -> \"Out"+to_string( outputs)+"\";"<<endl;
                    outputs++;
                }
			}
			else
			{
			    output <<"\""+mynode.op+"\" "<<styleNode<<endl;

			    if(mynode.isIn1)
			    {
                    output <<"\""+mynode.in1+"\" "<<styleInOut<<endl;
			    }

			    bool isUsed = false;

                for(int k = j+1;k<mybb[i].nodeNumber;k++)
                {
                    nodes another = mybb[i].Nodes[k];
                    if(another.in1 == mynode.op || another.in2 == mynode.op)
                        isUsed = true;
                }

                output <<"\"Out"+to_string(outputs)+"\" "<<styleInOut<<endl;
                output <<"\""+mynode.in1+"\" -> \""+mynode.op+"\";"<<endl;
                output <<"\""+mynode.op+"\" -> \""+mynode.in2+"\";"<<endl;

                if((mynode.isOut==true) && (mynode.isLast==true) || (isUsed == false))
                {
                    output <<"\"Out"+to_string(outputs)+"\" "<<styleInOut<<endl;
                    output <<"\""+mynode.in2+"\" -> \"Out"+to_string( outputs)+"\";"<<endl;
                    outputs++;
                }
			}

		}
		output <<"}";
		output.close();

		char cmdChar[200];
		string cmd="./exactui BasicBlocks/bb"+to_string(i)+".gv bit.conf\n";
		for(int i=0;i<cmd.size();i++)cmdChar[i]=cmd.at(i);
		cmdChar[cmd.size()] = '\0';
		system(cmdChar);
	}
}
int main(int argc,char *argv[])
{
	if(argc != 2)
	{
		cout << "Please decleare inputfile as an argument";
		return 0;
	}

	string path = argv[1];
	string line;

	ifstream infile;

	infile.open(path);

	for(int i = 0; i< 100; i++)
	{
		bbs[i].count =0;
	}

    //in this loop we read the input and extract the basic blocks
	while(!infile.eof())
	{
		getline(infile,line);
		cout <<line<<endl;
		if(startwith(line,5)=="  <bb" && endwith(line,2)==">:")
		{
			cout <<line<<endl;
			int linecount = 0;
			while(true)
			{
				getline(infile,line);
				//cout <<startwith(line,5);
				if(checkbbEnd(line)==true)
				{
					cout <<"--------------\n";
					break;
				}
				linevar linev = spliteLine(line);
				if(linev.count == 5 && startwith(linev.variable[2],1)!="(")
				{
					cout <<line<<endl;
					bbs[bbnumber].line[linecount] = line;
					bbs[bbnumber].count++;
					linecount++;
				}
				else if(linev.count == 3)
				{
					//load instruction
					if(startwith(linev.variable[0],1)=="*")
					{
						cout <<line<<endl;
						bbs[bbnumber].line[linecount] = line;
						bbs[bbnumber].count++;
						linecount++;
					}
					//store instruction
					else if (startwith(linev.variable[2],1)=="*")
					{
						cout <<line<<endl;
						bbs[bbnumber].line[linecount] = line;
						bbs[bbnumber].count++;
						linecount++;
					}
					else if(isnumber(linev.variable[2]))
                    {
                        cout <<line<<endl;
						bbs[bbnumber].line[linecount] = line;
						bbs[bbnumber].count++;
						linecount++;
                    }
				}
			}
			bbnumber++;
		}
	}
	cout <<"finish " << bbnumber <<endl;
	infile.close();

    //here we just print out ther basic blocks
    //there are bbnumer basic block(s) in ther bbs array
    //each bbs[i] has several line (node)
	for(int i =0;i<bbnumber;i++)
	{
		for(int j = 0;j<bbs[i].count;j++)
		{
			cout << bbs[i].line[j] <<endl;
		}
		cout << "---------"<<i<<"---------"<<endl;
	}

	/*--------------------------------------*/
	leftside myleft;
	myleft.number = 0;


	for(int i =0;i<100;i++)
	{
		mybb[i].nodeNumber = 0;
	}

	nodes mynode;
	for(int i =0;i<bbnumber;i++)
	{

		int constants = 0;
		int operations = 0;
		myleft.number = 0;

		if(bbs[i].count==0)
			continue;

		for(int j = 0;j<bbs[i].count;j++)
		{
			linevar myline = spliteLine(bbs[i].line[j]);

            if(myline.count == 5)
            {
                //check is out
                mynode.isOut = isOut(myline.variable[0],i);
                mynode.isLast = isLast(myline.variable[0],i,j);
                mynode.op = myline.variable[3]+"o"+to_string(operations);

                mynode.isIn1=mynode.isIn2 = true;

                for(int k = 0;k<myleft.number;k++)
                {
                    if(myleft.var[k] == myline.variable[2])
                    {
                        cout <<"before " <<myline.variable[2]<<endl;
                        myline.variable[2] = myleft.op[k];
                        cout <<"after " <<myline.variable[2]<<endl;
                        mynode.isIn1 = false;
                    }

                    if(myleft.var[k] == myline.variable[4])
                    {
                        myline.variable[4] = myleft.op[k];
                        mynode.isIn2 = false;
                    }
                }

                if(isnumber(myline.variable[2]))
                {
                    mynode.in1 = "C"+to_string(constants);
                    constants++;
                }

                else
                    mynode.in1 = myline.variable[2];

                if(isnumber(myline.variable[4]))
                {
                    mynode.in2 = "C"+to_string(constants);
                    constants++;
                }

                else
                    mynode.in2 = myline.variable[4];

                cout << mynode.in1 <<endl;
                mynode.opType = 0;

                myleft.var[myleft.number] = myline.variable[0];
                myleft.op[myleft.number] = mynode.op;
                myleft.number++;

            }
			else if(myline.count == 3)
            {
                //store instruction
                if(startwith(myline.variable[0],1)=="*")
                {
                    mynode.in2 = myline.variable[0].substr(1,myline.variable[0].length()-1);
                    mynode.op = "st"+to_string(myleft.number);
                    mynode.opType = 2;
                    mynode.in1 = myline.variable[2];

                    //check is out
                    mynode.isOut = isOut(mynode.in2,i);
                    mynode.isLast = isLast(mynode.in2,i,j);

                    mynode.isIn1= true;
                    mynode.isIn2= false;

                    for(int k = 0;k<myleft.number;k++)
                    {
                        if(myleft.var[k] == mynode.in1)
                        {
                            mynode.in1 = myleft.op[k];
                            mynode.isIn1 = false;
                        }
                    }

                    myleft.var[myleft.number] = mynode.in2;
                    myleft.op[myleft.number] = mynode.in2;
                    myleft.number++;

                }
                //load instruction
                else if (startwith(myline.variable[2],1)=="*")
                {
                    mynode.in1 = myline.variable[2].substr(1,myline.variable[2].length()-1);
                    mynode.op = "ld"+to_string(myleft.number);
                    mynode.opType = 1;
                    mynode.in2 = myline.variable[0];

                    //check is out
                    mynode.isOut = isOut(mynode.in2,i);
                    mynode.isLast = isLast(mynode.in2,i,j);

                    mynode.isIn1= true;
                    mynode.isIn2= false;

                    for(int k = 0;k<myleft.number;k++)
                    {
                        if(myleft.var[k] == mynode.in1)
                        {
                            mynode.in1 = myleft.op[k];
                            mynode.isIn1 = false;
                        }
                    }

                    myleft.var[myleft.number] = mynode.in2;
                    myleft.op[myleft.number] = mynode.in2;
                    myleft.number++;

                }
                //move instruction
                else if(isnumber(myline.variable[2]))
                {
                    mynode.in1 = myline.variable[2];
                    mynode.op = "mov"+to_string(myleft.number);
                    mynode.opType = 3;
                    mynode.in2 = myline.variable[0];

                    if(isnumber(mynode.in1))
                    {
                        mynode.in1 = "C"+to_string(constants);
                        constants++;
                    }
                    //check is out
                    mynode.isOut = isOut(mynode.in2,i);
                    mynode.isLast = isLast(mynode.in2,i,j);

                    mynode.isIn1= true;
                    mynode.isIn2= false;

                    for(int k = 0;k<myleft.number;k++)
                    {
                        if(myleft.var[k] == mynode.in1)
                        {
                            mynode.in1 = myleft.op[k];
                            mynode.isIn1 = false;
                        }
                    }
                    myleft.var[myleft.number] = mynode.in2;
                    myleft.op[myleft.number] = mynode.in2;
                    myleft.number++;

                }

            }

            operations++;
            constants++;
            mybb[bbnode].Nodes[j] = mynode;

		}

		mybb[bbnode++].nodeNumber = bbs[i].count;
	}

	for(int i =0;i<bbnode;i++)
	{
		cout <<"-------BB"<<i<<"-------"<<endl;
		for(int j=0;j<mybb[i].nodeNumber;j++)
		{
			cout << mybb[i].Nodes[j].in1 << " "<<mybb[i].Nodes[j].op<<" "<< mybb[i].Nodes[j].in2<<endl;
			cout << mybb[i].Nodes[j].isLast << " " << mybb[i].Nodes[j].isOut<<" "<< mybb[i].Nodes[j].isIn1<<" "<<mybb[i].Nodes[j].isIn2<<endl;
		}
	}
	createDotty();
	return 0;
}
