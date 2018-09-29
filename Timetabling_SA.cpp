#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

int timeTable[5][9][2];
int timeTableSrc[5][9][2]; // Save the code of group
int fitNow, fitSrc;

int main(void)
{
	struct subject
	{
		vector<int> student;
		int teacher, subcode, floor, position, time;
	};
	vector <subject> group;
	int L,looped,fit
	double T=100,dec
	initial(); //create initial timetable
	fitNow=fitness(0)

	while(T>0.01)
	{
		for (looped=0; looped <= L; looped++)
		{
			fit=fitness(search())
		}
		T *= dec;
	}
}
int fitness(int src) // src=0 : 현재해 적합도 src=1 이웃해 적합도
{
	int fitTimeTable[5][9][2];
	int fit = 0;
	if (src)
	{
		fitTimeTable[5][9][2] = timeTableSrc[5][9][2];
	}
	else
	{
		fitTimeTable[5][9][2] = timeTable[5][9][2];
	}
}
int checking()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			
		}
	}
}
void search()
{

}
int initial()
{

}