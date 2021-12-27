#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "Game.h"

int GameMenu(PositionQ headQ, PositionL headLB)
{
	//Integer for displaying different message based on if the user inputed a wrong input or something
	int Message = 0;
	//Integer for breaking the while loop for switch case
	int WhileCheck = 0;
	//Integer to make sure the game is running smoothly
	int GameCheck = 0;
	//Level char
	char Level = '0';
	//Buffer for input that cleans the trash too
	char TrashBuffer[MAX_LINE] = "";

	printf("Enter 1 for level 1\n");
	printf("Enter 2 for level 2\n");
	printf("Enter 3 for level 3\n");
	while (WhileCheck == 0)
	{
		if (Message == 0)
		{
			printf("Please enter the level you want to play:\n");
			Message++;
		}

		fgets(TrashBuffer, MAX_LINE, stdin);
		Level = TrashBuffer[0];
		strcpy(TrashBuffer, "");

		switch (Level)
		{
		case '1':
			GameCheck = GameStartUniversal(headQ, headLB, LEVEL_ONE);
			FreeQuestionsList(headQ);
			WhileCheck++;
			break;
		case '2':
			GameCheck = GameStartUniversal(headQ, headLB, LEVEL_TWO);
			FreeQuestionsList(headQ);
			WhileCheck++;
			break;
		case '3':
			GameCheck = GameStartUniversal(headQ, headLB, LEVEL_THREE);
			FreeQuestionsList(headQ);
			WhileCheck++;
			break;
		case '4':
			GameCheck = GameStartUniversal(headQ, headLB, LEVEL_DIE);
			FreeQuestionsList(headQ);
			WhileCheck++;
			break;
		case '5':
			WhileCheck++;
			break;
		default:
			printf("That value is not allowed. Please follow the rules of the game\n");
			fflush(stdin);
			break;
		}
	}
	return GameCheck;
}

int GameStartUniversal(PositionQ headQ, PositionL headLB, int NumberOfQuestions)
{
	//Integer to make sure the game runs smoothly
	int GameCheck = 0;
	//Array for the order of questions that is always 100 members and depending on level the range changes to make it universal instead of different fun
	int OrderOfQuestions[LEVEL_DIE];
	//Buffer that is going to contain the name of the file that has the question
	char Buffer[MAX_LINE] = "";
	//Joker name buffer
	char JokerName[USER_NAME_CHAR] = "";
	//Head for joker list
	joker headJ = { .Name = "", .next = NULL };

	for (int i = 0; i < LEVEL_DIE; i++)
	{
		OrderOfQuestions[i] = i;
	}

	for (int i = 0; i < NumberOfQuestions; i++)
	{
		int temp = OrderOfQuestions[i];
		int randomIndex = rand() % NumberOfQuestions;
		OrderOfQuestions[i] = OrderOfQuestions[randomIndex];
		OrderOfQuestions[randomIndex] = temp;
	}

	//Opens the question folder and inputs the data into the structure
	for (int i = 0; i < NUMBER_OF_QUESTIONS; i++)
	{
		//"Creates" the file name
		sprintf(Buffer, "question%d.txt", OrderOfQuestions[i]);
		ReadQuestionFromFile(headQ, Buffer);
	}

	for (int i = 1; i <= NUMBER_OF_JOKERS; i++)
	{
		PositionJ tempJoker = NULL;
		printf("Please enter the name of your %d. joker(Max name lenght is 10):", i);
		fgets(JokerName, USER_NAME_CHAR, stdin);
		if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0)
		{
			rewind(stdin);
			fgets(Buffer, MAX_LINE, stdin);
		}
		tempJoker = CreateNewJoker(JokerName);
		if (!tempJoker)
		{
			return MALLOC_FAIL;
		}
		JokerInput(&headJ, tempJoker);
		strcpy(Buffer, "");
		strcpy(JokerName, "");
	}

	GameCheck = GameLogic(headQ->next, headLB, headJ.next);
	FreeJokerList(&headJ);

	return GameCheck;
}

