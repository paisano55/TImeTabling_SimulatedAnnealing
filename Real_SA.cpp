#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <memory.h>
using namespace std;
int timeTable[5][9][2] // -1 : �Ҵ�� �׷����
int timeTableSrc[5][9][2] // Save the code of Searched Group
typedef struct subjectbase
{
	vector<int> student;
	int subcode, floor, x, time, y;
	vector <int> teacher;
}subjectBase;
typedef struct groupbase
{
	vector<subjectBase> subject;
	int groupCode;
	int maxTime;
	int currentTime;
	int grade;
	vector<int> student;
	vector<int> teacher;
}groupBase;

vector<groupBase> group;

int DistCal(int dowi, int periodi, int dowf, int periodf) // �Ÿ���� �������� ���θ���!
{
	int i, j, k, moving, temp;
	int stuInfo;
	int howmansub, howmanstu, whichsmall;
	int howmuctime[60] = { 0 };
	int totaltime = 0;
	vector<int> student_list; 
	vector<int> iniclass_student;
	vector<int> finclass_student;
	struct position
	{
		int floor, x, y;
	};//��, ���� �� ������ x,y ���� �����Ѵ�
	vector<position> initial_position;
	vector<position> final_position;
	vector<position> stair_location;
	howmansub = group[timeTable[dowi][periodi]].subject.size();
	for (i = 0; i < howmansub; i++)
	{
		howmanstu = group[timeTable[dowi][periodi]].subject[i].student.size();
		whichsmall = howmanstu;
		for (j = 0; j < howmanstu; j++)
		{
			stuInfo = group[timeTable[dowi][periodi]].subject[i].student[j];
			iniclass_student.push_back(stuInfo);//������ � ���ÿ� �ִ� ������ �����ϴ� �л����� ����Ʈ�� ������ ����
		}
		howmanstu = group[timeTable[dowf][periodf]].subject[i].student.size();
		for (j = 0; j < howmanstu; j++)
		{
			stuInfo = group[timeTable[dowf][periodf]].subject[i].student[j];
			finclass_student.push_back(stuInfo);//������ �� ���� ���ÿ� �ִ� ������ �����ϴ� �л����� ����Ʈ�� ������ ����
		}
		if (whichsmall < howmanstu) howmanstu = whichsmall;
		//���� �� ���� ���� ������ �����ϴ� �л����� ���� ���� �л� ������ ���ٸ� ���� ���� �л����� ���� �ð��� �����̶�� ���̹Ƿ� �� �л����� ��꿡 ���� �ʱ� ����
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < howmanstu; k++)
			{
				if (iniclass_student[j] == finclass_student[k])
				{
					student_list.push_back(finclass_student[k]);
				}
			}
		}//� �ð��� �� ���� �ð� ��� ������ ��� �л��鸸 �߷��� ����Ʈ�� ������ ����
		initial_position[i].floor = group[timeTable[dowi][periodi]].subject[i].floor;
		initial_position[i].x = group[timeTable[dowi][periodi]].subject[i].x;
		initial_position[i].y = group[timeTable[dowi][periodi]].subject[i].y;//�л����� �ʱ� ��ġ ���� 
		final_position[i].floor = group[timeTable[dowf][periodf]].subject[i].floor;
		final_position[i].x = group[timeTable[dowf][periodf]].subject[i].x;
		final_position[i].y = group[timeTable[dowf][periodf]].subject[i].y;//�л����� �̵� ��ġ ���� 
	}
	stair_location[0].x = 0; stair_location[0].y = 0;
	stair_location[1].x = 5; stair_location[1].y = 1;
	stair_location[1].x = -7; stair_location[1].y = 1;
	stair_location[1].x = -3; stair_location[1].y = 5;
	stair_location[1].x = -2; stair_location[1].y = -4;//�ʱ� ��� ��ġ ����
	for (i = 0; i < student_list.size(); i++)
	{
		totaltime += (initial_position[i].floor - final_position[i].floor) * 5;//���� �̵� ����ġ�� 5
		moving = 100;
		for (i = 0; i < 5; i++)
		{
			temp = abs(stair_location[i].x - initial_position[i].x) + abs(stair_location[i].y - initial_position[i].y);
			temp = abs(stair_location[i].x - final_position[i].x) + abs(stair_location[i].y - final_position[i].y);
			if (moving > temp) moving = temp;
		}
		totaltime += moving;
	}
	return totaltime;
}

