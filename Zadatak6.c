#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CHAR (100)
#define MAX_SIZE (1024)

struct Racuni;
struct Artikli;
typedef struct Racuni* PositionR;
typedef struct Artikli* PositionA;

typedef struct Racuni
{
	char ImeRacuna[MAX_CHAR];
	int God;
	int Mjesec;
	int Dan;
	PositionR nextR;
	PositionA nextAr;
}racuni;

typedef struct Artikli
{
	char Ime[MAX_CHAR];
	int Kolicina;
	int Cijena;
	PositionA nextA;
}artikli;

/// <summary>
/// 1.) Otvori datoteku //Done
/// 2.) Procitaj ime racuna //Done
/// 3.) Napravi novi element i dodaj ga u listu //Done
/// 4.) Nakon sto je lista racuna gdje su imena imena datoteka napravljena, otvori po redu i uspisi artikle i isto posalji poziciju originala da se datum upise 
/// 5.) Sortiraj Po datumu pa onda abecedno artikle
/// 6.) Ostalo
/// </summary>
/// <returns></returns>


int CitanjeRacunaIzDatoteke(PositionR head, char* imedatoteke);
int CitanjeArtikalaIzDatoteke(PositionR head);
void UnosUListu(PositionR head, PositionR temp);
void UnosUListuArtikala(PositionR head, PositionA newArtikal);
void OslobodiMemoriju(PositionR head);
void IspisiRacune(PositionR first);
void IspisiArtikle(PositionA first);
void SortirajListuPoDatumu(PositionR head);
PositionR CreateNewRacun(char* imeracuna);
PositionA CreateNewArtikl(char* imeArtikla, int kolicina, int cijena);

int main()
{
	char imedatoteke[] = "racuni.txt";
	racuni head = { .Dan = 0, .God = 0, .ImeRacuna = "", .Mjesec = 0, .nextAr = NULL, .nextR = NULL };

	CitanjeRacunaIzDatoteke(&head, imedatoteke);
	CitanjeArtikalaIzDatoteke(&head);

	IspisiRacune(head.nextR);

	OslobodiMemoriju(&head);
	return 0;
}

int CitanjeRacunaIzDatoteke(PositionR head, char* imedatoteke)
{
	char buffer[MAX_SIZE] = "";
	char tempIme[MAX_CHAR] = "";
	PositionR temp = NULL;
	FILE* f = NULL;

	f = fopen(imedatoteke, "r");
	if (NULL == f)
	{
		perror("Greska datoteka ne postoji\n");
		return EXIT_FAILURE;
	}

	while (!feof(f))
	{
		fgets(buffer, MAX_SIZE, f);
		sscanf(buffer, " %s", tempIme);
		temp = CreateNewRacun(tempIme);
		UnosUListu(head, temp);
	}

	fclose(f);
	return 0;
}
//Trebam jos osigurati da ako je red poslije datuma prazan da ne pravi praznu listu.
int CitanjeArtikalaIzDatoteke(PositionR head)
{
	char tempImeArtikla[MAX_CHAR]="";
	int tempKolicina=0;
	int tempCijena=0;

	char buffer[MAX_SIZE] = "";
	//Da bi se izbjeglo pravljenje duplikata zbog praznog reda, koristimo pomocni buffer;
	char Pbuffer[MAX_SIZE] = "0";

	FILE* f = NULL;
	PositionR temp = head->nextR;
	PositionA tempAr = NULL;
	while (temp != NULL)
	{
		f = fopen(temp->ImeRacuna, "r");
		if (NULL == f)
		{
			perror("Datoteka ne postoji\n");
			return -1;
		}
		fgets(buffer, MAX_SIZE, f);
		sscanf(buffer, " %d %d %d", &temp->God, &temp->Mjesec, &temp->Dan);
		while (!feof(f))
		{
			fgets(buffer, MAX_SIZE, f);
			if (_strcmpi(buffer, Pbuffer) != 0)
			{
				sscanf(buffer, " %s %d %d", tempImeArtikla, &tempKolicina, &tempCijena);
				tempAr = CreateNewArtikl(tempImeArtikla, tempKolicina, tempCijena);
				UnosUListuArtikala(temp, tempAr);
			}
			strcpy(Pbuffer, buffer);
		}
		temp = temp->nextR;
		fclose(f);
	}

	return 0;
}

void UnosUListu(PositionR head, PositionR temp)
{
	while (head->nextR != NULL)
	{
		head = head->nextR;
	}

	temp->nextR = head->nextR;
	head->nextR = temp;
}

void UnosUListuArtikala(PositionR head, PositionA newArtikal)
{
	PositionA temp = NULL;
	
	if (head->nextAr == NULL)
	{
		head->nextAr = newArtikal;
	}
	else
	{
		temp = head->nextAr;
		while (temp->nextA != NULL)
		{
			temp = temp->nextA;
		}

		newArtikal->nextA = temp->nextA;
		temp->nextA = newArtikal;
	}
}

void OslobodiMemoriju(PositionR head)
{
	PositionA tempA = NULL;
	PositionR tempR = NULL;
	while (head->nextR != NULL)
	{
		tempR = head->nextR;
		tempA = tempR->nextAr;
		while (tempR->nextAr != NULL)
		{
			tempA = tempR->nextAr;
			tempR->nextAr = tempA->nextA;
			free(tempA);
		}
		head->nextR = tempR->nextR;
		free(tempR);
	}
}

void IspisiRacune(PositionR first)
{
	while (first != NULL)
	{
		printf("--------------------------------------\n");
		printf("%s %d-%d-%d\n", first->ImeRacuna, first->God, first->Mjesec, first->Dan);
		IspisiArtikle(first->nextAr);
		first = first->nextR;
	}
}
void IspisiArtikle(PositionA first)
{
	while (first != NULL)
	{
		printf("Ime:%s \t Kolicina:%d \t Cijena:%d\n", first->Ime, first->Kolicina, first->Cijena);
		first = first->nextA;
	}
	printf("--------------------------------------\n");
}

void SortirajListuPoDatumu(PositionR head)
{


}

PositionR CreateNewRacun(char* imeracuna)
{
	PositionR newRacun = NULL;

	newRacun = (PositionR)malloc(sizeof(racuni));

	if (NULL == newRacun)
	{
		perror("Racun nije uspjesno napravljen\n");
		return NULL;
	}
	
	strcpy(newRacun->ImeRacuna, imeracuna);
	newRacun->God = 0;
	newRacun->Mjesec = 0;
	newRacun->Dan = 0;
	newRacun->nextR = NULL;
	newRacun->nextAr = NULL;
	return newRacun;
}
PositionA CreateNewArtikl(char* imeArtikla, int kolicina, int cijena)
{
	PositionA newArtikal = NULL;

	newArtikal = (PositionA)malloc(sizeof(artikli));

	if (NULL == newArtikal)
	{
		perror("Artikli grozni\n");
		return NULL;
	}

	strcpy(newArtikal->Ime, imeArtikla);
	newArtikal->Kolicina = kolicina;
	newArtikal->Cijena = cijena;
	newArtikal->nextA = NULL;
	
	return newArtikal;
}