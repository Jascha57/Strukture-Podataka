#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//Progress so far
//level 4 is still not done. Need to compose more questions.
//Still need more razzle dazzle and to add joker compatibility.
//Maybe the higher the level, the bigger the chance that the joker will fail but that might be not fun enough.
//Oh yeah and impliment the leaderboard. Maybe also make a score text file that at the start reads and forms a list then writes everything back
//So that we dont have to make a new list every time, or just append depending of preference.

//Magic numbers for different lengths
#define LEVEL_ONE (15)
#define LEVEL_TWO (30)
#define LEVEL_THREE (50)
#define LEVEL_DIE (100)
#define MAX_CHAR (100)
#define MAX_LINE (1024)
#define CHAR_FOR_OPTIONS (4)
#define ARRAY_FOR_ANSWERS (4)
#define SCORE_BOARD (16)
#define NUMBER_OF_QUESTIONS (15)

//Errors
#define SCANF_NOT_WORKING (1)
#define FILE_NOT_FOUND (2)
#define MALLOC_FAIL (3)
#define USER_EXIT (4)

struct _Questions;
struct _LeaderBoard;
typedef struct _Questions* PositionQ;
typedef struct _LeaderBoard* PositionL;
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

typedef struct _LeaderBoard
{

	int Score;
	char Name[MAX_CHAR];
	PositionL next;

}leaderboard;

int GameMenu(PositionQ headQ, PositionL headLB);
int GameStartLevel1(PositionQ headQ, PositionL headLB);
int GameStartLevel2(PositionQ headQ, PositionL headLB);
int GameStartLevel3(PositionQ headQ, PositionL headLB);
int GameStartLevel4(PositionQ headQ, PositionL headLB);
int ReadQuestionFromFile(PositionQ headQ, char* FileName);
int GameLogic(PositionQ first, PositionL headLB);

void ListInput(PositionQ headQ, PositionQ newQuestion);
void FreeQuestionsList(PositionQ headQ);
void PrintList(PositionQ first);
void DelayMessage(int number_of_seconds);

PositionQ CreateNewQuestion();

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

	do
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
			Exit++;
		}

		//Reset buffers after trash input just in case
		else
		{
			printf("Command is unusable. Please enter either yes or no.\n");
			strcpy(UserChoice, "");
			strcpy(TrashBuffer, "");
		}

	} while (Exit == 0);

	return 0;
}

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
			GameStartLevel1(headQ, headLB);
			//This is just to check that the list is good
			//PrintList(headQ->next);
			FreeQuestionsList(headQ);
			WhileCheck++;
			break;
		case '2':
			GameStartLevel2(headQ, headLB);
			//This is just to check that the list is good
			//PrintList(headQ->next);
			FreeQuestionsList(headQ);
			WhileCheck++;
			break;
		case '3':
			GameStartLevel3(headQ, headLB);
			//This is just to check that the list is good
			//PrintList(headQ->next);
			FreeQuestionsList(headQ);
			WhileCheck++;
			break;
		case '4':
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
	return 0;
}

