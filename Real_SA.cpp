#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <memory.h>

using namespace std;
int timeTable[5][9][2]; // -1 : 할당된 그룹없음
int timeTableSrc[5][9][2]; // Save the code of Searched Group
int subTable[5][9][2][5];
int subTableSrc[5][9][2][5];
typedef struct subjectbase
{
	vector<int> student;
	int subcode, floor, x, time, y, grade;
	int teacher;
	int sequent; // 연강 시간
}subjectBase;
typedef struct groupbase
{
	vector<subjectBase> subject;
	int groupCode;
	int maxTime;
	int currentTime;
	int grade;
	int seqTime; // 최대 연강 시수
	vector<int> student;
	vector<int> teacher;
}groupBase;
typedef struct timebase
{
	int day, period, selected;
}timeBase;

vector<groupBase> group;
vector<subjectBase> subjectData;
vector<vector<timeBase> >time(200);

int DistCal(int dowi, int periodi, int dowf, int periodf)
{
	int i, j, k, l, moving;
	int stuInfo;
	int howmansub = 0, howmanstu = 0, whichsmall;
	int howmuctime[60] = { 0 };
	int totaltime = 0;
	int temp1, temp2, temp;
	vector<int> student_list;
	vector<int> iniclass_student;
	vector<int> finclass_student;
	struct position
	{
		int floor, x, y;
	};//층, 같은 층 내에서 x,y 값을 저장한다
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
				iniclass_student.push_back(stuInfo);//요일의 어떤 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
			}
		}
		howmanstu = 0;//학생 수 초기화
		for (j = 0; j < 2; j++)
		{
			howmanstu += group[timeTable[dowf][periodf][k]].subject[i].student.size();
		}
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				stuInfo = group[timeTable[dowf][periodf][k]].subject[i].student[j];
				finclass_student.push_back(stuInfo);//요일의 그 다음 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
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
			for (k = 0; k < 5; k++)
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
					temp1 = abs(stair_location[i].x - initial_position[i].x) + abs(stair_location[i].y - initial_position[i].y);
					if (moving > temp) moving = temp;
					totaltime += moving;
					moving = 100;
					temp2 = abs(stair_location[i].x - final_position[i].x) + abs(stair_location[i].y - final_position[i].y);
					if (moving > temp) moving = temp;
					totaltime += moving;
				}
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
	int i, j, k, l, moving, temp1, temp2, temp;
	int stuInfo;
	int howmansub = 0, howmanstu = 0, whichsmall;
	int howmuctime[60] = { 0 };
	int totaltime = 0;
	vector<int> student_list;
	vector<int> iniclass_student;
	vector<int> finclass_student;
	struct position
	{
		int floor, x, y;
	};//층, 같은 층 내에서 x,y 값을 저장한다
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
				iniclass_student.push_back(stuInfo);//요일의 어떤 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
			}
		}
		howmanstu = 0;//학생 수 초기화
		for (j = 0; j < 2; j++)
		{
			howmanstu += group[timeTableSrc[dowf][periodf][k]].subject[i].student.size();
		}
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				stuInfo = group[timeTableSrc[dowf][periodf][k]].subject[i].student[j];
				finclass_student.push_back(stuInfo);//요일의 그 다음 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
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
			for (k = 0; k < 5; k++)
			{
				initial_position[i].floor = subjectData[subTable[dowi][periodi][j][k]].floor;
				initial_position[i].x = subjectData[subTable[dowi][periodi][j][k]].x;
				initial_position[i].y = subjectData[subTable[dowi][periodi][j][k]].y;//학생들의 초기 위치 저장 
				final_position[i].floor = subjectData[subTableSrc[dowf][periodf][j][k]].floor;
				final_position[i].x = subjectData[subTable[dowf][periodf][j][k]].x;
				final_position[i].y = subjectData[subTable[dowf][periodf][j][k]].y;//학생들의 이동 위치 저장 
				totaltime += (initial_position[i].floor - final_position[i].floor) * 5;//층간 이동 가중치는 5
				moving = 100;
				for (l = 0; l < 5; l++)
				{
					temp1 = abs(stair_location[i].x - initial_position[i].x) + abs(stair_location[i].y - initial_position[i].y);
					if (moving > temp) moving = temp;
					totaltime += moving;
					moving = 100;
					temp2 = abs(stair_location[i].x - final_position[i].x) + abs(stair_location[i].y - final_position[i].y);
					if (moving > temp) moving = temp;
					totaltime += moving;
				}
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
		for (int i = 0; i < group[timeTable[day][1][2]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTable[day + 1][1][2]].teacher.size(); j++)
			{
				if (group[timeTable[day][1][2]].teacher[i] == group[timeTable[day + 1][1][2]].teacher[j])
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
		for (int i = 0; i < group[timeTable[day][9][2]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTable[day + 1][9][2]].teacher.size(); j++)
			{
				if (group[timeTable[day][9][2]].teacher[i] == group[timeTable[day + 1][9][2]].teacher[j])
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
		for (int i = 0; i < group[timeTableSrc[day][1][2]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTableSrc[day + 1][1][2]].teacher.size(); j++)
			{
				if (group[timeTableSrc[day][1][2]].teacher[i] == group[timeTableSrc[day + 1][1][2]].teacher[j])
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
		for (int i = 0; i < group[timeTableSrc[day][9][2]].teacher.size(); i++)
		{
			for (int j = 0; j < group[timeTableSrc[day + 1][9][2]].teacher.size(); j++)
			{
				if (group[timeTableSrc[day][9][2]].teacher[i] == group[timeTableSrc[day + 1][9][2]].teacher[j])
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

int GrpDuplicate(int grp1, int grp2) // 두 그룹이 같은 시간에 수업이 가능한지 판단! ( 교사 / 학생 / 교실 중복여부 판단 ) Able:1 Disable:-1
{
	for (int i = 0; i < group[grp1].teacher.size(); i++)
	{
		for (int j = 0; j < group[grp2].teacher.size(); j++)
		{
			if (group[grp1].teacher[i] == group[grp2].teacher[j])
				return -1;
		}
	}
	for (int i = 0; i < group[grp1].student.size(); i++)
	{
		for (int j = 0; j < group[grp2].student.size(); j++)
		{
			if (group[grp1].student[i] == group[grp2].student[j])
				return -1;
		}
	}
	for (int i = 0; i < group[grp1].subject.size(); i++)
	{
		for (int j = 0; j < group[grp2].subject.size(); j++)
		{
			if (group[grp1].subject[i].floor == group[grp2].subject[j].floor && group[grp1].subject[i].x == group[grp2].subject[j].y && group[grp1].subject[i].y == group[grp2].subject[j].y)
				return -1;
		}
	}
	return 1;
}
int GrpAbleTime(int grpCode, int day, int period)// 이 그룹이 그 시간에 수업이 가능한지 판단!
{
	int impTime[60][5][9];
	for (int i = 0; i < group[grpCode].teacher.size; i++)
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
		for (j = 0; j < 5; j++)
		{
			if (subTable[dowi][periodi][i][j] != -1)
				howmansub++;//과목 수를 센다
		}
	}
	for (i = 0; i < howmansub; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < 5; k++)
			{
				while (subTable[dowi][periodi][j][k] != -1)
				{
					howmanstu += subjectData[subTable[dowi][periodi][j][k]].student.size();
				}//학생 수의 최댓값을 센다(겹치는 학생 고려 x한 결과)
			}
		}
		whichsmall = howmanstu;
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				for (l = 0; l < 5; l++)
				{
					while (subTable[dowi][periodi][j][k] != -1)
					{
						stuInfo = subjectData[subTable[dowi][periodi][k][l]].student[j];
						iniclass_student.push_back(stuInfo);//요일의 어떤 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
					}
				}
			}
		}
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < 5; k++)
			{
				while (subTable[dowf][periodf][j][k] != -1)
				{
					howmanstu += subjectData[subTable[dowf][periodf][j][k]].student.size();
				}
			}
		}
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				for (l = 0; l < 5; l++)
				{
					while (subTable[dowf][periodf][j][k] != -1)
					{
						stuInfo = subjectData[subTable[dowf][periodf][k][l]].student[j];
						finclass_student.push_back(stuInfo);//요일의 그 다음 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
					}
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
			for (k = 0; k < 5; k++)
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
					temp1 = abs(stair_location[i].x - initial_position[i].x) + abs(stair_location[i].y - initial_position[i].y);
					if (moving > temp) moving = temp;
					totaltime += moving;
					moving = 100;
					temp2 = abs(stair_location[i].x - final_position[i].x) + abs(stair_location[i].y - final_position[i].y);
					if (moving > temp) moving = temp;
					totaltime += moving;
				}
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
		for (j = 0; j < 5; j++)
		{
			if (subTableSrc[dowi][periodi][i][j] != -1)
				howmansub++;//과목 수를 센다
		}
	}
	for (i = 0; i < howmansub; i++)
	{
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < 5; k++)
			{
				while (subTableSrc[dowi][periodi][j][k] != -1)
				{
					howmanstu += subjectData[subTableSrc[dowi][periodi][j][k]].student.size();
				}//학생 수의 최댓값을 센다(겹치는 학생 고려 x한 결과)
			}
		}
		whichsmall = howmanstu;
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				for (l = 0; l < 5; l++)
				{
					while (subTableSrc[dowi][periodi][j][k] != -1)
					{
						stuInfo = subjectData[subTableSrc[dowi][periodi][k][l]].student[j];
						iniclass_student.push_back(stuInfo);//요일의 어떤 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
					}
				}
			}
		}
		for (j = 0; j < 2; j++)
		{
			for (k = 0; k < 5; k++)
			{
				while (subTableSrc[dowf][periodf][j][k] != -1)
				{
					howmanstu += subjectData[subTableSrc[dowf][periodf][j][k]].student.size();
				}
			}
		}
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < 2; k++)
			{
				for (l = 0; l < 5; l++)
				{
					while (subTableSrc[dowf][periodf][j][k] != -1)
					{
						stuInfo = subjectData[subTableSrc[dowf][periodf][k][l]].student[j];
						finclass_student.push_back(stuInfo);//요일의 그 다음 교시에 있는 수업에 참여하는 학생들을 리스트에 정리해 넣음
					}
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
			for (k = 0; k < 5; k++)
			{
				initial_position[i].floor = subjectData[subTableSrc[dowi][periodi][j][k]].floor;
				initial_position[i].x = subjectData[subTableSrc[dowi][periodi][j][k]].x;
				initial_position[i].y = subjectData[subTableSrc[dowi][periodi][j][k]].y;//학생들의 초기 위치 저장 
				final_position[i].floor = subjectData[subTable[dowf][periodf][j][k]].floor;
				final_position[i].x = subjectData[subTableSrc[dowf][periodf][j][k]].x;
				final_position[i].y = subjectData[subTableSrc[dowf][periodf][j][k]].y;//학생들의 이동 위치 저장 
				totaltime += (initial_position[i].floor - final_position[i].floor) * 5;//층간 이동 가중치는 5
				moving = 100;
				for (l = 0; l < 5; l++)
				{
					temp1 = abs(stair_location[i].x - initial_position[i].x) + abs(stair_location[i].y - initial_position[i].y);
					if (moving > temp) moving = temp;
					totaltime += moving;
					moving = 100;
					temp2 = abs(stair_location[i].x - final_position[i].x) + abs(stair_location[i].y - final_position[i].y);
					if (moving > temp) moving = temp;
					totaltime += moving;
				}
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
			Dtotaltime += SrcSubDistCal(dow, period, dow, period + 1);
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
			if (subTable[i][0][0][j] != -1)
				initTea1.push_back(subjectData[subTable[i][0][0][j]].teacher);
			if (subTable[i][0][1][j] != -1)
				initTea1.push_back(subjectData[subTable[i][0][1][j]].teacher);
			if (subTable[i][8][0][j] != -1)
				initTea9.push_back(subjectData[subTable[i][8][0][j]].teacher);
			if (subTable[i][8][1][j] != -1)
				initTea9.push_back(subjectData[subTable[i][8][1][j]].teacher);
			if (subTable[i + 1][0][0][j] != -1)
				finTea1.push_back(subjectData[subTable[i + 1][0][0][j]].teacher);
			if (subTable[i + 1][0][1][j] != -1)
				finTea1.push_back(subjectData[subTable[i + 1][0][1][j]].teacher);
			if (subTable[i + 1][8][0][j] != -1)
				finTea9.push_back(subjectData[subTable[i + 1][8][0][j]].teacher);
			if (subTable[i + 1][8][1][j] != -1)
				finTea9.push_back(subjectData[subTable[i + 1][8][1][j]].teacher);
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
			if (subTableSrc[i][0][0][j] != -1)
				initTea1.push_back(subjectData[subTableSrc[i][0][0][j]].teacher);
			if (subTableSrc[i][0][1][j] != -1)
				initTea1.push_back(subjectData[subTableSrc[i][0][1][j]].teacher);
			if (subTableSrc[i][8][0][j] != -1)
				initTea9.push_back(subjectData[subTableSrc[i][8][0][j]].teacher);
			if (subTableSrc[i][8][1][j] != -1)
				initTea9.push_back(subjectData[subTableSrc[i][8][1][j]].teacher);
			if (subTableSrc[i + 1][0][0][j] != -1)
				finTea1.push_back(subjectData[subTableSrc[i + 1][0][0][j]].teacher);
			if (subTableSrc[i + 1][0][1][j] != -1)
				finTea1.push_back(subjectData[subTableSrc[i + 1][0][1][j]].teacher);
			if (subTableSrc[i + 1][8][0][j] != -1)
				finTea9.push_back(subjectData[subTableSrc[i + 1][8][0][j]].teacher);
			if (subTableSrc[i + 1][8][1][j] != -1)
				finTea9.push_back(subjectData[subTableSrc[i + 1][8][1][j]].teacher);
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
	int groupDiv;
	for (int i = 0; i < group.size(); i++)
	{
		if (group[i].grade == 3)
		{
			groupDiv = i;
		}
	}
	for (int i = 0; i < group.size(); i++)
	{
		group[i].currentTime = 0;
	}
	for (int i = 0; i < group.size(); i++)
	{
		for (int j = 0; j < group[i].subject.size(); j++)
		{
			for (int k = 0; k < group[i].subject[j].student.size(); k++)
			{
				(group[i].student).push_back(group[i].subject[j].student[k]);
			}
			for (int k = 0;)
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
	for (int i = 0; i < 5; i++) // Initializing Timetable
	{
		for (int j = 0; j < 9; j++)
		{
			timeTable[i][j][0] = -1;
			timeTable[i][j][1] = -1;
			timeTableSrc[i][j][0] = -1;
			timeTableSrc[i][j][1] = -1;
		}
	}
	for (int i = 0; i < group.size(); i++)
	{
		for (int j = 0; j < group[i].subject.size(); j++)
		{
			group[i].subject[j].grade = group[i].grade;
		}
	}
	for (int i = 0; i < 5; i++) // Random Tabling
	{
		while (1) {
			randGroup = rand() % (groupDiv);
			if (group[randGroup].maxTime >= group[randGroup].currentTime + 1)
			{
				if (GrpAbleTime(randGroup, i, 0))
				{
					group[randGroup].currentTime++;
					timeTable[i][0][0] = randGroup;
					break;
				}
			}
			randGroup = rand() % (group.size() - groupDiv);
			randGroup += groupDiv;
			if (group[randGroup].maxTime >= group[randGroup].currentTime + 1)
			{
				if (GrpAbleTime(randGroup, i, 0))
				{
					group[randGroup].currentTime++;
					timeTable[i][0][1] = randGroup;
					break;
				}
			}

		}
	}
	printf("Random Tabling Finished!\n");
	for (int prd = 0; prd < 5; prd++) // Greedy Tabling ( For initial Table )
	{
		for (int day = 0; day < 5; day++)
		{
			int shortDist = 0;
			int currentDist = 0;
			int shortGroup = 0;
			int groupGreed;
			for (groupGreed = 0; groupGreed < group.size(); groupGreed++)
			{
				if (group[groupGreed].maxTime >= group[groupGreed].currentTime + 1)
				{
					if (timeTable[day][prd + 1] == -1)
					{
						timeTable[day][prd + 1] = groupGreed;
						shortDist = DistCal(day, prd, day, prd + 1);
						shortGroup = groupGreed;
						continue;
					}
					timeTable[day][prd + 1] = groupGreed;
					currentDist = DistCal(day, prd, day, prd + 1);
					if (shortDist > currentDist)
					{
						shortDist = currentDist;
						shortGroup = groupGreed;
					}
					else
					{
						timeTable[day][prd + 1] = shortGroup;
					}
				}
				if (shortDist == 0 && shortGroup == 0)
				{
					break;
				}
			}
		}
	}
	printf("Greedy Initial Tabling Finished! \n");
	double tem = 1, temDec = 0.8, tFin = 0.01, random;
	int loop = 5000, looped, randDay1, randDay2, randPrd1, randPrd2, randGrd, swap, srcFit, crtFit, fitDiff;
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
			if (GrpAbleTime(timeTable[randDay1][randPrd1][randGrd], randDay2, randDay2) == -1) // 각 그룹을 이동했을때 수업이 가능한지 계산
				continue;
			if (GrpAbleTime(timeTable[randDay2][randPrd2][randGrd], randDay1, randDay1) == -1)
				continue;
			swap = timeTableSrc[randDay1][randPrd1][randGrd]; //가능하면 두 교시를 바꾼 후 적합도 계산
			timeTableSrc[randDay1][randPrd1][randGrd] = timeTableSrc[randDay2][randPrd2][randGrd];
			timeTableSrc[randDay2][randPrd2][randGrd] = swap;
			crtFit = FitnessCal();
			srcFit = SrcFitnessCal();
			fitDiff = srcFit - crtFit;
			if (fitDiff <= 0)
			{
				memcpy(timeTable, timeTableSrc, sizeof(timeTableSrc));
				looped++;
				continue;
			}
			else
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
				time[group[timeTable[i][j][0]]].subject[k].subcode].push_back({ i,j,0 });
			}
			for (int k = 0; k < group[timeTable[i][j][1]].subject.size(); k++)
			{
				time[group[timeTable[i][j][1]]].subject[k].subcode].push_back({ i,j,0 });

			}
		}
	}
	int randTime, subTime; currentTime = 0;
	for (int i = 0; i < subjectData.size(); i++) // 과목 무작위 할당
	{
		currentTime = 0;
		subTime = subjectData[i].time;
		while (currentTime < subTime)
		{
			randTime = rand % ()subTime;
			if (time[i][randTime].selected == 0)
			{
				subTable[time[i][randTime].day][time[i][randTime].period][subjectData[i].grade - 2].push_back(i);
				currentTime++;
			}
			else
			{
				continue;
			}
		}
	}
	int randSub, randTime1, randTime2, incTime, noIncTime;
	for (tem = 1; tem >= tFin; tem *= temDec)
	{
		randSub = rand % () subjectData.size();
		while (1)
		{
			memcpy(subTableSrc, subTable, sizeof(subTable));
			randTime1 = rand % () subjectData[randSub].time;
			randTime2 = rand % () subjectData[randSub].time;
			if (randTime1 == randTime2)
				break;
			if (time[randSub][randTime1].selected == time[randSub][randTime2].selected)
				break;
		}
		if (time[randSub][randTime1].selected == 1)
		{
			incTime = randTime1;
			noIncTime = randTime2;
		}
		else
		{
			incTime = randTime2;
			noIncTime = randTime1;
		}
		subTableSrc[time[randSub][incTime].day][time[randSub][incTime].period][subjectData[randSub].grade - 2].remove(randSub);
		subTableSrc[time[randSub][noIncTime].day][time[randSub][noIncTime].period][subjectData[randSub].grade - 2].push_back(randSub);
		crtFit = subFit();
		srcFit = SrcSubFit();
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