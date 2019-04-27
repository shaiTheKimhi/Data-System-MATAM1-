#include<cstring>
#include<cstdlib>

#include"eurovision.h"
#include"Country.h"
#include"Judge.h"
#include"list.h"

#define POINTS_LEN 10

struct eurovision_t {
	List countries;
	List judges;
};
typedef struct eurovision_t eurovision_t;

Eurovision eurovisionCreate()
{
	Eurovision eur = (Eurovision)malloc(sizeof(eurovision_t));
	if (!eur)
		return NULL;
	eur->countries = listCreate(copyCountry, freeCountry);
	eur->judges = listCreate(copyJudge, freeJudge);
	return eur;
}

void eurovisionDestroy(Eurovision eurovision)
{
	if (!eurovision)
		return;
	listClear(eurovision->countries);
	listClear(eurovision->judges);
	free(eurovision);
}
bool validName(char* name)
{
	int i = 0;
	while (name[i])
	{
		if ((name[i] < 'a' || name[i] > 'z') && name[i] != ' ')
			return false;
		i++;
	}
	return true;
}
bool stateExist(Eurovision e, int stateId)
{
	Country s = listGetFirst(e->countries);
	while (s)
	{
		if (s->id == stateId)
			return true;
		s = listGetNext(e->countries);
	}
	return false;
}

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId ,const char* stateName, 
	const char* songName)
{
	if (stateId < 0)
		return EUROVISION_INVALID_ID;
	if (!validName(stateName) || !validName(songName))
		return EUROVISION_INVALID_NAME;
	if (stateExist(eurovision, stateId))
		return EUROVISION_STATE_ALREADY_EXIST;

	Country s = (Country)malloc(sizeof(struct country_t));
	if (!s)
		return EUROVISION_OUT_OF_MEMORY;
	s->id = stateId;
	setStateName(s, stateName);
	setSongName(s, songName);
	listInsertLast(eurovision->countries, s);
	return EUROVISION_SUCCESS;
}

bool judgeExists(Eurovision e, int judgeId)
{
	Judge j = listGetFirst(e->judges);
	while (j)
	{
		if (j->id == judgeId)
			return true;
		j = listGetNext(e->judges);
	}
	return false;
}
bool judgeResValid(Eurovision e, int* res)
{
	for (int i = 0; i < POINTS_LEN; i++)
	{
		if (!stateExist(e, res[i]))
			return false;
	}
	return true;
}

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
	const char* judgeName, int *judgeResults)
{
	if (judgeId < 0)
		return EUROVISION_INVALID_ID;
	if (!validName(judgeName))
		return EUROVISION_INVALID_NAME;
	if (!judgeResValid(eurovision, judgeResults))
		return EUROVISION_STATE_NOT_EXIST;
	if (judgeExists(eurovision, judgeId))
		return EUROVISION_JUDGE_ALREADY_EXIST;

	Judge j = (Judge)malloc(sizeof(struct judge_t));
	if (!j)
		return EUROVISION_OUT_OF_MEMORY;
	j->id = judgeId;
	setJudgeName(j, judgeName);
	setJudgePoints(j, judgeResults);
	listInsertLast(eurovision->judges, j);
	return EUROVISION_SUCCESS;
	
}

void removeStateFromStates(Eurovision e, int stateId)
{
	Country s = listGetFirst(e->countries);
	while (s)
	{
		removeCountryFromVotes(e, stateId);
		s = listGetNext(e->countries);
	}
}

void removeStateFromJudges(Eurovision e, int stateId)
{
	Judge j = listGetFirst(e->judges);
	while (j)
	{
		if (judgeFindState(j, stateId))
			freeJudge(j);
		j = listGetNext(e->judges);
	}
}

EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId)
{
	if (stateId < 0)
		return EUROVISION_INVALID_ID;
	if (!stateExist(eurovision, stateId))
		return EUROVISION_STATE_NOT_EXIST;

	removeStateFromStates(eurovision, stateId);
	removeStateFromJudges(eurovision, stateId);
	return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId)
{
	if (judgeId < 0)
		return EUROVISION_INVALID_ID;
	if (!judgeExists(eurovision, judgeId))
		return EUROVISION_JUDGE_NOT_EXIST;

	Judge j = listGetFirst(eurovision->judges);
	while (j)
	{
		if (j->id == judgeId)
			freeJudge(j);
		j = listGetNext(eurovision->judges);
	}
	return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver, int stateTaker)
{
	if (stateGiver < 0 || stateTaker < 0)
		return EUROVISION_INVALID_ID;
	if (!(stateExist(eurovision, stateGiver) && stateExist(eurovision, stateTaker)))
		return EUROVISION_STATE_NOT_EXIST;
	if (stateGiver == stateTaker)
		return EUROVISION_SAME_STATE;

	Country s = listGetFirst(eurovision->countries);
	while (s)
	{
		if (s->id == stateGiver)
			CountryAddVote(s, stateTaker);
		s = listGetNext(eurovision->countries);
	}
	return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveVote(Eurovision eurovision, int stateGiver, int stateTaker)
{

}