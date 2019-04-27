#ifndef COUNTRY_H_
#define COUNTRY_H_

#include"Country.h"
#include"list.h"

typedef struct country_t *Country;

struct country_t {
	unsigned int id;
	char* name;
	char* songname;

	int* votes;
	int votes_length;
	//votes format might change
};

Country copyCountry(Country country);
void freeCountry(Country country);
void removeCountryFromVotes(Country voter, int remove_id);
void CountryAddVote(Country c, int vote);
void countryRemoveVote(Country voter, int stateId);
int countryGetScore(Country voter, int stateId);

int getStateId(Country c);
char* getStateName(Country c);
char* getSongName(Country c);
void setStateId(Country, int);
void setStateName(Country,const char*);
void setSongName(Country, const char*);

#endif