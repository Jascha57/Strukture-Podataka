#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include "MagicNumbers.h"

struct _LeaderBoard;
typedef struct _LeaderBoard* PositionL;

typedef struct _LeaderBoard
{

	int Score;
	char Name[USER_NAME_CHAR];
	PositionL next;

}leaderboard;

void LeaderboardInput(PositionL headLB, PositionL newUser);
void FreeLeaderboardList(PositionL headLB);
void PrintLeaderboard(PositionL first);

PositionL CreateNewLeaderboardMember();

#endif