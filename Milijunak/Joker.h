#ifndef JOKER_H
#define JOKER_H
#include "Questions.h"
#include "MagicNumbers.h"

struct _Joker;
typedef struct _Joker* PositionJ;
typedef struct _Joker
{
	char Name[USER_NAME_CHAR];
	PositionJ next;
}joker;

void JokerInput(PositionJ headJ, PositionJ newJoker);
void FreeJokerList(PositionJ headJ);
void JokerAnswer(PositionQ Question, PositionJ Joker);

PositionJ CreateNewJoker(char* NameOfJoker);

#endif