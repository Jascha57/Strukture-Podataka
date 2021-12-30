#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct _Tree;
typedef struct _Tree* Position;
typedef struct _Tree
{
	int El;
	Position left;
	Position right;
}tree;

struct INNumbers;
typedef struct INNumbers* PositionIN;
typedef struct INNumbers
{
	int Num;
	PositionIN next;
}numbers;

int Replace(Position root);

void UpisUListuIN(Position root, PositionIN head);
void UnosUListu(PositionIN head, PositionIN newElement);
void FreeList(PositionIN head);

Position Insert(Position root, int x);
PositionIN CreateNewListMember(int x);

int main()
{
	int Numbers[10];// = { 2,5,7,8,11,1,4,2,3,7 };
	//Jer mi program dobro izracuna za sve osim root, koristim ovo kao spremnik da negiram pocetnu sumu u Replace
	int tempFix = 0;
	srand(time(0));
	for (int i = 0; i < 10; i++)
	{
		int temp = rand() % (90 - 10 + 1) + 10;
		printf("%d\t", temp);
		Numbers[i] = temp;
	}
	//Nije mi se dalo praviti jos jednu listu za replace i njezine funkcije pa sam ih samo nadodato u istu listu
	Position root = NULL;
	numbers head = { .next = NULL, .Num = 0 };
	FILE* f = fopen("numbers.txt", "w");

	for (int i = 0; i < 10; i++)
	{
		root = Insert(root, Numbers[i]);
	}

	UpisUListuIN(root, &head);
	PositionIN temp = head.next;

	while (temp != NULL)
	{
		fprintf(f, "%d\n", temp->Num);
		temp = temp->next;
	}

	fprintf(f,"Ovdje pocinje replace\n");
	tempFix = root->El;
	root->El = Replace(root);
	root->El -= tempFix;
	UpisUListuIN(root, &head);

	temp = head.next;
	while (temp != NULL)
	{
		fprintf(f, "%d\n", temp->Num);
		temp = temp->next;
	}

	FreeList(&head);
	fclose(f);

	return 0;
}

int Replace(Position root)
{
	int suma = 0;
	if (root->left != NULL && root->right != NULL)
	{
		suma += root->El;
		root->El = Replace(root->left) + Replace(root->right);
		suma += root->El;
	}
	else if (root->left == NULL && root->right != NULL)
	{
		suma += root->El;
		root->El = Replace(root->right);
		suma += root->El;
	}
	else if (root->left != NULL && root->right == NULL)
	{
		suma += root->El;
		root->El = Replace(root->left);
		suma += root->El;
	}
	else
	{
		suma += root->El;
		root->El = 0;
	}
	return suma;
}

void UpisUListuIN(Position root, PositionIN head)
{
	if (root != NULL)
	{
		UpisUListuIN(root->left,head);
		PositionIN temp = CreateNewListMember(root->El);
		UnosUListu(head, temp);
		UpisUListuIN(root->right,head);
	}
}

void UnosUListu(PositionIN head, PositionIN newElement)
{
	while (head->next != NULL)
	{
		head = head->next;
	}
	head->next = newElement;
}

void FreeList(PositionIN head)
{
	while (head->next != NULL)
	{
		PositionIN temp = head->next;
		head->next = temp->next;
		free(temp);
	}
}

Position Insert(Position root, int x)
{
	if (NULL == root)
	{
		root = (Position)malloc(sizeof(tree));
		root->El = x;
		root->left = NULL;
		root->right = NULL;
	}

	else if (x >= root->El)
	{
		root->left = Insert(root->left, x);
	}

	else if (x < root->El)
	{
		root->right = Insert(root->right, x);
	}

	return root;
}

PositionIN CreateNewListMember(int x)
{
	PositionIN newNumber = NULL;

	newNumber = (PositionIN)malloc(sizeof(numbers));
	if (!newNumber)
	{
		return NULL;
	}
	newNumber->Num = x;
	newNumber->next = NULL;
	return newNumber;
}