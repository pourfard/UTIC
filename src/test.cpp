//
//  File: test.cpp , this is just a test file
//  Project: Exact
//	Author: Mahdi Mohammad Pour Fard
//  Supervisor: Dr. Salehi
//  Copyright (c) 2015 (University of Tehran). All rights reserved.
//


#include <stdio.h>

int main()
{
	int t1,t2,t3,t4,t5,i1,i2,i3,i4,i5;
	t1 = i1 * i2;
	t2 = i3 | i4;
	t3 = t1 + t2;
	t4 = t3 & t2;
	t5 = t2 - i5;

	if(t1 == 10)
	{
	    t3 = i1 * i2;
        t2 = t1 | i4;
        t5 = t1 + t2;

        if(t3==12)
        {
            t3 = 30 * t1;
            t2 = t1 + t5;
            t3 = i1 * i2;
            t2 = t1 | i4;
            t1 = 48 * t2;
            t2 = t5 + t4;
            t5 = t1 + t2;
            t1 = 48 * t2;
            t2 = t5 + t4;
            t4 = 36 * t2;
            t5 = t1 + t4;
        }
        else if(t3==50)
        {
            t3 = 30 * t1;
            t2 = t1 + t5;
            t3 = i1 * i2;
            t2 = t1 | i4;
            t5 = t1 + t2;
        }
        else if(t3==40)
        {
            t1 = 23 * t2;
            t2 = t5 + t4;
            t4 = 12 * t2;
            t5 = t1 + t4;
            t1 = 48 * t2;
            t2 = t5 + t4;
        }

        for(int i=0;i<40;i++)
        {
            t1 = i1 * i2;
            t2 = i3 | i4;
            t3 = t1 + t2;
            t4 = t3 & t2;
        }

	}
	else
	{
	    t3 = 30 * t1;
        t2 = t1 + t5;
        t1 = 48 * t2;
        t2 = t5 + t4;
        t4 = 36 * t2;
        t5 = t1 + t4;

        for(int i=0;i<40;i++)
        {
            t2 = i3 | i4;
            t3 = t1 + t2;
            t1 = 48 * t2;
            t2 = t5 + t4;
            t4 = t3 & t2;
            t5 = t2 - i5;
        }
	}

	return 0;
}
