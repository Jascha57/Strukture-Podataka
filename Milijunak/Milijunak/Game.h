#ifndef GAME_H
#define GAME_H
#include "Questions.h"
#include "Leaderboard.h"
#include "Joker.h"
#include "MagicNumbers.h"
#include "Errors.h"

int GameMenu(PositionQ headQ, PositionL headLB);
int GameStartUniversal(PositionQ headQ, PositionL headLB, int NumberOfQuestions);
int ReadQuestionFromFile(PositionQ headQ, char* FileName);
int GameLogic(PositionQ first, PositionL headLB, PositionJ headJ);

#endif