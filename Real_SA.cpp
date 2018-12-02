#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <memory.h>

using namespace std;
// 이 아래로는 자료형이다.
int timeTable[5][9][2]; // 그룹번호를 담고있는 시간표 배열, 이 아래는 이웃해 탐색용
int timeTableSrc[5][9][2];
typedef struct subjectbase // 과목의 정보를 담고 있는 구조체
{
	vector<int> student; // 학생들의 코드를 담고 있는 벡터
	int floor, x, time, y, grade; // 좌측부터 과목코드, 층, x좌표, 시수, y좌표, 학년
	vector<int> teacher; // 교사 코드
	int sequent; // 연강 시간
	int subcode;

}subjectBase;
typedef struct groupbase // 그룹의 정보를 담고 있는 구조체 
{
	vector<subjectBase> subject; // 그룹에 포함된 과목들의 번호
	int maxTime; // 포함된 과목 중 가장 시수가 많은 과목의 시수
	int currentTime; // 현재 시간표에 할당된 시수
	int grade; // 그룹내 수업의 학년
	int seqTime; // 최대 연강 시수
	vector<int> student; // 그룹에 포함된 모든 과목의 학생들
	vector<int> teacher; // 그룹에 포함된 모든 교사들
}groupBase;
typedef struct timebase // 각 그룹들이 할당되있는 시간을 담기 위한 구조체 
{
	int day, period, selected; // 좌측부터 요일, 교시, 선택여부 (2차 Simulated Annealing에서 사용)
}timeBase;

vector<groupBase> group; // 그룹 구조체들의 데이터를 담을 벡터를 선언 ( 기초 데이터 )
vector<subjectBase> subjectData; // 과목들의 데이터를 담을 벡터를 선언 ( 기초 데이터 )
vector<timeBase> timeData[200]; // 과목들이 할당되있는 시간들을 담을 벡터를 선언 ( 1차 SA 후 도출 )
vector<int> subTable[5][9][2]; // 2차 SA때 사용될 과목들이 담긴 과목번호들을 담는 3차원배열 벡터
vector<int> subTableSrc[5][9][2]; // 2차 SA때 이웃해로 사용될 3차원배열 벡터
//vector<int>[5][9][2] &a;
int impTime[60][5][9] = { 0 }; // 교사들의 수업 불가능 시간을 담고 있는 배열 ( 기초 데이터 )

FILE *fout = fopen("saoutput.txt", "w");
FILE *fin = fopen("sainput.txt", "r");

int DistCal(int dowi, int periodi, int dowf, int periodf)
{
	int i, j, k, moving, temp1, temp2;
	int stuInfo;
	int howmansub = 0, howmanstu, whichsmall;
	int howmuctime[60] = { 0 };
	int totaltime = 0;
	vector <int> student_list;
	vector <int> iniclass_student;
	vector <int> finclass_student;
	typedef struct positionBase
	{
		int floor, x, y;
	}position;
	vector <position> initial_position;
	vector <position> final_position;
	vector <position> stair_location;
	for (i = 0; i < 2; i++)
	{
		howmansub += group[timeTable[dowi][periodi][i]].subject.size();
	}
	for (i = 0; i < howmansub; i++)
	{
		for (j = 0; j < 2; j++)
		{
			howmanstu += group[timeTable[dowi][periodi][j]].subject[i].student.size();
		}
		whichsmall = howmanstu;
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				stuInfo = group[timeTable[dowi][periodi][k]].subject[i].student[j];
				iniclass_student.push_back(stuInfo);
			}
		}
		howmanstu = 0; //초기화
		for (j = 0; j < 2; j++)
		{
			howmanstu += group[timeTable[dowf][periodf][k]].subject[i].student.size();
		}
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				stuInfo = group[timeTable[dowf][periodf][k]].subject[i].student[j];
				finclass_student.push_back(stuInfo);
			}
		}
		if (whichsmall < howmanstu) howmanstu = whichsmall;

		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < howmanstu; k++)
			{
				if (iniclass_student[j] == finclass_student[k])
				{
					student_list.push_back(finclass_student[k]);
				}
			}
		}
	}
	stair_location[0].x = 0; stair_location[0].y = 0;
	stair_location[1].x = 5; stair_location[1].y = 1;
	stair_location[1].x = -7; stair_location[1].y = 1;
	stair_location[1].x = -3; stair_location[1].y = 5;
	stair_location[1].x = -2; stair_location[1].y = -4;
	for (i = 0; i < student_list.size(); i++)
	{
		for (j = 0; j < 2; j++)
		{

			initial_position[i].floor = group[timeTable[dowi][periodi][j]].subject[i].floor;
			initial_position[i].x = group[timeTable[dowi][periodi][j]].subject[i].x;
			initial_position[i].y = group[timeTable[dowi][periodi][j]].subject[i].y;//학생들의 초기 위치 저장 
			final_position[i].floor = group[timeTableSrc[dowf][periodf][j]].subject[i].floor;
			final_position[i].x = group[timeTable[dowf][periodf][j]].subject[i].x;
			final_position[i].y = group[timeTable[dowf][periodf][j]].subject[i].y;//학생들의 이동 위치 저장 
			totaltime += (initial_position[i].floor - final_position[i].floor) * 5;//층간 이동 가중치는 5
			moving = 100;
			for (k = 0; k < 5; k++)
			{
				temp1 = abs(stair_location[k].x - initial_position[i].x) + abs(stair_location[k].y - initial_position[i].y);
				if (moving > temp1) moving = temp1;
				totaltime += moving;
				moving = 100;
				temp2 = abs(stair_location[k].x - final_position[i].x) + abs(stair_location[k].y - final_position[i].y);
				if (moving > temp2) moving = temp2;
				totaltime += moving;
			}
		}
	}
	return totaltime;
}

