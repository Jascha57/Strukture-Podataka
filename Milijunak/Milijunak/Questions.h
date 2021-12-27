#ifndef QUESTIONS_H
#define QUESTIONS_H
#include "MagicNumbers.h"

struct _Questions;
typedef struct _Questions* PositionQ;
typedef struct _Questions
{

	char Question[MAX_CHAR];
	char A[MAX_CHAR];
	char B[MAX_CHAR];
	char C[MAX_CHAR];
	char D[MAX_CHAR];
	char Answer[MAX_CHAR];
	PositionQ next;

}questions;

void ListInput(PositionQ headQ, PositionQ newQuestion);
void FreeQuestionsList(PositionQ headQ);
void PrintList(PositionQ first);

PositionQ CreateNewQuestion();

#endif