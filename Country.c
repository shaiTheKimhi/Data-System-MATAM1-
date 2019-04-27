#include<cstdlib>//replace with stdlib.h
#include<cstring>//replace to string.h

#include"Country.h"
#include"list.h"

#define HALF_INTEGER 65536
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



//These three functuons are for internal use
int getIdOfVote(int vote)
{
	return vote / HALF_INTEGER;
}
int getSumOfVote(int vote)
{
	return vote % HALF_INTEGER;
}
int VoteId(int id)
{
	return id * HALF_INTEGER;
}


Country copyCountry(Country country)
{
	Country c = (Country)malloc(sizeof(struct country_t));
	c->id = country->id;
	strcpy(c->name, country->name);
	strcpy(c->songname, country->songname);
	c->votes = malloc(sizeof(int) * country->votes_length);
	c->votes_length = country->votes_length;
	for (int i = 0; i < c->votes_length; i++)
		c->votes[i] = country->votes[i];

	return c;
}

void freeCountry(Country country)
{
	free(country->name);
	free(country->songname);
	free(country->votes);
	free(country);
}

void removeCountryFromVotes(Country voter, int remove_id)
{
	//code here
	int len = voter->votes_length, counter = 0, vote;
	int* nvotes = (int*)malloc(sizeof(int)*len);
	for (int i = 0; i < len; i++)
	{
		vote = voter->votes[i];
		if (getIdOfVote(vote) != remove_id) {
			nvotes[counter++] = vote;
		}
	}
	free(voter->votes);
	voter->votes = nvotes;
	voter->votes_length = counter;
}

void countryRemoveVote(Country voter, int stateId)
{
	int len = voter->votes_length;
	for (int i = 0; i < len; i++)
	{
		if (getIdOfVote(voter->votes[i]) == stateId)
		{
			if (getSumOfVote(voter->votes[i]) > 0)
				voter->votes[i]--;
		}
	}
}

void CountryAddVote(Country c, int vote)
{
	int len = c->votes_length, flag = 0;
	for (int i = 0; i < len; i++)
	{
		if (getIdOfVote(c->votes[i]) == vote) {
			c->votes[i]++;
			flag = 1;
		}
	}
	if (flag)
	{
		int* arr = malloc(sizeof(int) * (++c->votes_length));
		for (int i = 0; i < c->votes_length - 1; i++)
			arr[i] = c->votes[i];
		c->votes[c->votes_length - 1] = VoteId(vote) + 1;
	}
}

void countrySortScores(Country country)
{
	int len = country->votes_length, temp;
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = i; j < len - 1; j++)
		{
			if (getSumOfVote(country->votes[i]) > getSumOfVote(country->votes[i + 1]))
			{
				temp = country->votes[i + 1];
				country->votes[i + 1] = country->votes[i];
				country->votes[i] = temp;
			}
		}
	}
}

int countryGetScore(Country voter, int stateId)
{
	int len = voter->votes_length, 
		scores[S_SIZE] = { S_12, S_10, S_8, S_7, S_6, S_5, S_4, 
		S_3, S_2, S_1 };
	countrySortScores(voter);
	for (int i = len - 1; i > len - 12 && i >= 0; i--)
	{
		if (getIdOfVote(voter->votes[i]) == stateId)
			return scores[len - 1 - i];
	}
}


int getStateId(Country c)
{
	return c->id;
}
char* getStateName(Country c)
{
	return c->name;
}
char* getSongName(Country c)
{
	return c->songname;
}
void setStateId(Country c, int id)
{
	c->id = id;
}
void setStateName(Country c,const char* s)
{
	c->name = (char*)malloc(sizeof(char) * strlen(s) + 1);
	strcpy(c->name, s);
}
void setSongName(Country c,const char* s)
{
	c->songname = (char*)malloc(sizeof(char) * strlen(s) + 1);
	strcpy(c->songname, s);
}