int DowTotDist()
{
	int Dtotaltime = 0;
	int dow, period;
	for (dow = 0; dow < 5; dow++)
	{
		for (period = 0; period < 8; period++)
		{
			Dtotaltime += DistCal(dow, period, dow, period + 1);
		}
	}
	return Dtotaltime;
}
int SrcDistCal(int dowi, int periodi, int dowf, int periodf)
{
	int i, j, k, moving, temp1, temp2;
	int stuInfo;
	int howmansub = 0, howmanstu, whichsmall;
	int howmuctime[60] = { 0 };
	int totaltime = 0;
	vector <int> student_list;
	vector <int> iniclass_student;
	vector <int> finclass_student;
	struct position
	{
		int floor, x, y;
	};
	vector <position> initial_position;
	vector <position> final_position;
	vector <position> stair_location;
	for (i = 0; i < 2; i++)
	{
		howmansub += group[timeTableSrc[dowi][periodi][i]].subject.size();
	}
	for (i = 0; i < howmansub; i++)
	{
		for (j = 0; j < 2; j++)
		{
			howmanstu += group[timeTableSrc[dowi][periodi][j]].subject[i].student.size();
		}
		whichsmall = howmanstu;
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				stuInfo = group[timeTableSrc[dowi][periodi][k]].subject[i].student[j];
				iniclass_student.push_back(stuInfo);
			}
		}
		howmanstu = 0; //초기화
		for (j = 0; j < 2; j++)
		{
			howmanstu += group[timeTableSrc[dowf][periodf][k]].subject[i].student.size();
		}
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				stuInfo = group[timeTableSrc[dowf][periodf][k]].subject[i].student[j];
				finclass_student.push_back(stuInfo);
			}
		}
		if (whichsmall < howmanstu) howmanstu = whichsmall;

		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < howmanstu; k++)
			{
				if (iniclass_student[j] == finclass_student[k])
				{
					student_list.push_back(finclass_student[k]);
				}
			}
		}
	}
	stair_location[0].x = 0; stair_location[0].y = 0;
	stair_location[1].x = 5; stair_location[1].y = 1;
	stair_location[1].x = -7; stair_location[1].y = 1;
	stair_location[1].x = -3; stair_location[1].y = 5;
	stair_location[1].x = -2; stair_location[1].y = -4;
	for (i = 0; i < student_list.size(); i++)
	{
		for (j = 0; j < 2; j++)
		{

			initial_position[i].floor = group[timeTableSrc[dowi][periodi][j]].subject[i].floor;
			initial_position[i].x = group[timeTableSrc[dowi][periodi][j]].subject[i].x;
			initial_position[i].y = group[timeTableSrc[dowi][periodi][j]].subject[i].y;//학생들의 초기 위치 저장 
			final_position[i].floor = group[timeTableSrc[dowf][periodf][j]].subject[i].floor;
			final_position[i].x = group[timeTableSrc[dowf][periodf][j]].subject[i].x;
			final_position[i].y = group[timeTableSrc[dowf][periodf][j]].subject[i].y;//학생들의 이동 위치 저장 
			totaltime += (initial_position[i].floor - final_position[i].floor) * 5;//층간 이동 가중치는 5
			moving = 100;
			for (k = 0; k < 5; k++)
			{
				temp1 = abs(stair_location[k].x - initial_position[i].x) + abs(stair_location[k].y - initial_position[i].y);
				if (moving > temp1) moving = temp1;
				totaltime += moving;
				moving = 100;
				temp2 = abs(stair_location[k].x - final_position[i].x) + abs(stair_location[k].y - final_position[i].y);
				if (moving > temp2) moving = temp2;
				totaltime += moving;
			}
		}
	}
	return totaltime;
}