int ReadQuestionFromFile(PositionQ headQ, char* FileName)
{
	//Array to randomize the answer input
	int AnswerOrder[ARRAY_FOR_ANSWERS] = { 2,3,4,5 };
	//Buffer to store the string and input it to the list
	char Buffer[MAX_LINE] = "";
	//New question list
	PositionQ newQuestion = NULL;
	//File pointer for the question
	FILE* QuestionFolder = NULL;

	newQuestion = CreateNewQuestion();
	if (!newQuestion)
	{
		return MALLOC_FAIL;
	}

	QuestionFolder = fopen(FileName, "r");
	if (!QuestionFolder)
	{
		return FILE_NOT_FOUND;
	}

	//Randomize the array
	for (int i = 0; i < ARRAY_FOR_ANSWERS; i++)
	{
		int temp = AnswerOrder[i];
		int randomIndex = rand() % 4;
		AnswerOrder[i] = AnswerOrder[randomIndex];
		AnswerOrder[randomIndex] = temp;
	}

	//This is the question scan
	fgets(Buffer, MAX_LINE, QuestionFolder);
	strcpy(newQuestion->Question, Buffer);
	//This is to get the real answer
	fgets(Buffer, MAX_LINE, QuestionFolder);
	strcpy(newQuestion->Answer, Buffer);
	//Reset the file pointer
	fseek(QuestionFolder, 0, SEEK_SET);
	//Algorithm to read the other answers per random array
	for (int i = 0; i < ARRAY_FOR_ANSWERS; i++)
	{
		for (int j = 0; j < AnswerOrder[i]; j++)
		{
			fgets(Buffer, MAX_LINE, QuestionFolder);
		}

		fgets(Buffer, MAX_LINE, QuestionFolder);

		if (i == 0)
		{
			strcpy(newQuestion->A, Buffer);
		}
		else if (i == 1)
		{
			strcpy(newQuestion->B, Buffer);
		}
		else if (i == 2)
		{
			strcpy(newQuestion->C, Buffer);
		}
		else
		{
			strcpy(newQuestion->D, Buffer);
		}
		fseek(QuestionFolder, 0, SEEK_SET);
	}

	ListInput(headQ, newQuestion);
	fclose(QuestionFolder);
	return 0;
}

