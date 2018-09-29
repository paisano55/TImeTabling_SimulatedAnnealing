#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <vector>
#include <time.h>

int timeTable[5][9][2] = { 0 }; // 0 : 할당된 그룹없음 , -1 : 수업이 없는 시간 (연구활동, 방과후 etc.)
int timeTableSrc[5][9][2] = { 0 }; // Save the code of group
int fitNow, fitSrc;

int main(void)
{
	struct subjectBase
	{
		vector<int> student;
		int teacher, subcode, floor, position, time;
	};
	struct groupBase
	{
		vector <subjectBase> subject;
		groupCode;
		maxTime;
		vector <int> student;
		vector <int> teacher;
	};
	vector <subject> group;
	int L, looped, fit
	double T = 100, dec;
	initialize();
	initTimeTable(); //create initial timetable
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
int checking(int src)
{
	int chkTimeTable[5][9][2];
	int chk = 1;
	if (src)
	{
		chkTimeTable[5][9][2] = timeTableSrc[5][9][2];
	}
	else
	{
		chkTimeTable[5][9][2] = timeTable[5][9][2];
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; ( chkTimeTable[i][j][0]!= 0 && chkTimeTable[i][j][0] != 0 ) && (chkTimeTable[i][j][1] != 0 && chkTimeTable[i][j][1] != 0); j++)
		{
			
		}
	}
}
void search()
{

}
int initTimeTable()
{

}
void initialize()
{
	timeTable[0][7][0], timeTable[0][7][1] = -1;
	timeTable[2][5][0], timeTable[2][5][1], timeTable[2][6][0], timeTable[2][6][1], timeTable[2][7][0], timeTable[2][7][1], timeTable[2][8][0], timeTable[2][8][1] = -1;
	timeTable[4][5][0], timeTable[4][5][1], timeTable[4][6][0], timeTable[4][6][1], timeTable[4][7][0], timeTable[4][7][1] = -1;	
	

}