int GameStartLevel1(PositionQ headQ, PositionL headLB)
{
	//Intiger to make sure the game runs smoothly
	int GameCheck = 0;
	//Array of numbers to be randomized and the order to choose the questions in
	int OrderOfQuestions[LEVEL_ONE];
	//Buffer that is going to contain the name of the file that has the question data
	char Buffer[MAX_LINE]="";
	
	//Initialize the array
	for (int i = 0; i < LEVEL_ONE; i++)
	{
		OrderOfQuestions[i] = i;
	}
	//Randomize the array
	for (int i = 0; i < LEVEL_ONE; i++)
	{
		int temp = OrderOfQuestions[i];
		int randomIndex = rand() % LEVEL_ONE;
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

	GameCheck = GameLogic(headQ->next, headLB);

	return 0;
}
int GameStartLevel2(PositionQ headQ, PositionL headLB)
{
	//Intiger to make sure the game runs smoothly
	int GameCheck = 0;
	//Array of numbers to be randomized and the order to choose the questions in
	int OrderOfQuestions[LEVEL_TWO];
	//Buffer that is going to contain the name of the file that has the question data
	char Buffer[MAX_LINE] = "";

	//Initialize the array
	for (int i = 0; i < LEVEL_TWO; i++)
	{
		OrderOfQuestions[i] = i;
	}
	//Randomize the array
	for (int i = 0; i < LEVEL_TWO; i++)
	{
		int temp = OrderOfQuestions[i];
		int randomIndex = rand() % LEVEL_TWO;
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

	GameCheck = GameLogic(headQ->next, headLB);

	return 0;
}
int GameStartLevel3(PositionQ headQ, PositionL headLB)
{
	//Intiger to make sure the game runs smoothly
	int GameCheck = 0;
	//Array of numbers to be randomized and the order to choose the questions in
	int OrderOfQuestions[LEVEL_THREE];
	//Buffer that is going to contain the name of the file that has the question data
	char Buffer[MAX_LINE] = "";

	//Initialize the array
	for (int i = 0; i < LEVEL_THREE; i++)
	{
		OrderOfQuestions[i] = i;
	}
	//Randomize the array
	for (int i = 0; i < LEVEL_THREE; i++)
	{
		int temp = OrderOfQuestions[i];
		int randomIndex = rand() % LEVEL_THREE;
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

	GameCheck = GameLogic(headQ->next, headLB);
	return 0;
}
int GameStartLevel4(PositionQ headQ, PositionL headLB)
{
	return 0;
}
int ReadQuestionFromFile(PositionQ headQ, char* FileName)
{
	//Array to randomize the answer input
	//Made it static in hopes that it will remember the last randomizer and randomize that instead of the same order over and over again.
	static int AnswerOrder[ARRAY_FOR_ANSWERS] = {2,3,4,5};
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
int GameLogic(PositionQ first, PositionL headLB)
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
	//Character that the user inputs so that we can check their answer
	char UserAnswer = 'F';
	//Buffer for trash after the answer
	char TrashBuffer[MAX_LINE] = "";

	printf("Remember that you can always enter E and keep the amount you got\n");
	printf("If you get a question wrong, you will leave with 0 euro\n");
	while (first != NULL && AnswerCheck == 0)
	{

		printf("=====================================================================\n");
		printf("You currently have %d euros\n", UserScore);
		DelayMessage(3);
		printf("Here comes our %d. question\n", QuestionNumber);
		printf("\n");
		DelayMessage(1);
		printf("%s \n", first->Question);
		DelayMessage(3);
		printf("A: %s \n", first->A);
		DelayMessage(3);
		printf("B: %s \n", first->B);
		DelayMessage(3);
		printf("C: %s \n", first->C);
		DelayMessage(3);
		printf("D: %s \n", first->D);
		DelayMessage(3);

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
					DelayMessage(1);
					printf(".");
					DelayMessage(1);
					printf(".\n");
					DelayMessage(1);
					printf("I am sorry but you are walking away with 0 euros. Try again.\n");
					DelayMessage(1);
					AnswerCheck++;
					WhileCheck++;
				}
				else
				{
					printf(".");
					DelayMessage(1);
					printf(".");
					DelayMessage(1);
					printf(".\n");
					DelayMessage(1);
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
					DelayMessage(1);
					printf(".");
					DelayMessage(1);
					printf(".\n");
					DelayMessage(1);
					printf("I am sorry but you are walking away with 0 euros. Try again.\n");
					DelayMessage(1);
					AnswerCheck++;
					WhileCheck++;
				}
				else
				{
					printf(".");
					DelayMessage(1);
					printf(".");
					DelayMessage(1);
					printf(".\n");
					DelayMessage(1);
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
					DelayMessage(1);
					printf(".");
					DelayMessage(1);
					printf(".\n");
					DelayMessage(1);
					printf("I am sorry but you are walking away with 0 euros. Try again.\n");
					DelayMessage(1);
					AnswerCheck++;
					WhileCheck++;
				}
				else
				{
					printf(".");
					DelayMessage(1);
					printf(".");
					DelayMessage(1);
					printf(".\n");
					DelayMessage(1);
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
					DelayMessage(1);
					printf(".");
					DelayMessage(1);
					printf(".\n");
					DelayMessage(1);
					printf("I am sorry but you are walking away with 0 euros. Try again.\n");
					DelayMessage(1);
					AnswerCheck++;
					WhileCheck++;
				}
				else
				{
					printf(".");
					DelayMessage(1);
					printf(".");
					DelayMessage(1);
					printf(".\n");
					DelayMessage(1);
					printf("That is the correct answer. Congratulations on your %d. question\n", QuestionNumber);
					UserScore = ScoreBoard[QuestionNumber];
					WhileCheck++;
				}
				break;
			case 'E':
			case 'e':
				printf("You decided to exit\n");
				return USER_EXIT;
				break;
			default:
				printf("That is not possible. Please enter either A,B,C,D or E\n");
				printf("Answer:");
				break;
			}
		}
		first = first->next;
		MessageCheck = 0;
		QuestionNumber++;
		WhileCheck = 0;
	}
	return 0;
}
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
//Since we have already ordered the questions, we add them to the end each time.
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
	while (first!=NULL)
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
//Just for extra razzle dazzle
void DelayMessage(int number_of_seconds)
{
	int mili_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
	while (clock() < start_time + mili_seconds)
	{
		;
	}
}

PositionQ CreateNewQuestion()
{
	PositionQ newQuestion = NULL;

	newQuestion = (PositionQ)malloc(sizeof(questions));
	if (!newQuestion)
	{
		perror("Malloc failed\n");
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