int SrcDowTotDist() // SA 이웃해용 총 거리계산
{
	int Dtotaltime = 0;
	int dow, period;
	for (dow = 0; dow < 5; dow++)
	{
		for (period = 0; period < 8; period++)
		{
			Dtotaltime += DistCal(dow, period, dow, period + 1);
		}
	}
	return Dtotaltime;
}
int FitnessCal() // 시간표의 적합도 합산 계산
{
	int period1Seq, period9Seq, totDist, fitness;
	for (int day = 0; day <= 3; day++) // 여기서부터 아래 4개의 for문은 1교시/9교시 연강을 확인한다
	{
		for (int i = 0; i < group[timeTable[day][1][0]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTable[day + 1][1][0]].teacher.size(); j++)
			{
				if (group[timeTable[day][1][0]].teacher[i] == group[timeTable[day + 1][1][0]].teacher[j])
				{
					period1Seq++;
				}
			}
		}
		for (int i = 0; i < group[timeTable[day][1][1]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTable[day + 1][1][1]].teacher.size(); j++)
			{
				if (group[timeTable[day][1][1]].teacher[i] == group[timeTable[day + 1][1][1]].teacher[j])
				{
					period1Seq++;
				}
			}
		}
		for (int i = 0; i < group[timeTable[day][9][1]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTable[day + 1][9][1]].teacher.size(); j++)
			{
				if (group[timeTable[day][9][1]].teacher[i] == group[timeTable[day + 1][9][1]].teacher[j])
				{
					period9Seq++;
				}
			}
		}
		for (int i = 0; i < group[timeTable[day][9][0]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTable[day + 1][9][0]].teacher.size(); j++)
			{
				if (group[timeTable[day][9][0]].teacher[i] == group[timeTable[day + 1][9][0]].teacher[j])
				{
					period9Seq++;
				}
			}
		}
	}
	totDist = DowTotDist();
	fitness = totDist + period1Seq * 100 + period9Seq * 100;
	return fitness;
}
int SrcFitnessCal() // 이웃해 시간표의 적합도 합산 계산
{
	int period1Seq, period9Seq, totDist, fitness;
	for (int day = 0; day <= 3; day++) // 여기서부터 아래 4개의 for문은 1교시/9교시 연강을 확인한다
	{
		for (int i = 0; i < group[timeTableSrc[day][1][1]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTableSrc[day + 1][1][1]].teacher.size(); j++)
			{
				if (group[timeTableSrc[day][1][1]].teacher[i] == group[timeTableSrc[day + 1][1][1]].teacher[j])
				{
					period1Seq++;
				}
			}
		}
		for (int i = 0; i < group[timeTableSrc[day][1][0]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTableSrc[day + 1][1][0]].teacher.size(); j++)
			{
				if (group[timeTableSrc[day][1][0]].teacher[i] == group[timeTableSrc[day + 1][1][0]].teacher[j])
				{
					period1Seq++;
				}
			}
		}
		for (int i = 0; i < group[timeTableSrc[day][9][1]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTableSrc[day + 1][9][1]].teacher.size(); j++)
			{
				if (group[timeTableSrc[day][9][1]].teacher[i] == group[timeTableSrc[day + 1][9][1]].teacher[j])
				{
					period9Seq++;
				}
			}
		}
		for (int i = 0; i < group[timeTableSrc[day][9][0]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTableSrc[day + 1][9][0]].teacher.size(); j++)
			{
				if (group[timeTableSrc[day][9][0]].teacher[i] == group[timeTableSrc[day + 1][9][0]].teacher[j])
				{
					period9Seq++;
				}
			}
		}
	}
	totDist = SrcDowTotDist();
	fitness = totDist + period1Seq * 100 + period9Seq * 100;
	return fitness;
}

