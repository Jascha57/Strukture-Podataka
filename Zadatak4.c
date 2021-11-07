#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR (100)
#define MAX_SIZE (1024)

struct Polinomi;
typedef struct Polinomi* Position;
typedef struct Polinomi {
	int coefficient;
	int exponent;
	Position next;
}pol;

int UcitajIzDatoteke(Position P1, Position P2, char* nazivdatoteke);
int UnesiSortirano(Position P, int c, int e);

void IspisPolinoma(Position P);
void MergePolinoms(Position P);
void OslobodiMemoriju(Position head);
void ZbrojPolinoma(Position P1, Position P2, Position Z);
void UmnozakPolinoma(Position P1, Position P2, Position U);

int main()
{
	int provjera = 0;
	char nazivdatoteke[MAX_CHAR] = { "" };
	pol P1 = { .coefficient = 0, .exponent = 0,.next = NULL };
	pol P2 = { .coefficient = 0, .exponent = 0,.next = NULL };
	pol Zbroj = { .coefficient = 0, .exponent = 0,.next = NULL };
	pol Umnozak = { .coefficient = 0, .exponent = 0,.next = NULL };

	//1.) Ucitaj polinome iz datoteke ----Radi
	//2.) Sortirano ih unesi u listu P1 i P2 ----Radi
	//3.) Zbroji i pomnozi ih u listu zbroj i umnozak ----Radi
	//4.) ?????? ----Radi

	printf("Unesite ime datoteke i na kraj dodajte .txt:");
	scanf(" %s", nazivdatoteke);
	provjera = UcitajIzDatoteke(&P1, &P2, nazivdatoteke);
	if (provjera == EXIT_FAILURE) {
		printf("Problem u funkciji UcitajIzDatoteke\n");
		return -1;
	}

	IspisPolinoma(P1.next);
	IspisPolinoma(P2.next);

	ZbrojPolinoma(P1.next, P2.next, &Zbroj);
	MergePolinoms(&Zbroj);

	UmnozakPolinoma(P1.next, P2.next, &Umnozak);
	MergePolinoms(&Umnozak);
	
	IspisPolinoma(Zbroj.next);
	IspisPolinoma(Umnozak.next);

	OslobodiMemoriju(&P1);
	OslobodiMemoriju(&P2);
	OslobodiMemoriju(&Zbroj);
	OslobodiMemoriju(&Umnozak);

	return 0;
}


int UcitajIzDatoteke(Position P1, Position P2, char* nazivdatoteke)
{
	int coeff = 0;
	int exp = 0;
	int duljina = 0;
	int kojipolinom = 1;
	int provjera = 0;

	FILE* f = NULL;
	char buffer[MAX_SIZE] = "";
	char* Pbuffer = NULL;
	
	f = fopen(nazivdatoteke, "r");
	if (f == NULL)
	{
		printf("Datoteka ne postoji");
		return EXIT_FAILURE;
	}
	else
	{
		printf("Sve uredu\n");
	}

	while (!feof(f))
	{
		fgets(buffer, MAX_SIZE, f);
		Pbuffer = buffer;
		while (strlen(Pbuffer) > 0) 
		{
			sscanf(Pbuffer, "%d %d %n", &coeff, &exp, &duljina);
			if (kojipolinom == 1) {
				provjera=UnesiSortirano(P1, coeff, exp);
				if (provjera == EXIT_FAILURE) {
					printf("Greska u UnesiSortirano\n");
					fclose(f);
					return EXIT_FAILURE;
				}
			}
			else if (kojipolinom == 2) {
				provjera=UnesiSortirano(P2, coeff, exp);
				if (provjera == EXIT_FAILURE) {
					printf("Greska u UnesiSortirano\n");
					fclose(f);
					return EXIT_FAILURE;
				}
			}
			Pbuffer += duljina;
		}
		kojipolinom++;
	}

	MergePolinoms(P1);
	MergePolinoms(P2);

	fclose(f);
	return EXIT_SUCCESS;
}

