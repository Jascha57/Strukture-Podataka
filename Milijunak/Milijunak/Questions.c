#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Questions.h"

//Adds new question to the list
void ListInput(PositionQ headQ, PositionQ newQuestion)
{
	if (headQ->next == NULL)
	{
		headQ->next = newQuestion;
	}
	else
	{
		PositionQ temp = headQ;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		newQuestion->next = temp->next;
		temp->next = newQuestion;
	}
}

//Just to free the memory
void FreeQuestionsList(PositionQ headQ)
{
	PositionQ temp = NULL;
	while (headQ->next != NULL)
	{
		temp = headQ->next;
		headQ->next = temp->next;
		free(temp);
	}
}

//Just to check if the list prints cleanly
void PrintList(PositionQ first)
{
	while (first != NULL)
	{
		printf("=====================================================================\n");
		printf("\n");
		printf(" %s\n", first->Question);
		printf(" %s\n", first->Answer);
		printf(" %s\n", first->A);
		printf(" %s\n", first->B);
		printf(" %s\n", first->C);
		printf(" %s\n", first->D);
		printf("=====================================================================\n");
		first = first->next;
	}
}

PositionQ CreateNewQuestion()
{
	PositionQ newQuestion = NULL;

	newQuestion = (PositionQ)malloc(sizeof(questions));
	if (!newQuestion)
	{
		perror("Malloc Q failed\n");
		return NULL;
	}

	strcpy(newQuestion->Question, "");
	strcpy(newQuestion->A, "");
	strcpy(newQuestion->B, "");
	strcpy(newQuestion->C, "");
	strcpy(newQuestion->D, "");
	strcpy(newQuestion->Answer, "");
	newQuestion->next = NULL;

	return newQuestion;
}