#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_CHAR (100)

struct _Direktorij;
struct _Stog;
typedef struct _Direktorij* PositionD;
typedef struct _Stog* PositionS;

typedef struct _Direktorij
{
	char Ime[MAX_CHAR];
	PositionD Brat;
	PositionD Dijete;
}direkt;

typedef struct _Stog
{
	PositionD Parent;
	PositionS next;
}stog;

void md(PositionD current, char* imeNovogDirektorija);
void dir(PositionD current);
void FreeAllDir(PositionD root);
void FreeAllStack(PositionS stack);

PositionD Initilize(char* imeDirektorija);
PositionD cd(PositionD current, PositionS stack, char* ime);
PositionD cdUndo(PositionD current, PositionS stack);

int main()
{
	char naredba[MAX_CHAR] = "";
	char imeDirektorija[MAX_CHAR] = "";
	PositionD root = NULL;
	PositionD current = NULL;
	PositionS stack = (PositionS)malloc(sizeof(stog));
	stack->next = NULL;
	stack->Parent = NULL;

	if (stack == NULL)
	{
		return -1;
	}

	root = Initilize("C:");
	current = root;

	if (root == NULL)
	{
		return -1;
	}

	printf("==========Opcije==========\n");
	printf("md = Napravi Direkrotij\n");
	printf("dir = Ispis pod-direktorija\n");
	printf("cd = Mjenjanje direktorija\n");
	printf("cd..= Vracanje unazad 1 direktorij\n");
	printf("izlaz = izlaz iz programa\n");
	printf("==========Opcije==========\n");

	while (1)
	{
		printf("Trenutno ste u %s\>\n", current->Ime);
		printf("Vasa naredba:");
		scanf(" %s", naredba);

		if (strcmp(naredba, "md") == 0)
		{
			printf("Upisite ime novog direktorija:");
			scanf(" %s", imeDirektorija);
			md(current, imeDirektorija);
			strcpy(naredba, "");
			fflush(stdin);
		}

		else if (strcmp(naredba, "dir") ==0 )
		{
			dir(current->Dijete);
			strcpy(naredba, "");
			fflush(stdin);
		}

		else if (strcmp(naredba, "cd") == 0)
		{
			printf("Upisite ime trazenog direktorija:");
			scanf(" %s", imeDirektorija);
			current = cd(current, stack, imeDirektorija);
			strcpy(naredba, "");
			fflush(stdin);
		}

		else if (strcmp(naredba, "cd..") == 0)
		{
			current = cdUndo(current, stack);
			strcpy(naredba, "");
			fflush(stdin);
		}

		else if (strcmp(naredba, "izlaz") == 0)
		{
			FreeAllDir(root);
			FreeAllStack(stack);
			return 0;
		}
		else
		{
			printf("Naredba nije moguca u ovoj verziji programa. Donirajte milijun dolara za tu komandu\n");
			strcpy(naredba, "");
			fflush(stdin);
		}
	}
	
	return 0;
}

PositionD Initilize(char* imeDirektorija)
{
	PositionD newDir = NULL;
	newDir = (PositionD)malloc(sizeof(direkt));
	if (NULL == newDir)
	{
		printf("Greska u alokaciji\n");
		return NULL;
	}

	strcpy(newDir->Ime, imeDirektorija);
	newDir->Brat = NULL;
	newDir->Dijete = NULL;

	return newDir;
}

void md(PositionD current, char* imeNovogDirektorija)
{
	//Trebalo bi jos provjeriti duplicate
	if (current->Dijete == NULL)
	{
		current->Dijete = Initilize(imeNovogDirektorija);
	}
	else
	{
		PositionD direktorij = Initilize(imeNovogDirektorija);
		if (direktorij == NULL)
		{

		}
		else
		{
			//Nije sortirani unos samo dodaje na kraj
			PositionD temp = current->Dijete;
			while (temp->Brat != NULL)
			{
				temp = temp->Brat;
			}
			temp->Brat = direktorij;
		}
	}
}

void dir(PositionD current)
{
	printf("\n");
	while (current != NULL) {
		printf("\t-%s\n", current->Ime);
		current = current->Brat;
	}
}

void FreeAllDir(PositionD root)
{
	PositionD tempDijete = NULL;
	PositionD tempBrat = NULL;
	while (root->Dijete != NULL)
	{
		tempDijete = root->Dijete;
		while (tempDijete->Brat != NULL)
		{
			tempBrat = tempDijete->Brat;
			tempDijete->Brat = tempBrat->Brat;
			free(tempBrat);
		}
		root->Dijete = tempDijete->Dijete;
		free(tempDijete);
	}
	while (root->Brat != NULL)
	{
		tempBrat = root->Brat;
		root->Brat = tempBrat->Brat;
		free(tempBrat);
	}
	free(root);
}

void FreeAllStack(PositionS stack)
{
	PositionS temp = NULL;
	while (!stack)
	{
		temp = stack->next;
		stack->next = temp->next;
		free(temp);
	}
}

PositionD cd(PositionD current, PositionS stack, char* ime)
{
	PositionD temp = current->Dijete;
	while (temp != NULL)
	{
		if (!strcmp(temp->Ime, ime))
		{
			PositionS tempStack = NULL;
			tempStack = (PositionS)malloc(sizeof(stog));
			tempStack->Parent = current;
			tempStack->next = stack->next;
			stack->next = tempStack;
			return temp;
		}
		temp = temp->Brat;
	}
	printf("Ne postoji\n");
	return current;
}
//Posalje me u shadow realm ne znam zasto
PositionD cdUndo(PositionD current, PositionS stack)
{
	if (stack->next == NULL)
	{
		return current;
	}
	PositionS temp1 = stack;
	PositionS temp2 = stack;
	PositionD dirToReturn = NULL;
	while (temp1->next != NULL)
	{
		temp1 = temp1->next;
	}
	while (temp2->next != temp1)
	{
		temp2 = temp2->next;
	}
	temp2->next = temp1->next;
	dirToReturn = temp1->Parent;
	free(temp1);
	return dirToReturn;
}