int UnesiSortirano(Position P, int c, int e)
{
	//Moze se i u ovoj funkciji provjeriti dali su eksponenti jednaki i zbrojiti ih pa ne treba praviti novu listu
	//Probat cu poslije
	Position Q=NULL;
	Q = (Position)malloc(sizeof(pol));
	
	if (Q == NULL)
	{
		printf("Greska alociranja memorije\n");
		return EXIT_FAILURE;
	}
	if (P->next == NULL)
	{
		Q->coefficient = c;
		Q->exponent = e;
		P->next = Q;
		Q->next = NULL;

		return EXIT_SUCCESS;
	}

	while (P->next != NULL && P->next->exponent > e)
	{
		P = P->next;
	}
	if (P->next == NULL)
	{
		Q->coefficient = c;
		Q->exponent = e;
		P->next = Q;
		Q->next = NULL;
	}
	else if (P->next != NULL)
	{
		Q->coefficient = c;
		Q->exponent = e;
		Q->next = P->next;
		P->next = Q;
	}

	return EXIT_SUCCESS;
}

void IspisPolinoma(Position P)
{
	while (P != NULL)
	{
		printf("%d^%d", P->coefficient, P->exponent);
		P = P->next;
		if (P != NULL)
		{
			printf("+");
		}
	}
	printf("\n");
}


void MergePolinoms(Position P)
{
	Position temp1 = P->next;
	Position izbrisi = NULL;

	while (temp1 != NULL && temp1->next != NULL)
	{
		if (temp1->exponent == temp1->next->exponent)
		{
			temp1->coefficient = temp1->coefficient + temp1->next->coefficient;
			izbrisi = temp1->next;
			temp1->next = izbrisi->next;
			free(izbrisi);
		}
		else
		{
			temp1 = temp1->next;
		}
	}
}



void OslobodiMemoriju(Position head)
{
	Position temp = NULL;

	if (head->next != NULL)
	{
		temp = head->next;
		if (NULL == temp->next) {
			free(temp);
		}
		else {
			while (head->next != NULL) {
				temp = head->next;
				head->next = temp->next;
				free(temp);
			}
		}
	}
}


void ZbrojPolinoma(Position P1, Position P2, Position Z)
{
	while (NULL != P1 && NULL != P2)
	{
		if (P1->exponent == P2->exponent)
		{
			Z->coefficient = P1->coefficient + P2->coefficient;
			Z->exponent = P1->exponent;
			UnesiSortirano(Z, Z->coefficient, Z->exponent);
			P1 = P1->next;
			P2 = P2->next;
		}
		else if (P1->exponent < P2->exponent) {
			Z->coefficient = P2->coefficient;
			Z->exponent = P2->exponent;
			UnesiSortirano(Z, Z->coefficient, Z->exponent);
			P2 = P2->next;
		}
		else if (P1->exponent > P2->exponent) {
			Z->coefficient = P1->coefficient;
			Z->exponent = P1->exponent;
			UnesiSortirano(Z, Z->coefficient, Z->exponent);
			P1 = P1->next;
		}
	}

	if (P1 == NULL) {
		while (P2 != NULL) {
			Z->coefficient = P2->coefficient;
			Z->exponent = P2->exponent;
			UnesiSortirano(Z, Z->coefficient, Z->exponent);
			P2 = P2->next;
		}
	}
	else if (P2 == NULL) {
		while (P1 != NULL)
		{
			Z->coefficient = P1->coefficient;
			Z->exponent = P1->exponent;
			UnesiSortirano(Z, Z->coefficient, Z->exponent);
			P1 = P1->next;
		}
	}

}

void UmnozakPolinoma(Position P1, Position P2, Position U)
{
	Position temp1 = P1->next;
	Position temp2 = P2->next;

	while (temp1 != NULL)
	{
		while (temp2 != NULL)
		{
			U->coefficient = temp1->coefficient * temp2->coefficient;
			U->exponent = temp1->exponent + temp2->exponent;
			UnesiSortirano(U, U->coefficient, U->exponent);
			temp2 = temp2->next;
		}
		temp2 = P2->next;
		temp1 = temp1->next;
	}
}