int GrpDuplicate(int grpi, int grpj) // 두 그룹이 같은 시간에 수업이 가능한지 판단! ( 교사 / 학생 / 교실 중복여부 판단 ) Able:1 Disable:-1
{
	for (int i = 0; i < group[grpi].teacher.size(); i++)
	{
		for (int j = 0; j < group[grpj].teacher.size(); j++)
		{
			if (group[grpi].teacher[i] == group[grpj].teacher[j])
				return -1;
		}
	}
	for (int i = 0; i < group[grpi].student.size(); i++)
	{
		for (int j = 0; j < group[grpj].student.size(); j++)
		{
			if (group[grpi].student[i] == group[grpj].student[j])
				return -1;
		}
	}
	for (int i = 0; i < group[grpi].subject.size(); i++)
	{
		for (int j = 0; j < group[grpj].subject.size(); j++)
		{
			if (group[grpi].subject[i].floor == group[grpj].subject[j].floor && group[grpi].subject[i].x == group[grpj].subject[j].y && group[grpi].subject[i].y == group[grpj].subject[j].y)
				return -1;
		}
	}
	for (int i = 0; i < group[grpi].subject.size(); i++)
	{
		for (int j = 0; j < group[grpj].subject.size(); j++)
		{
			if (group[grpi].subject[i].x == group[grpj].subject[j].x && group[grpi].subject[i].y == group[grpj].subject[j].y && group[grpi].subject[i].floor == group[grpj].subject[j].floor)
				return -1;
		}
	}
	return 1;
}
int GrpAbleTime(int grpCode, int day, int period)// 이 그룹이 그 시간에 수업이 가능한지 판단!
{

	for (int i = 0; i < group[grpCode].teacher.size(); i++)
	{
		if (impTime[group[grpCode].teacher[i]][day][period] == 1)
		{
			return -1;
		}
	}
	if (group[grpCode].grade == 2)
	{
		if (period == 0)
		{
			if (grpCode == timeTable[day][period + 1][0] && group[grpCode].seqTime == 1)
				return -1;
		}
		else if (period == 8)
		{
			if (grpCode == timeTable[day][period - 1][0] && group[grpCode].seqTime == 1)
				return -1;
		}
		else
		{
			if (grpCode == timeTable[day][period - 1][0] && group[grpCode].seqTime == 1)
				return -1;
			if (grpCode == timeTable[day][period + 1][0] && group[grpCode].seqTime == 1)
				return -1;
			if (grpCode == timeTable[day][period - 1][0] && grpCode == timeTable[day][period + 1][0] && group[grpCode].seqTime == 2)
				return -1;
		}
	}
	else if (group[grpCode].grade == 3)
	{
		if (period == 0)
		{
			if (grpCode == timeTable[day][period + 1][1] && group[grpCode].seqTime == 1)
				return -1;
		}
		else if (period == 8)
		{
			if (grpCode == timeTable[day][period - 1][1] && group[grpCode].seqTime == 1)
				return -1;
		}
		else
		{
			if (grpCode == timeTable[day][period - 1][1] && group[grpCode].seqTime == 1)
				return -1;
			if (grpCode == timeTable[day][period + 1][1] && group[grpCode].seqTime == 1)
				return -1;
			if (grpCode == timeTable[day][period - 1][1] && grpCode == timeTable[day][period + 1][1] && group[grpCode].seqTime == 2)
				return -1;
		}
	}
	if (group[grpCode].grade == 2)
	{
		if (timeTable[day][period][1] == -1)
		{
			return 1;
		}
		else // Need to find duplicated teacher & students
		{
			int sameTimeGrp = timeTable[day][period][1];
			return GrpDuplicate(grpCode, sameTimeGrp);
		}
	}
	else
	{
		if (timeTable[day][period][0] == -1) // Need to find duplicated teacher & students, also
		{
			return 1;
		}
		else
		{
			int sameTimeGrp = timeTable[day][period][0];
			return GrpDuplicate(grpCode, sameTimeGrp);
		}
	}
}
int SubDistCal(int dowi, int periodi, int dowf, int periodf)
{
	int i, j, k, l, moving, temp1, temp2, temp;
	int stuInfo;
	int howmansub = 0, whichsmall;
	int howmuctime[60] = { 0 };
	int howmanstu;
	int totaltime = 0;
	vector<int>student_list;
	vector<int>iniclass_student;
	vector<int>finclass_student;

	struct position
	{
		int floor, x, y;
	};
	vector<position> initial_position;
	vector<position> final_position;
	vector<position> stair_location;
	stair_location[0].x = 0; stair_location[0].y = 0;
	stair_location[1].x = 5; stair_location[1].y = 1;
	stair_location[1].x = -7; stair_location[1].y = 1;
	stair_location[1].x = -3; stair_location[1].y = 5;
	stair_location[1].x = -2; stair_location[1].y = -4;//초기 계단 위치 설정
	for (i = 0; i < 2; i++)
	{
		howmansub += subTable[dowi][periodi][i].size();
	}
	for (i = 0; i < howmansub; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < subTable[dowi][periodi][j].size(); k++)
			{
				howmanstu += subjectData[subTable[dowi][periodi][j][k]].student.size();
			}
		}
		whichsmall = howmanstu;
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				for (l = 0; l < 5; l++)
				{
					stuInfo = subjectData[subTable[dowi][periodi][k][l]].student[j];
					iniclass_student.push_back(stuInfo);//요일의 어떤 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
				}
			}
		}
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < subTable[dowi][periodi][j].size(); k++)
			{
				howmanstu += subjectData[subTable[dowf][periodf][j][k]].student.size();
			}
		}
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				for (l = 0; l < 5; l++)
				{
					stuInfo = subjectData[subTable[dowf][periodf][k][l]].student[j];
					finclass_student.push_back(stuInfo);//요일의 그 다음 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
				}
			}
		}
		if (whichsmall < howmanstu) howmanstu = whichsmall;
		//만약 그 다음 교시 수업에 참여하는 학생들이 이전 수업 학생 수보다 적다면 이전 수업 학생들의 다음 시간이 공강이라는 뜻이므로 그 학생들은 계산에 넣지 않기 위함
		for (j = 0; j < iniclass_student.size(); j++)
		{
			for (k = 0; k < finclass_student.size(); k++)
			{
				if (iniclass_student[j] == finclass_student[k])
				{
					student_list.push_back(finclass_student[k]);
				}
			}
		}//어떤 시간과 그 다음 시간 모두 수업을 듣는 학생들만 추려서 리스트에 정리해 넣음
	}
	for (i = 0; i < student_list.size(); i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < subTable[dowi][periodi][j].size(); k++)
			{
				initial_position[i].floor = subjectData[subTable[dowi][periodi][j][k]].floor;
				initial_position[i].x = subjectData[subTable[dowi][periodi][j][k]].x;
				initial_position[i].y = subjectData[subTable[dowi][periodi][j][k]].y;//학생들의 초기 위치 저장 
				final_position[i].floor = subjectData[subTable[dowf][periodf][j][k]].floor;
				final_position[i].x = subjectData[subTable[dowf][periodf][j][k]].x;
				final_position[i].y = subjectData[subTable[dowf][periodf][j][k]].y;//학생들의 이동 위치 저장 
				totaltime += (initial_position[i].floor - final_position[i].floor) * 5;//층간 이동 가중치는 5
				moving = 100;
				for (l = 0; l < 5; l++)
				{
					temp1 = abs(stair_location[l].x - initial_position[i].x) + abs(stair_location[l].y - initial_position[i].y);
					temp2 = abs(stair_location[l].x - final_position[i].x) + abs(stair_location[l].y - final_position[i].y);
					if (moving > temp1 + temp2)
					{
						moving = temp1 + temp2;
					}
				}
				totaltime += moving;
			}
		}
	}
	return totaltime;
}
int SrcSubDistCal(int dowi, int periodi, int dowf, int periodf)
{
	int i, j, k, l, moving, temp1, temp2, temp;
	int stuInfo;
	int howmansub = 0, howmanstu, whichsmall;
	int howmuctime[60] = { 0 };
	int totaltime = 0;
	vector<int>student_list;
	vector<int>iniclass_student;
	vector<int>finclass_student;

	struct position
	{
		int floor, x, y;
	};
	vector<position> initial_position;
	vector<position> final_position;
	vector<position> stair_location;
	stair_location[0].x = 0; stair_location[0].y = 0;
	stair_location[1].x = 5; stair_location[1].y = 1;
	stair_location[1].x = -7; stair_location[1].y = 1;
	stair_location[1].x = -3; stair_location[1].y = 5;
	stair_location[1].x = -2; stair_location[1].y = -4;//초기 계단 위치 설정
	for (i = 0; i < 2; i++)
	{
		howmansub += subTableSrc[dowi][dowf][i].size();
	}
	for (i = 0; i < howmansub; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < subTableSrc[dowi][periodi][j].size(); k++)
			{
				howmanstu += subjectData[subTableSrc[dowi][periodi][j][k]].student.size();
			}
		}
		whichsmall = howmanstu;
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				for (l = 0; l < 5; l++)
				{
					stuInfo = subjectData[subTableSrc[dowi][periodi][k][l]].student[j];
					iniclass_student.push_back(stuInfo);//요일의 어떤 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
				}
			}
		}
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < subTableSrc[dowi][periodi][j].size(); k++)
			{
				howmanstu += subjectData[subTableSrc[dowf][periodf][j][k]].student.size();
			}
		}
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				for (l = 0; l < 5; l++)
				{
					stuInfo = subjectData[subTableSrc[dowf][periodf][k][l]].student[j];
					finclass_student.push_back(stuInfo);//요일의 그 다음 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
				}
			}
		}
		if (whichsmall < howmanstu) howmanstu = whichsmall;
		//만약 그 다음 교시 수업에 참여하는 학생들이 이전 수업 학생 수보다 적다면 이전 수업 학생들의 다음 시간이 공강이라는 뜻이므로 그 학생들은 계산에 넣지 않기 위함
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < howmanstu; k++)
			{
				if (iniclass_student[j] == finclass_student[k])
				{
					student_list.push_back(finclass_student[k]);
				}
			}
		}//어떤 시간과 그 다음 시간 모두 수업을 듣는 학생들만 추려서 리스트에 정리해 넣음
	}
	for (i = 0; i < student_list.size(); i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < subTableSrc[dowi][periodi][j].size(); k++)
			{
				initial_position[i].floor = subjectData[subTableSrc[dowi][periodi][j][k]].floor;
				initial_position[i].x = subjectData[subTableSrc[dowi][periodi][j][k]].x;
				initial_position[i].y = subjectData[subTableSrc[dowi][periodi][j][k]].y;//학생들의 초기 위치 저장 
				final_position[i].floor = subjectData[subTableSrc[dowf][periodf][j][k]].floor;
				final_position[i].x = subjectData[subTableSrc[dowf][periodf][j][k]].x;
				final_position[i].y = subjectData[subTableSrc[dowf][periodf][j][k]].y;//학생들의 이동 위치 저장 
				totaltime += (initial_position[i].floor - final_position[i].floor) * 5;//층간 이동 가중치는 5
				moving = 100;
				for (l = 0; l < 5; l++)
				{
					temp1 = abs(stair_location[l].x - initial_position[i].x) + abs(stair_location[l].y - initial_position[i].y);
					temp2 = abs(stair_location[l].x - final_position[i].x) + abs(stair_location[l].y - final_position[i].y);
					if (moving > temp1 + temp2)
					{
						moving = temp1 + temp2;
					}
				}
				totaltime += moving;
			}
		}
	}
	return totaltime;
}
int SubTotDist() // 2차 SA 이웃해용 총 거리계산
{
	int Dtotaltime = 0;
	int dow, period;
	for (dow = 0; dow < 5; dow++)
	{
		for (period = 0; period < 8; period++)
		{
			Dtotaltime += SubDistCal(dow, period, dow, period + 1);
		}
	}
	return Dtotaltime;
}
int SrcSubTotDist() // 2차 SA 이웃해용 총 거리계산
{
	int Dtotaltime = 0;
	int dow, period;
	for (dow = 0; dow < 5; dow++)
	{
		for (period = 0; period < 8; period++)
		{
			Dtotaltime += SubDistCal(dow, period, dow, period + 1);
		}
	}
	return Dtotaltime;
}
int subFit()
{
	vector<int> initTea1;
	vector<int> initTea9;
	vector<int> finTea1;
	vector<int> finTea9;

	int Seq1 = 0, Seq9 = 0, totDist, fit = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (subTable[i][0][0].size() > j)
				for (int k = 0; subjectData[subTable[i][0][0][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTable[i][0][0][j]].teacher[k]);
				}
			if (subTable[i][0][1].size() > j)
				for (int k = 0; subjectData[subTable[i][0][1][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTable[i][0][1][j]].teacher[k]);
				}
			if (subTable[i][8][0].size() > j)
				for (int k = 0; subjectData[subTable[i][8][0][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTable[i][8][0][j]].teacher[k]);
				}
			if (subTable[i][8][1].size() > j)
				for (int k = 0; subjectData[subTable[i][8][1][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTable[i][8][1][j]].teacher[k]);
				}
			if (subTable[i + 1][0][0].size() > j)
				for (int k = 0; subjectData[subTable[i + 1][0][0][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTable[i + 1][0][0][j]].teacher[k]);
				}
			if (subTable[i + 1][0][1].size() > j)
				for (int k = 0; subjectData[subTable[i + 1][0][1][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTable[i + 1][0][1][j]].teacher[k]);
				}
			if (subTable[i + 1][8][0].size() > j)
				for (int k = 0; subjectData[subTable[i + 1][8][0][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTable[i + 1][8][0][j]].teacher[k]);
				}
			if (subTable[i + 1][8][1].size() > j)
				for (int k = 0; subjectData[subTable[i + 1][8][1][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTable[i + 1][8][1][j]].teacher[k]);
				}
		}
		for (int i = 0; i < initTea1.size(); i++)
		{
			for (int j = 0; j < finTea1.size(); j++)
			{
				if (initTea1[i] == finTea1[j])
					Seq1++;
			}
		}
		for (int i = 0; i < initTea9.size(); i++)
		{
			for (int j = 0; j < finTea1.size(); j++)
			{
				if (initTea9[i] == finTea9[j])
					Seq9++;
			}
		}
		initTea1.clear();
		initTea9.clear();
		finTea1.clear();
		finTea9.clear();
	}
	totDist = SubTotDist();
	fit = totDist + Seq1 * 100 + Seq9 * 100;
	return fit;
}
int SrcSubFit()
{
	vector<int> initTea1;
	vector<int> initTea9;
	vector<int> finTea1;
	vector<int> finTea9;
	int Seq1 = 0, Seq9 = 0, totDist, fit = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (subTableSrc[i][0][0].size() > j)
				for (int k = 0; subjectData[subTableSrc[i][0][0][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTableSrc[i][0][0][j]].teacher[k]);
				}
			if (subTableSrc[i][0][1].size() > j)
				for (int k = 0; subjectData[subTableSrc[i][0][1][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTableSrc[i][0][1][j]].teacher[k]);
				}
			if (subTableSrc[i][8][0].size() > j)
				for (int k = 0; subjectData[subTableSrc[i][8][0][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTableSrc[i][8][0][j]].teacher[k]);
				}
			if (subTableSrc[i][8][1].size() > j)
				for (int k = 0; subjectData[subTableSrc[i][8][1][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTableSrc[i][8][1][j]].teacher[k]);
				}
			if (subTableSrc[i + 1][0][0].size() > j)
				for (int k = 0; subjectData[subTableSrc[i + 1][0][0][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTableSrc[i + 1][0][0][j]].teacher[k]);
				}
			if (subTableSrc[i + 1][0][1].size() > j)
				for (int k = 0; subjectData[subTableSrc[i + 1][0][1][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTableSrc[i + 1][0][1][j]].teacher[k]);
				}
			if (subTableSrc[i + 1][8][0].size() > j)
				for (int k = 0; subjectData[subTableSrc[i + 1][8][0][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTableSrc[i + 1][8][0][j]].teacher[k]);
				}
			if (subTableSrc[i + 1][8][1].size() > j)
				for (int k = 0; subjectData[subTableSrc[i + 1][8][1][j]].teacher.size(); k++)
				{
					initTea1.push_back(subjectData[subTableSrc[i + 1][8][1][j]].teacher[k]);
				}
		}

		for (int i = 0; i < initTea1.size(); i++)
		{
			for (int j = 0; j < finTea1.size(); j++)
			{
				if (initTea1[i] == finTea1[j])
					Seq1++;
			}
		}
		for (int i = 0; i < initTea9.size(); i++)
		{
			for (int j = 0; j < finTea1.size(); j++)
			{
				if (initTea9[i] == finTea9[j])
					Seq9++;
			}
		}
		initTea1.clear();
		initTea9.clear();
		finTea1.clear();
		finTea9.clear();
	}
	totDist = SrcSubTotDist();
	fit = totDist + Seq1 * 100 + Seq9 * 100;
	return fit;
}
int main()
{
	srand(time(NULL));
	int randGroup = 0;
	int groupDiv = 0; // 2,3학년 그룹의 경계
	int a, esc = 1, escape = 1;
	int grpNum, subNum = 0, scanCode = 0;
	if (fin == NULL)
	{
		printf("뭐하냐");
		return -1;
	}
	fscanf(fin, "%d", &grpNum);
	group.resize(grpNum);
	for (int i = 0; i < grpNum; i++)
	{
		fscanf(fin, "%d", &subNum);
		group[i].subject.resize(subNum);
		printf("%d", subNum);
		for (int j = 0; j < subNum; j++)
		{
			fscanf(fin, "%d", &scanCode);
			/*if (scanCode == -1)
			{
				break;
			}
			else
			{
				group[i].subject[j].subcode=scanCode;
			}*/
			group[i].subject[j].subcode = scanCode;
		}
	}
	int totSubNum, studentNum, tchr, stdt;
	fscanf(fin, "%d", &totSubNum);
	subjectData.resize(totSubNum);
	for (int i = 0; i < totSubNum; i++)
	{
		fscanf(fin, "%d %d %d %d %d %d %d %d", &subjectData[i].floor, &subjectData[i].x, &subjectData[i].y, &subjectData[i].teacher, &subjectData[i].grade, &subjectData[i].time, &subjectData[i].sequent, &studentNum);
		for (int j = 0;; j++)
		{
			fscanf(fin, "%d", &tchr);
			if (tchr == -100)
				break;
			else
				subjectData[i].teacher.push_back(tchr);
		}
		for (int j = 0; ; j++)
		{
			fscanf(fin, "%d", &stdt);
			if (stdt == -100)
				break;
			else
				subjectData[i].student.push_back(stdt);
		}
	}
	fclose(fin);
	for (int i = 0; i < group.size(); i++)
	{
		if (group[i].grade == 2 && group[i + 1].grade == 3)
			groupDiv == i + 1;
	}
	for (int i = 0; i < group.size(); i++) // 초기화
	{
		group[i].currentTime = 0;
	}
	for (int i = 0; i < group.size(); i++)
	{
		for (int j = 0; j < group[i].subject.size(); j++)
		{
			for (int k = 0; k < group[i].subject[j].student.size(); k++)
			{
				group[i].student.push_back(group[i].subject[j].student[k]);
			}
			for (int k = 0; k < group[i].subject[j].teacher.size(); j++)
			{
				group[i].teacher.push_back(group[i].subject[j].student[k]);
			}
		}
	}
	for (int i = 0; i < group.size(); i++)
	{
		int inc = 0;
		for (int j = 0; j < group[i].subject.size(); j++)
		{
			if (group[i].subject[j].sequent >= 1)
			{
				if (group[i].subject[j].sequent > inc)
					inc = group[i].subject[j].sequent;
			}
		}
		group[i].seqTime = inc;
	}
	for (int i = 0; i < 5; i++) // 시간표 초기화
	{
		for (int j = 0; j < 9; j++)
		{
			timeTable[i][j][0] = -1;
			timeTable[i][j][1] = -1;
			timeTableSrc[i][j][0] = -1;
			timeTableSrc[i][j][1] = -1;
		}
	}
	for (int i = 0; i < subjectData.size(); i++)
	{
		subjectData[i].subcode = i;
	}
	for (int i = 0; i < group.size(); i++)
	{
		for (int j = 0; j <= group[i].subject.size(); j++)
		{
			memcpy(&(group[i].subject[j]), &(subjectData[group[i].subject[j].subcode]), sizeof(subjectData[group[i].subject[j].subcode]));

		}
	}
	for (int i = 0; i < group.size(); i++)
	{
		group[i].grade = group[i].subject[0].grade;
	}
	int totalTime = 0;
	int totalCrtTime = 0;
	for (int i = 0; i < group.size(); i++)
	{
		totalTime += group[i].maxTime;
	}
	for (int i = 0; i < 5; i++) // 1차 SA를 위한 무작위 시간표 할당, 단 가능 여부는 고려
	{
		for (int j = 0; j < 9; j++)
		{
			while (totalCrtTime < totalTime) {
				randGroup = rand() % (groupDiv);
				if (group[randGroup].maxTime >= group[randGroup].currentTime + 1)
				{
					if (GrpAbleTime(randGroup, i, j))
					{
						group[randGroup].currentTime++;
						timeTable[i][j][0] = randGroup;
						totalCrtTime++;
						break;
					}
				}
				randGroup = (rand() % (group.size() - groupDiv));
				randGroup += groupDiv;
				if (group[randGroup].maxTime >= group[randGroup].currentTime + 1)
				{
					if (GrpAbleTime(randGroup, i, 0))
					{
						group[randGroup].currentTime++;
						timeTable[i][0][1] = randGroup;
						totalCrtTime++;
						break;
					}
				}
			}
		}
	}
	printf("Random Tabling Finished!\n");
	double tem = 1, temDec = 0.8, tFin = 0.01, random; // SA를 위한 온도, 온도감소계수, 탈출온도 설정
	int loop = 5000, looped, randDay1, randDay2, randPrd1, randPrd2, randGrd, swap, srcFit, crtFit, fitDiff; // SA를 위한 내부루프수 설정 및 무작위요일, 무작위교시, 현재 해의 적합도, 이웃 해의 적합도 선언
	memcpy(timeTableSrc, timeTable, sizeof(timeTable));
	for (; tem > tFin; tem *= temDec)
	{
		looped = 0;
		while (looped <= loop)
		{
			do
			{
				randDay1 = rand() % 5;
				randPrd1 = rand() % 9;
				randDay2 = rand() % 5;
				randPrd2 = rand() % 9;
				randGrd = rand() % 2;
			} while (randDay1 != randDay2 || randPrd1 != randPrd2);
			if (GrpAbleTime(timeTable[randDay1][randPrd1][randGrd], randDay2, randDay2) == -1) // 각 그룹을 이동했을때 수업이 가능한지 계산 ( 함수 GrpAbleTime 이용 )
				continue;
			if (GrpAbleTime(timeTable[randDay2][randPrd2][randGrd], randDay1, randDay1) == -1)
				continue;
			swap = timeTableSrc[randDay1][randPrd1][randGrd]; // 교체가 가능하면 두 교시를 바꾼 후 적합도 계산
			timeTableSrc[randDay1][randPrd1][randGrd] = timeTableSrc[randDay2][randPrd2][randGrd];
			timeTableSrc[randDay2][randPrd2][randGrd] = swap;
			crtFit = FitnessCal(); // 사용자 정의 함수를 사용해 현재 해의 적합도를 도출 
			srcFit = SrcFitnessCal(); // 사용자 정의 함수를 사용해 이웃 해의 적합도를 도출 
			fitDiff = srcFit - crtFit;
			if (fitDiff <= 0) // 만약 이웃해의 적합도가 높다면 이웃해로 이동, 단 여기서 이웃해의 적합도 (srtFit 값)은 수치로 봤을때는 작을수록 적합하다는 것을 주의, 앞에서 언급한 적합도가 높다는 것은 그 값이 크다는게 아닌, 더욱 적합하다는 뜻이다. )
			{
				memcpy(timeTable, timeTableSrc, sizeof(timeTableSrc));
				looped++;
				continue;
			}
			else // 만약 이웃해의 적합도가 낮다면, 현재해와 적합도 차이와 온도를 고려해 exponential 함수를 이용하여 확률함수로 이동 여부를 결정한다.
			{
				random = (1 / ((rand() % 1000) + 1));
				if (exp((float)((-fitDiff) / (tem))) >= random)
				{
					memcpy(timeTable, timeTableSrc, sizeof(timeTableSrc));
					looped++;
					continue;
				}
				else
				{
					memcpy(timeTableSrc, timeTable, sizeof(timeTable));
					looped++;
				}
			}
		}
	}
	printf("Level 1 Simulated Annealing Done!");
	for (int i = 0; i < 5; i++) // 수업시간 구조체 벡터를 선언
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < group[timeTable[i][j][0]].subject.size(); k++)
			{
				timeData[group[timeTable[i][j][0]].subject[k].subcode].push_back({ i,j,0 });
			}
			for (int k = 0; k < group[timeTable[i][j][1]].subject.size(); k++)
			{
				timeData[group[timeTable[i][j][1]].subject[k].subcode].push_back({ i,j,0 });

			}
		}
	}
	int randTime, subTime, currentTime = 0;
	for (int i = 0; i < subjectData.size(); i++) // 과목 무작위 할당
	{
		currentTime = 0;
		subTime = subjectData[i].time;
		while (currentTime < subTime)
		{
			randTime = rand() % subTime;
			if (timeData[i][randTime].selected == 0)
			{
				subTable[timeData[i][randTime].day][timeData[i][randTime].period][subjectData[i].grade - 2].push_back(i);
				currentTime++;
			}
			else
			{
				continue;
			}
		}
	}
	int randSub, randTime1, randTime2, incTime, noIncTime;
	for (tem = 1; tem >= tFin; tem *= temDec) // 2차 Simulated Annealing, 그룹에 포함된 과목들을 그룹이 할당된 시간안에서 각 과목의 시수에 맞추어 탐색한다.
	{
		looped = 0;
		while (looped < loop)
		{
			while (1) // 어떠한 과목이 포함되있는 그룹이 할당된 시간 중에서 랜덤으로 탐색
			{
				randSub = rand() % subjectData.size();
				if (subjectData[randSub].time == 1)
					continue;
				randTime1 = rand() % subjectData[randSub].time;
				randTime2 = rand() % subjectData[randSub].time;
				if (randTime1 == randTime2)
					break;
				if (timeData[randSub][randTime1].selected == timeData[randSub][randTime2].selected)
					break;
			}
			memcpy(subTableSrc, subTable, sizeof(subTable));
			if (timeData[randSub][randTime1].selected == 1)
			{
				incTime = randTime1;
				noIncTime = randTime2;
				timeData[randSub][randTime1].selected = 0;
				timeData[randSub][randTime2].selected = 1;
			}
			else
			{
				incTime = randTime2;
				noIncTime = randTime1;
				timeData[randSub][randTime2].selected = 0;
				timeData[randSub][randTime1].selected = 1;
			}
			for (a = 0; a < subTableSrc[timeData[randSub][incTime].day][timeData[randSub][incTime].period][subjectData[randSub].grade - 2].size(); a++)
			{
				if (subTableSrc[timeData[randSub][incTime].day][timeData[randSub][incTime].period][subjectData[randSub].grade - 2][a] == randSub)
				{
					for (int b = a; b < subTableSrc[timeData[randSub][incTime].day][timeData[randSub][incTime].period][subjectData[randSub].grade - 2].size() - 1; b++)
					{
						subTableSrc[timeData[randSub][incTime].day][timeData[randSub][incTime].period][subjectData[randSub].grade - 2][b] = subTableSrc[timeData[randSub][incTime].day][timeData[randSub][incTime].period][subjectData[randSub].grade - 2][b + 1];
						subTableSrc[timeData[randSub][incTime].day][timeData[randSub][incTime].period][subjectData[randSub].grade - 2].pop_back();
					}
				}
			}
			subTableSrc[timeData[randSub][noIncTime].day][timeData[randSub][noIncTime].period][subjectData[randSub].grade - 2].push_back(randSub);
			crtFit = subFit(); // 이 또한 적합도 도출이나, 2차 SA에 맞추어 새롭게 정의된 함수 
			srcFit = SrcSubFit(); // 이웃 해의 적합도를 도출하는 함수
			if (crtFit > srcFit)
			{
				memcpy(subTable, subTableSrc, sizeof(subTable));
				looped++;
			}
			else
			{
				fitDiff = srcFit - crtFit;
				random = (1 / ((rand() % 1000) + 1));
				if (exp((float)((-fitDiff) / (tem))) >= random)
				{
					memcpy(subTable, subTableSrc, sizeof(subTableSrc));
					looped++;
					continue;
				}
				else
				{
					memcpy(subTableSrc, subTable, sizeof(subTable));
					looped++;
				}
			}
		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			fprintf(fout, "%d일 %d교시 :  ", i + 1, j + 1);
			for (int k = 0; k < subTable[i][j][0].size(); k++)
			{
				fprintf(fout, "%d ", subTable[i][j][0][k]);
			}
			for (int k = 0; k < subTable[i][j][1].size(); k++)
			{
				fprintf(fout, "%d ", subTable[i][j][0][1]);
			}
		}
		fprintf(fout, "\n");
	}
	fprintf(fout, "총 이동거리 : %d", SubTotDist());
	fclose(fout);
}
