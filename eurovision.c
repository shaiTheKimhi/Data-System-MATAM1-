#include<cstring>
#include<cstdlib>

#include"eurovision.h"
#include"Country.h"
#include"Judge.h"
#include"list.h"

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

void eurovisionDestroy(Eurovision eurovision);
{
	if (!eurovision)
		return;
	listClear(eurovision->countries);
	listClear(eurovision->judges);
	free(eurovision);
}

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId ,const char* stateName, 
	const char* songName)
{
	Country s = (Country)malloc(sizeof(struct country_t));

}
