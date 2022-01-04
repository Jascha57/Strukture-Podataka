#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Leaderboard.h"

void LeaderboardInput(PositionL headLB, PositionL newUser)
{
	if (headLB->next == NULL)
	{
		headLB->next = newUser;
	}
	else
	{
		PositionL temp = headLB;
		while (temp->next != NULL && temp->next->Score > newUser->Score)
		{
			temp = temp->next;
		}
		newUser->next = temp->next;
		temp->next = newUser;
	}
}

//Same as with the question free fun
void FreeLeaderboardList(PositionL headLB)
{
	PositionL temp = NULL;
	while (headLB->next != NULL)
	{
		temp = headLB->next;
		headLB->next = temp->next;
		free(temp);
	}
}

void PrintLeaderboard(PositionL first)
{
	if (!first)
	{
		printf("There are no scores at the moment.\n");
	}
	else
	{
		while (first != NULL)
		{
			printf("=====================================================================\n");
			printf("Name:%s\n", first->Name);
			printf("Score:%d\n", first->Score);
			printf("=====================================================================\n");
			first = first->next;
		}
	}
}

PositionL CreateNewLeaderboardMember()
{
	PositionL newLeaderboardMember = NULL;

	newLeaderboardMember = (PositionL)malloc(sizeof(leaderboard));
	if (!newLeaderboardMember)
	{
		perror("Malloc LB failed\n");
		return NULL;
	}

	strcpy(newLeaderboardMember->Name, "");
	newLeaderboardMember->Score = 0;
	newLeaderboardMember->next = NULL;

	return newLeaderboardMember;
}