#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Joker.h"

void JokerInput(PositionJ headJ, PositionJ newJoker)
{
	if (headJ->next == NULL)
	{
		headJ->next = newJoker;
	}
	else
	{
		PositionJ temp = headJ;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = newJoker;
	}
}

void FreeJokerList(PositionJ headJ)
{
	PositionJ temp = NULL;
	while (headJ->next != NULL)
	{
		temp = headJ->next;
		headJ->next = temp->next;
		free(temp);
	}
}

void JokerAnswer(PositionQ Question, PositionJ Joker)
{
	//Integer that determines what answer the joker will give
	int WhatAnswer = rand() % 4;

	printf("%s\n", Joker->Name);
	printf("=====================================================================\n");
	if (WhatAnswer == 0)
	{
		if (_strcmpi(Question->Answer, Question->A) == 0)
		{
			printf("The answer is %sI am absolutely sure of it. Good luck.\n", Question->A);
		}
		else
		{
			printf("I think the answer is %sbut I am not absolutely sure. Good luck.\n", Question->A);
		}
	}
	else if (WhatAnswer == 1)
	{
		if (_strcmpi(Question->Answer, Question->B) == 0)
		{
			printf("The answer is %sI am absolutely sure of it. Good luck.\n", Question->B);
		}
		else
		{
			printf("I think the answer is %sbut I am not absolutely sure. Good luck.\n", Question->B);
		}
	}
	else if (WhatAnswer == 2)
	{
		if (_strcmpi(Question->Answer, Question->C) == 0)
		{
			printf("The answer is %sI am absolutely sure of it. Good luck.\n", Question->C);
		}
		else
		{
			printf("I think the answer is %sbut I am not absolutely sure. Good luck.\n", Question->C);
		}
	}
	else if (WhatAnswer == 3)
	{
		if (_strcmpi(Question->Answer, Question->D) == 0)
		{
			printf("The answer is %sI am absolutely sure of it. Good luck.\n", Question->D);
		}
		else
		{
			printf("I think the answer is %sbut I am not absolutely sure. Good luck.\n", Question->D);
		}
	}
	printf("=====================================================================\n");
}

PositionJ CreateNewJoker(char* NameOfJoker)
{
	PositionJ newJoker = NULL;

	newJoker = (PositionJ)malloc(sizeof(joker));
	if (!newJoker)
	{
		return NULL;
	}
	strcpy(newJoker->Name, NameOfJoker);
	newJoker->next = NULL;

	return newJoker;
}