//This is the brain of the programm for the most part
int GameLogic(PositionQ first, PositionL headLB, PositionJ Joker)
{
	//Array that contains the score values of each step that is each correct answer in euros
	int ScoreBoard[SCORE_BOARD] = { 0,100,200,300,500,1000,2000,4000,8000,16000,32000,64000,125000,250000,500000,1000000 };
	//Keeps track of what score the user got
	int UserScore = ScoreBoard[0];
	//Integer that keeps track of what question it is
	int QuestionNumber = 1;
	//Checks if the answer is right or wrong. As long as this is 0, the game continues
	int AnswerCheck = 0;
	//Integer to break the while and switch case loop
	int WhileCheck = 0;
	//Integer for what message to display to the user
	int MessageCheck = 0;
	//Number of jokers that the player has the right to use
	int NumberOfJokers = NUMBER_OF_JOKERS;
	//Char for users name.
	char UserName[USER_NAME_CHAR] = "";
	//Character that the user inputs so that we can check their answer
	char UserAnswer = 'F';
	//Buffer for trash after the answer
	char TrashBuffer[MAX_LINE] = "";

	printf("Remember that you can always enter E and keep the amount you have won\n");
	printf("If you get a question wrong, you will leave with 0 euro\n");
	while (first != NULL && AnswerCheck == 0)
	{

		printf("=====================================================================\n");
		printf("You currently have %d euros\n", UserScore);
		Sleep(3000);
		printf("Here comes our %d. question\n", QuestionNumber);
		printf("\n");
		Sleep(3000);
		printf("%s \n", first->Question);
		Sleep(3000);
		printf("A: %s \n", first->A);
		Sleep(3000);
		printf("B: %s \n", first->B);
		Sleep(3000);
		printf("C: %s \n", first->C);
		Sleep(3000);
		printf("D: %s \n", first->D);
		Sleep(3000);

		if (MessageCheck == 0)
		{
			printf("What will be your answer?\n");
			printf("Answer:");
			MessageCheck++;
		}

		while (WhileCheck == 0)
		{
			//Basic user input with fgets()
			fflush(stdin);
			fgets(TrashBuffer, MAX_LINE, stdin);
			UserAnswer = TrashBuffer[0];
			strcpy(TrashBuffer, "");

			switch (UserAnswer)
			{
			case 'A':
			case 'a':
				if (_strcmpi(first->A, first->Answer) != 0)
				{
					printf(".");
					Sleep(1000);
					printf(".");
					Sleep(1000);
					printf(".\n");
					Sleep(1000);
					printf("I am sorry but you are walking away with 0 euros. Try again.\n");
					UserScore = 0;
					Sleep(1000);
					AnswerCheck++;
					WhileCheck++;
				}
				else
				{
					printf(".");
					Sleep(1000);
					printf(".");
					Sleep(1000);
					printf(".\n");
					Sleep(1000);
					printf("That is the correct answer. Congratulations on your %d. question\n", QuestionNumber);
					UserScore = ScoreBoard[QuestionNumber];
					WhileCheck++;
				}
				break;
			case 'B':
			case 'b':
				if (_strcmpi(first->B, first->Answer) != 0)
				{
					printf(".");
					Sleep(1000);
					printf(".");
					Sleep(1000);
					printf(".\n");
					Sleep(1000);
					printf("I am sorry but you are walking away with 0 euros. Try again.\n");
					UserScore = 0;
					Sleep(1000);
					AnswerCheck++;
					WhileCheck++;
				}
				else
				{
					printf(".");
					Sleep(1000);
					printf(".");
					Sleep(1000);
					printf(".\n");
					Sleep(1000);
					printf("That is the correct answer. Congratulations on your %d. question\n", QuestionNumber);
					UserScore = ScoreBoard[QuestionNumber];
					WhileCheck++;
				}
				break;
			case 'C':
			case 'c':
				if (_strcmpi(first->C, first->Answer) != 0)
				{
					printf(".");
					Sleep(1000);
					printf(".");
					Sleep(1000);
					printf(".\n");
					Sleep(1000);
					printf("I am sorry but you are walking away with 0 euros. Try again.\n");
					UserScore = 0;
					Sleep(1000);
					AnswerCheck++;
					WhileCheck++;
				}
				else
				{
					printf(".");
					Sleep(1000);
					printf(".");
					Sleep(1000);
					printf(".\n");
					Sleep(1000);
					printf("That is the correct answer. Congratulations on your %d. question\n", QuestionNumber);
					UserScore = ScoreBoard[QuestionNumber];
					WhileCheck++;
				}
				break;
			case 'D':
			case 'd':
				if (_strcmpi(first->D, first->Answer) != 0)
				{
					printf(".");
					Sleep(1000);
					printf(".");
					Sleep(1000);
					printf(".\n");
					Sleep(1000);
					printf("I am sorry but you are walking away with 0 euros. Try again.\n");
					UserScore = 0;
					Sleep(1000);
					AnswerCheck++;
					WhileCheck++;
				}
				else
				{
					printf(".");
					Sleep(1000);
					printf(".");
					Sleep(1000);
					printf(".\n");
					Sleep(1000);
					printf("That is the correct answer. Congratulations on your %d. question\n", QuestionNumber);
					UserScore = ScoreBoard[QuestionNumber];
					WhileCheck++;
				}
				break;
			case 'E':
			case 'e':
				printf("You decided to take %d euros\n", UserScore);
				AnswerCheck++;
				WhileCheck++;
				break;
			case 'J':
			case 'j':
				if (NumberOfJokers > 0)
				{
					JokerAnswer(first, Joker);
					Joker = Joker->next;
					NumberOfJokers--;
				}
				else
				{
					printf("I am afraid you have used all of your jokers. You are on your own.\n");
				}
				printf("What will be your answer?\n");
				printf("Answer:");
				break;
			default:
				printf("That is not possible. Please enter either A,B,C,D,J or E\n");
				printf("Answer:");
				break;
			}
		}

		first = first->next;
		MessageCheck = 0;
		QuestionNumber++;
		WhileCheck = 0;

	}

	if (UserScore > 0)
	{
		printf("Please enter you user name to save it in the leaderboard(Max name lenght is 10):");
		fgets(UserName, USER_NAME_CHAR, stdin);
		if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0)
		{
			rewind(stdin);
			fgets(TrashBuffer, MAX_LINE, stdin);
		}
		PositionL newUser = CreateNewLeaderboardMember();
		if (!newUser)
		{
			return MALLOC_FAIL;
		}
		strcpy(newUser->Name, UserName);
		newUser->Score = UserScore;
		LeaderboardInput(headLB, newUser);
	}

	return 0;
}