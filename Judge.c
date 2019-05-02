#include<stdlib.h>
#include<string.h>

#include"Judge.h"
#include"list.h"

#define POINTS_LEN 10

#define S_SIZE 10
#define S_12 12
#define S_10 10
#define S_8 8
#define S_7 7
#define S_6 6
#define S_5 5
#define S_4 4
#define S_3 3
#define S_2 2
#define S_1 1


Judge copyJudge(Judge judge)
{
	Judge j = (Judge)malloc(sizeof(struct judge_t));
	j->id = judge->id;
	strcpy(j->name, judge->name);
	j->points = malloc(sizeof(int) * POINTS_LEN);
	for (int i = 0; i < POINTS_LEN; i++)
	{
		j->points[i] = judge->points[i];
	}
	return j;
}

void freeJudge(Judge judge)
{
	free(judge->name);
	free(judge->points);
	free(judge);
}

int getJudgeScore(Judge judge, int stateId)
{
	int len = POINTS_LEN,
		scores[POINTS_LEN] = { S_12, S_10, S_8, S_7, S_6, S_5, S_4,
		S_3, S_2, S_1 };
	for (int i = 0; i < len; i++)
	{
		if (judge->points[i] == stateId)
			return scores[i];
	}
	return 0;
}

bool judgeFindState(Judge j, int stateId)
{
	for (int i = 0; i < POINTS_LEN; i++)
	{
		if (j->points[i] == stateId)
			return true;
	}
	return false;
}

int getJudgeId(Judge j)
{
	return j->id;
}
char* getJudgeName(Judge j)
{
	return j->name;
}
int* getJudgePoints(Judge j)
{
	return j->points;
}
void setJudgeId(Judge j , int id)
{
	j->id = id;
}
void setJudgeName(Judge j,const char* name)
{
	j->name = (char*)malloc(sizeof(char) * strlen(name) + 1);
	strcpy(j->name, name);
}
void setJudgePoints(Judge j, int* points)
{
	//might set it to new array and copy content
	j->points = points;
}




