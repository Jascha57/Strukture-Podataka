#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE (1024)
#define MAX_CHAR (100)

struct _Drzave;
struct _Tree;

typedef struct _Tree* PositionT;
typedef struct _Tree
{
	int BrojStanovnika;
	char ImeGrada[MAX_CHAR];
	PositionT left;
	PositionT right;
}tree;

typedef struct _Drzave* PositionD;
typedef struct _Drzave
{
	char ImeDrzave[MAX_CHAR];
	PositionD next;
	PositionT root;
}drzave;

int ReadingFromFile(PositionD head);
int ReadingTownsFromFile(PositionD Drzava, char* imeDatoteke);

void SortedInsertDrzava(PositionD head, PositionD newDrzava);
void FreeList(PositionD head);
void Search(PositionD head);
void Ispis(PositionT root, int BrojStan);

PositionD CreateNewDrzava(char* ime);
PositionT CreateNewGrad(char* ime);
PositionT SortedInsertGrad(PositionT root, PositionT newGrad);

int main()
{
	int Check = 0;
	drzave head = { .ImeDrzave = "", .next = NULL, .root = NULL };

	Check = ReadingFromFile(&head);

	Search(head.next);

	FreeList(&head);

	return Check;
}

int ReadingFromFile(PositionD head)
{
	int Check = 0;
	char buffer[MAX_LINE]="";
	FILE* f = fopen("drzave.txt", "r");

	if (!f)
	{
		return -1;
	}

	while (!feof(f) && Check == 0)
	{
		fgets(buffer, MAX_LINE, f);
		//Miracle cure for the plague that is \n
		buffer[strcspn(buffer, "\n")] = 0;
		PositionD temp = CreateNewDrzava(buffer);
		if (!temp)
		{
			fclose(f);
			return -2;
		}
		SortedInsertDrzava(head, temp);
		fgets(buffer, MAX_LINE, f);
		buffer[strcspn(buffer, "\n")] = 0;
		Check = ReadingTownsFromFile(temp, buffer);
	}
	fclose(f);
	return 0;
}

int ReadingTownsFromFile(PositionD Drzava, char* imeDatoteke)
{
	char buffer[MAX_LINE]="";
	FILE* f = fopen(imeDatoteke, "r");

	if (f==NULL)
	{
		return -3;
	}

	while (!feof(f))
	{
		fgets(buffer, MAX_LINE, f);
		buffer[strcspn(buffer, "\n")] = 0;
		PositionT temp = CreateNewGrad(buffer);
		if (NULL == temp)
		{
			fclose(f);
			return -4;
		}
		fgets(buffer, MAX_LINE, f);
		sscanf(buffer, "%d", &temp->BrojStanovnika);
		Drzava->root = SortedInsertGrad(Drzava->root, temp);
	}
	fclose(f);
	return 0;
}

void SortedInsertDrzava(PositionD head, PositionD newDrzava)
{
	while (head->next != NULL && _strcmpi(head->next->ImeDrzave, newDrzava->ImeDrzave) > 0)
	{
		head = head->next;
	}

	newDrzava->next = head->next;
	head->next = newDrzava;
}

void FreeList(PositionD head)
{
	PositionD temp = NULL;
	while (head->next != NULL)
	{
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}
}

void Search(PositionD head)
{
	int BrojStan = 0;
	char buffer[MAX_LINE] = "";

	printf("Koju drzavu zelite vidjeti?\n");
	fgets(buffer, MAX_LINE, stdin);
	buffer[strcspn(buffer, "\n")] = 0;
	while (head != NULL && _strcmpi(buffer, head->ImeDrzave) != 0)
	{
		head = head->next;
	}
	if (head == NULL)
	{
		printf("Ne postoji drzava\n");
	}
	else
	{
		printf("Koliko stanovnika treba imati trazeni gradovi?\n");
		fgets(buffer, MAX_LINE, stdin);
		sscanf(buffer, "%d", &BrojStan);
		Ispis(head->root, BrojStan);
	}
}

void Ispis(PositionT root, int BrojStan)
{
	if (root != NULL && BrojStan <= root->BrojStanovnika)
	{
		Ispis(root->left, BrojStan);
		printf("%s \t %d\n", root->ImeGrada, root->BrojStanovnika);
		Ispis(root->right, BrojStan);
	}
	else if(root==NULL)
	{
		;
	}
	else
	{
		Ispis(root->left, BrojStan);
		printf("%s \t %d\n", root->ImeGrada, root->BrojStanovnika);
		Ispis(root->right, BrojStan);
	}
}

PositionD CreateNewDrzava(char* ime)
{
	PositionD newDrzava = NULL;

	newDrzava = (PositionD)malloc(sizeof(drzave));
	if (!newDrzava)
	{
		return NULL;
	}

	strcpy(newDrzava->ImeDrzave, ime);
	newDrzava->next = NULL;
	newDrzava->root = NULL;

	return newDrzava;
}

PositionT CreateNewGrad(char* ime)
{
	PositionT newGrad = NULL;

	newGrad = (PositionT)malloc(sizeof(tree));
	if (!newGrad)
	{
		return NULL;
	}
	strcpy(newGrad->ImeGrada, ime);
	newGrad->BrojStanovnika = 0;
	newGrad->left = NULL;
	newGrad->right = NULL;

	return newGrad;
}

PositionT SortedInsertGrad(PositionT root, PositionT newGrad)
{
	if (root == NULL)
	{
		root = newGrad;
	}
	else if (root->BrojStanovnika > newGrad->BrojStanovnika)
	{
		root->left = SortedInsertGrad(root->left, newGrad);
	}
	else if (root->BrojStanovnika <= newGrad->BrojStanovnika)
	{
		root->right = SortedInsertGrad(root->right, newGrad);
	}

	return root;
}