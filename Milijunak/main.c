#define _CRT_SECURE_NO_WARNINGS
#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int main()
{
	//Used to exit the while game loop
	int Exit = 0;
	//Used to make sure the game is running smoothly
	int GameCheck = 0;
	//Buffer for trash
	char TrashBuffer[MAX_LINE] = "";
	//Used for archiving the users input
	char UserChoice[CHAR_FOR_OPTIONS]="";
	//Head elements for the Questions list and the LeaderBoard list
	questions headQ = { .A = "", .B = "", .C = "", .D = "", .Question= "", .Answer = "", .next = NULL };
	leaderboard headLB = { .Score = 0,.Name = "", .next = NULL };
	//Make sure the RNG is actually random
	srand((unsigned)time(NULL));

	while(1 && GameCheck==0)
	{
		printf("Would you like to play Who Wants To Be A Milionare?\n");
		//User inputs a string that then cleans itself up. It will still do the right thing if yes or no is followed by a sequence of letters
		//But much better that the cursed scanf()
		fgets(UserChoice, CHAR_FOR_OPTIONS, stdin);
		//Check to see if there is anything after UserChoice has been read. If there is, make sure it does not interfere with the program
		if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0)
		{
			rewind(stdin);
			fgets(TrashBuffer, MAX_LINE, stdin);
		}

		//Start the game
		if (_strcmpi(UserChoice, "yes") == 0)
		{
			GameCheck = GameMenu(&headQ, &headLB);
		}

		//End the game
		else if (_strcmpi(UserChoice, "no\n") == 0)
		{
			FreeLeaderboardList(&headLB);
			return USER_EXIT;
		}

		else if (_strcmpi(UserChoice, "lb\n")==0)
		{
			PrintLeaderboard(headLB.next);
		}

		//Reset buffers after trash input just in case
		else
		{
			printf("Command is unusable. Please enter either yes, no or lb for leaderboards.\n");
			strcpy(UserChoice, "");
			strcpy(TrashBuffer, "");
		}
	}

	return GameCheck;
}