int DowTotDist()
{
	int Dtotaltime = 0;
	int dow, period;
	for (dow = 0; dow < 5; dow++)
	{
		for (period = 0; period < 5; period++)
		{
			Dtotaltime += DistCal(dow, period, dow, period + 1);
		}
	}
	return Dtotaltime;
}
int SrcDistCal(int dowi, int periodi, int dowf, int periodf) // SA �̿��� �� �Ÿ����
{
	int i, j, k, moving, temp;
	int stuInfo;
	int howmansub, howmanstu, whichsmall;
	int howmuctime[60] = { 0 };
	int totaltime = 0;
	vector<int> student_list;
	vector<int> iniclass_student;
	vector<int> finclass_student;
	struct position
	{
		int floor, x, y;
	};//��, ���� �� ������ x,y ���� �����Ѵ�
	vector<position> initial_position;
	vector<position> final_position;
	vector<position> stair_location;
	howmansub = group[timeTableSrc[dowi][periodi]].subject.size();
	for (i = 0; i < howmansub; i++)
	{
		howmanstu = group[timeTableSrc[dowi][periodi]].subject[i].student.size();
		whichsmall = howmanstu;
		for (j = 0; j < howmanstu; j++)
		{
			stuInfo = group[timeTableSrc[dowi][periodi]].subject[i].student[j];
			iniclass_student.push_back(stuInfo);//������ � ���ÿ� �ִ� ������ �����ϴ� �л����� ����Ʈ�� ������ ����
		}
		howmanstu = group[timeTableSrc[dowf][periodf]].subject[i].student.size();
		for (j = 0; j < howmanstu; j++)
		{
			stuInfo = group[timeTableSrc[dowf][periodf]].subject[i].student[j];
			finclass_student.push_back(stuInfo);//������ �� ���� ���ÿ� �ִ� ������ �����ϴ� �л����� ����Ʈ�� ������ ����
		}
		if (whichsmall < howmanstu) howmanstu = whichsmall;
		//���� �� ���� ���� ������ �����ϴ� �л����� ���� ���� �л� ������ ���ٸ� ���� ���� �л����� ���� �ð��� �����̶�� ���̹Ƿ� �� �л����� ��꿡 ���� �ʱ� ����
		for (j = 0; j < howmanstu; j++)
		{
			for (k = 0; k < howmanstu; k++)
			{
				if (iniclass_student[j] == finclass_student[k])
				{
					student_list.push_back(finclass_student[k]);
				}
			}
		}//� �ð��� �� ���� �ð� ��� ������ ��� �л��鸸 �߷��� ����Ʈ�� ������ ����
		initial_position[i].floor = group[timeTableSrc[dowi][periodi]].subject[i].floor;
		initial_position[i].x = group[timeTableSrc[dowi][periodi]].subject[i].x;
		initial_position[i].y = group[timeTableSrc[dowi][periodi]].subject[i].y;//�л����� �ʱ� ��ġ ���� 
		final_position[i].floor = group[timeTableSrc[dowf][periodf]].subject[i].floor;
		final_position[i].x = group[timeTableSrc[dowf][periodf]].subject[i].x;
		final_position[i].y = group[timeTableSrc[dowf][periodf]].subject[i].y;//�л����� �̵� ��ġ ���� 
	}
	stair_location[0].x = 0; stair_location[0].y = 0;
	stair_location[1].x = 5; stair_location[1].y = 1;
	stair_location[1].x = -7; stair_location[1].y = 1;
	stair_location[1].x = -3; stair_location[1].y = 5;
	stair_location[1].x = -2; stair_location[1].y = -4;//�ʱ� ��� ��ġ ����
	for (i = 0; i < student_list.size(); i++)
	{
		totaltime += (initial_position[i].floor - final_position[i].floor) * 5;//���� �̵� ����ġ�� 5
		moving = 100;
		for (i = 0; i < 5; i++)
		{
			temp = abs(stair_location[i].x - initial_position[i].x) + abs(stair_location[i].y - initial_position[i].y);
			temp = abs(stair_location[i].x - final_position[i].x) + abs(stair_location[i].y - final_position[i].y);
			if (moving > temp) moving = temp;
		}
		totaltime += moving;
	}
	return totaltime;
}

int SrcDowTotDist() // SA �̿��ؿ� �� �Ÿ����
{
	int Dtotaltime = 0;
	int dow, period;
	for (dow = 0; dow < 5; dow++)
	{
		for (period = 0; period < 5; period++)
		{
			Dtotaltime += DistCal(dow, period, dow, period + 1);
		}
	}
	return Dtotaltime;
}
int SrcFit


int main()
{
	srand(time(NULL));
	int randGroup = 0;
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
				(group[i].student).push_back(group[i].subject[j].student[k])
			}
			for (int k=0;)
		}
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
		for (int i = 0; i < 5; i++) // Random Tabling
		{
			while (1) {
				randGroup = rand() % (group.size());
				if (group[randGroup].maxTime >= group[randGroup].currentTime + 1)
				{
					group[randGroup].currentTime++;
					timeTable[i][0] = randGroup;
					break;
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
			for (int groupGreed = 0; groupGreed < group.size(); groupGreed++)
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
	double tem = 1, temDec = 0.8, tFin = 0.01;
	int loop = 5000, looped, randDay1, randDay2, randPrd1, randPrd2, randGrd, swap, srcDist, crtDist, distDiff;
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
			swap = timeTableSrc[randDay1][randPrd1][randGrd];
			timeTableSrc[randDay1][randPrd1][randGrd] = timeTableSrc[randDay2][randPrd2][randGrd];
			timeTableSrc[randDay2][randPrd2][randGrd] = swap;
			crtDist = DowTotDist();
			srcDist = SrcDowTotDist();
			distDiff = srcDist - crtDist;
			if (distDiff <= 0)
			{
				memcpy(timeTable, timeTableSrc, sizeof(timeTableSrc));
				continue;
			}
			else
			{
				double random;
				random = (1 / ((rand() % 1000) + 1));
				if (exp((float)((-distDiff) / (tem))) >= random)
				{
					memcpy(timeTable, timeTableSrc, sizeof(timeTableSrc));
					continue;
				}
				else 
				{
					memcpy(timeTableSrc, timeTable, sizeof(timeTable));
				}
			}
		}
	}
	printf("Simulated Annealing Done!");
	for (int i = 0, i < 6; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (timeTable[j][i] == -1)
			{
				fprintf(table, "0 ");
			}
			else
			{
				fprintf(table, "%d ", group[timeTable[j][i]].groupCode);
			}
		}
		fprintf(table, "\n");
	}
	fprintf(table, "�� �̵� �Ÿ� : %d \n", DowTotDist());
}