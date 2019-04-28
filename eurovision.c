#include<cstring>
#include<cstdlib>

#include"eurovision.h"
#include"Country.h"
#include"Judge.h"
#include"list.h"

#define POINTS_LEN 10
#define FULL_PERCENT 100
#define TWO 2

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
	eur->countries = listCreate(&copyCountry, &freeCountry);
	eur->judges = listCreate(&copyJudge, &freeJudge);
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

char* copyString(char* string)
{
	int len = strlen(string);
	char* new_string = (char*)malloc(sizeof(char) * len);
	strcpy(new_string, string);
	return new_string;
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
	if (stateGiver < 0 || stateTaker < 0)
		return EUROVISION_INVALID_ID;
	if (!stateExist(eurovision, stateGiver) || !stateExist(eurovision, stateTaker))
		return EUROVISION_STATE_NOT_EXIST;
	if (stateGiver == stateTaker)
		return EUROVISION_SAME_STATE;
	
	Country s = listGetFirst(eurovision->countries);
	while (s)
	{
		if (s->id == stateGiver)
			countryRemoveVote(s, stateTaker);
		s = listGetNext(eurovision->countries);
	}
	return EUROVISION_SUCCESS;
}

float scoreStates(Eurovision eur, int stateId)
{
	float sum = 0;
	Country s = listGetFirst(eur->countries);
	while (s)
	{
		sum += countryGetScore(s, stateId);
		s = listGetNext(eur->countries);
	}
	return sum / listGetSize(eur->countries);
}
float scoreJudges(Eurovision eur, int stateId)
{
	float sum = 0;
	Judge j = listGetFirst(eur->judges);
	while (j)
	{
		sum += getJudgeScore(j, stateId);
		j = listGetNext(eur->judges);
	}
	return sum / listGetSize(eur->judges);
}

float calculateScore(float aud, float jug, int audiencePercent)
{
	int aPer = aud * audiencePercent;
	int jPer = jug * (FULL_PERCENT - audiencePercent);
	return (aPer + jPer) / TWO * FULL_PERCENT;
}

void initStatesArrays(Eurovision eur, int* ids, float* points, int audiencePercent)//ids is id array and points is points array
{
	int count = 0;
	Country s = listGetFirst(eur->countries);
	while (s)
	{
		ids[count] = s->id;
		points[count] = calculateScore(scoreStates(eur, s->id), 
			scoreJudges(eur, s->id), audiencePercent);
		s = listGetNext(eur->countries);
	}
}
void sortStates(int* ids, int* points, int len)//ids is id array and points is points array
{
	int temp;
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = 0; j < len - i - 1; i++)
		{
			if (points[j] > points[j + 1])
			{
				temp = points[j + 1];
				points[j + 1] = points[j];
				points[j] = temp;

				temp = ids[j + 1];
				ids[j + 1] = ids[j];
				ids[j] = temp;
			}
		}
	}
}
Country getStateById(Eurovision eur, int stateId)
{
	Country s = listGetFirst(eur->countries);
	while (s)
	{
		if (s->id == stateId)
			return s;
		s = listGetNext(eur->countries);
	}
}
List getStates(Eurovision eur, int* ids, int len)
{
	List states = listCreate(&copyString, &free);
	for (int i = len - 1; i >= 0; i--)
	{
		listInsertLast(states, getStateName(getStateId(eur, ids[i])));
	}
	return states;
}

List eurovisionRunContest(Eurovision eurovision, int audiencePercent)
{
	int* ids, len = listGetSize(eurovision->countries);
	float* points;//parr represents points arr
	
	ids = (int*)malloc(sizeof(int) * len);
	points = (float*)malloc(sizeof(float) * len);

	initStatesArrays(eurovision, ids, points, audiencePercent);

	sortStates(ids, points, len);
	free(ids);
	free(points);
	return getStates(eurovision, ids, len);
}

List eurovisionRunAudienceFavorite(Eurovision eurovision)
{
	return eurovisionRunContest(eurovision, FULL_PERCENT);
}

char** initFriendlyArray(Eurovision eur)
{
	int len = listGetSize(eur->countries), count = 0;
	char** friends_array = (char**)malloc(sizeof(char*) * len), *name;
	Country s = listGetFirst(eur->countries), temp;
	while (s)
	{
		temp = getStateById(eur, countryGetFriendlyState(s));
		name = getStateName(s);
		friends_array[count] = (char*)malloc(sizeof(char) * strlen(name));
		//might switch sides
		strcpy(friends_array[count], name);
		//put state-friend at friends array
		strcat(friends_array[count], "-");
		strcat(friends_array[count++],getStateName(temp));
		s = listGetNext(eur->countries);
	}
	return friends_array;
}
void sortFriendlyArray(char** friends_array, int len)
{
	char* temp_string;
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = 0; j < len - i - 1; j++)
		{
			if (strcmp(friends_array[j], friends_array[j + 1]) > 0)
			{
				temp_string = friends_array[j + 1];
				friends_array[j + 1] = friends_array[j];
				friends_array[j] = temp_string;
			}
		}
	}
}

List eurovisionRunGetFriendlyStates(Eurovision eurovision)
{
	int len = listGetSize(eurovision->countries);
	char** friends_array = initFriendlyArray(eurovision);
	List friends_list = listCreate(&copyString, &free);
	sortFriendlyArray(friends_array, len);
	for (int i = len - 1; i >= 0; i++)
	{
		listInsertLast(friends_list, friends_array[i]);
		free(friends_array[i]);
	}
	free(friends_array);
	return friends_list;
}
