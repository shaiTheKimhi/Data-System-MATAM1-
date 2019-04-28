#ifndef JUDGE_H_
#define JUDGE_H_

#include"Judge.h"
#include"list.h"

typedef struct judge_t * Judge;

struct judge_t {
	unsigned int id;
	char* name;
	int* points;//splits points for specific judge (unlike vote which has to be processed) and is sorted on lowering 
};


Judge copyJudge(Judge judge);
void freeJudge(Judge judge);
int getJudgeScore(Judge judge, int stateId);
bool judgeFindState(Judge j, int stateId);

int getJudgeId(Judge j);
char* getJudgeName(Judge j);
int* getJudgePoints(Judge j);
void setJudgeId(Judge, int);
void setJudgeName(Judge,const char*);
void setJudgePoints(Judge, int*);

#endif // !JUDGE_H_

