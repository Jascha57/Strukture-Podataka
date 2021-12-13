#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_ARRAY (20)

struct _Tree;
typedef struct _Tree* Position;
typedef struct _Tree
{
	int Element;
	Position Left;
	Position Right;
}tree;


void IspisIN(Position root);
void IspisPRE(Position root);
void IspisPOST(Position root);
void IspisLO(Position root, int brojUpisanihElemenata);

Position Input(Position root, int x);
Position Delete(Position root, int x);
Position SearchElement(Position root, int x);
Position SearchMin(Position root);
Position SearchMax(Position root);

//Functions for level order from GeeksForGeeks
Position* MakeQue(int*, int*, int);
void enQue(Position*, int*, Position);
Position dQue(Position*, int*);


int main()
{
	Position root = NULL;
	Position temp = NULL;
	int i = 0, j = 0, k = 0;
	int UserOption = 0;
	int Ispis = 0;
	//Pracenje elemenata za lako brisanje cijele liste
	int UneseniElementi[MAX_ARRAY];
	int BrojUnesenihElementata = 0;
	/// <summary>
	/// Vrlo sam svjestan da ako korisnik upise vise od 20 elementata da program nece raditi. Vrlo se lako osigura sa while()
	/// i u zagradi brojelemenata < 20 i brojacem ali za sad samo da program radi
	/// </summary>
	/// <returns></returns>

	while (1)
	{
		printf("==========Opcije==========\n");
		printf("1 = Umetanje novog elementa\n");
		printf("2 = Trazenje elementa\n");
		printf("3 = Brisanje elementa\n");
		printf("4 = Ispis\n");
		printf("5 = Izlaz\n");
		printf("Vas odabir:");
		scanf("%d", &UserOption);

		switch (UserOption)
		{
		case 1:
			printf("Unesite zeljeni broj:");
			scanf("%d", &i);
			root=Input(root, i);
			UneseniElementi[BrojUnesenihElementata] = i;
			BrojUnesenihElementata++;
			break;
		case 2:
			printf("Unesite zeljeni broj:");
			scanf("%d", &j);
			temp = SearchElement(root, j);
			if (temp != NULL)
				{
				printf("Element %d se nalazi na adresi %p\n", temp->Element, temp);
				}
			else
				{
				printf("Broj ne postoji u stablu\n");
				}
			break;
		case 3:
			printf("Unesite zeljeni broj:");
			scanf("%d", &k);
			root = Delete(root, k);
			break;
		case 4:
			printf("1=IspisIN\n");
			printf("2=IspisPRE\n");
			printf("3=IspisPOST\n");
			printf("4=IspisLO\n");
			printf("Vas odabir:");
			scanf("%d", &Ispis);
			switch (Ispis)
			{
			case 1:
				printf("INORDER:");
				IspisIN(root);
				printf("\n");
				break;
			case 2:
				printf("PREORDER:");
				IspisPRE(root);
				printf("\n");
				break;
			case 3:
				printf("POSTORDER:");
				IspisPOST(root);
				printf("\n");
				break;
			case 4:
				printf("LEVELORDER:");
				IspisLO(root, BrojUnesenihElementata);
				printf("\n");
				break;
			default:
				printf("Ta opcija nije dostupna u ovoj verziji programa\n");
				fflush(stdin);
				break;
			}
			break;
		case 5:
			for (int l = BrojUnesenihElementata-1; l >= 0; l--)
			{
				Delete(root, UneseniElementi[l]);
			}
			return 0;
			break;
		default:
			printf("Ta opcija nije dostupna u ovoj verziji programa\n");
			fflush(stdin);
			break;
		}
	}

	return 0;
}

void IspisIN(Position root)
{
	if (root != NULL)
	{
		IspisIN(root->Left);
		printf("%d", root->Element);
		IspisIN(root->Right);
	}
}
void IspisPRE(Position root)
{
	if (root != NULL)
	{
		IspisPRE(root->Right);
		printf("%d", root->Element);
		IspisPRE(root->Left);
	}
}
void IspisPOST(Position root)
{
	if (root != NULL)
	{
		IspisPOST(root->Left);
		IspisPOST(root->Right);
		printf("%d", root->Element);
	}
}
void IspisLO(Position root, int BrojUnesenihElemenata)
{
	int front;
	int rear;
	int i = 0;

	Position* que = MakeQue(&front, &rear, BrojUnesenihElemenata);
	Position temp = root;
	/// <summary>
	/// Program puca ovdje jer ne moze citati memoriju. Ne znam zasto to radi. Trebalo bi stat.
	/// Dobro ispise level order ali puca.
	/// </summary>
	/// <param name="root"></param>
	while (i<BrojUnesenihElemenata)
	{
		printf("%d", temp->Element);
		if (temp->Left)
		{
			enQue(que, &rear, temp->Left);
		}
		if (temp->Right)
		{
			enQue(que, &rear, temp->Right);
		}
		temp = dQue(que, &front);
		i++;
	}
	
}

Position Input(Position root, int x)
{
	if (root == NULL)
	{
		root = (Position)malloc(sizeof(tree));
		root->Element = x;
		root->Left = NULL;
		root->Right = NULL;
	}
	else if (x > root->Element)
	{
		root->Right = Input(root->Right, x);
	}
	else if (x < root->Element)
	{
		root->Left = Input(root->Left, x);
	}

	return root;
}

Position Delete(Position root, int x)
{
	if (root == NULL)
	{
		return NULL;
	}

	if (x == root->Element)
	{
		if (root->Left != NULL)
		{
			Position temp = SearchMax(root->Left);
			root->Element = temp->Element;
			root->Left = Delete(root->Left, temp->Element);
		}
		else if (root->Right != NULL)
		{
			Position temp = SearchMin(root->Right);	
			root->Element = temp->Element;
			root->Right = Delete(root->Right, temp->Element);
		}
		else
		{
			free(root);
			return NULL;
		}
	}
	else if (x < root->Element)
	{
		root->Left = Delete(root->Left, x);
	}
	else
	{
		root->Right = Delete(root->Right, x);
	}

	return root;

}

Position SearchElement(Position root, int x)
{
	if (root == NULL)
	{
		return NULL;
	}
	if (x < root->Element)
	{
		return SearchElement(root->Left, x);
	}
	else if (x > root->Element)
	{
		return SearchElement(root->Right, x);
	}
	else
	{
		return root;
	}
}

Position SearchMin(Position root)
{
	if (root == NULL)
	{
		return NULL;
	}

	while (root->Left != NULL)
	{
		root = root->Left;
	}
	return root;
}

Position SearchMax(Position root)
{
	if (root == NULL)
	{
		return NULL;
	}

	while (root->Right != NULL)
	{
		root = root->Right;
	}
	return root;
}


//GFG functions start here
Position* MakeQue(int* front, int* rear, int BrojUnesenihElemenata)
{
	Position* que = (Position*)malloc(sizeof(Position)*BrojUnesenihElemenata);
	*front = *rear = 0;
	return que;
}
void enQue(Position* que, int* rear, Position root)
{
	que[*rear] = root;
	(*rear)++;
}
Position dQue(Position* que, int* front)
{
	(*front)++;
	return que[*front - 1];
}