#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CHAR (100)
#define MAX_SIZE (1024)

struct _Racun;
struct _Artikl;
typedef struct _Racun* PositionR;
typedef struct _Artikl* PositionA;

typedef struct _Artikl
{
	char Ime[MAX_CHAR];
	int Kolicina;
	int Cijena;
	PositionA nextA;
}artikl;

/// <summary>
/// Yo dawg I heard you like structures so I put a structure inside your structure.
/// Xzibit 2021
/// https://knowyourmeme.com/memes/xzibit-yo-dawg
/// </summary>

typedef struct _Racun
{
	char ImeRacuna[MAX_CHAR];
	int God;
	int Mjesec;
	int Dan;
	PositionR nextR;
	artikl headAR;
}racun;

int CitanjeRacunaIzDatoteke(PositionR head, char* imedatoteke);
int CitanjeArtikalaIzDatoteke(PositionR head);
int Info(PositionR head);

void UnosUListu(PositionR head, PositionR temp);
void UnosUListuArtikala(PositionA head, PositionA newArtikal);
void IspisiRacune(PositionR first);
void IspisiArtikle(PositionA first);
void OslobodiMemoriju(PositionR head);

PositionR CreateNewRacun(char* imeracuna);
PositionA CreateNewArtikl(char* imeArtikla, int kolicina, int cijena);

int main()
{
	char imedatoteke[] = "racuni.txt";
	racun head = { .ImeRacuna = "", .God = 0, .Mjesec = 0, .Dan = 0, .nextR = NULL, .headAR.Ime = "", .headAR.Kolicina = 0, .headAR.Cijena = 0 };

	CitanjeRacunaIzDatoteke(&head, imedatoteke);
	CitanjeArtikalaIzDatoteke(&head);

	IspisiRacune(head.nextR);

	Info(&head);

	OslobodiMemoriju(&head);

	return 0;
}

/// <summary>
/// Cita racune, datume i pravi listu
/// </summary>
/// <param name="head"></param>
/// <param name="imedatoteke"></param>
/// <returns></returns>
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
/// <summary>
/// Cita artikle iz datoteke. Ime datoteke je sadrzano u listi zbog lakseg koda, barem za mene.
/// </summary>
/// <param name="head"></param>
/// <returns></returns>
int CitanjeArtikalaIzDatoteke(PositionR head)
{
	char tempImeArtikla[MAX_CHAR] = "";
	int tempKolicina = 0;
	int tempCijena = 0;

	char buffer[MAX_SIZE] = "";
	//Da bi se izbjeglo pravljenje duplikata zbog praznog reda, koristimo pomocni buffer;
	//Isto ne znam zasto javlja da je Pbuffer uninitialized c6001. Iako ne javlja za bufffer.
	char Pbuffer[MAX_SIZE] = "0";

	FILE* f = NULL;
	PositionR tempR = head->nextR;
	PositionA tempAr = NULL;

	while (tempR != NULL)
	{
		f = fopen(tempR->ImeRacuna, "r");
		if (NULL == f)
		{
			perror("Datoteka ne postoji\n");
			return -1;
		}
		fgets(buffer, MAX_SIZE, f);
		sscanf(buffer, " %d %d %d", &tempR->God, &tempR->Mjesec, &tempR->Dan);
		while (!feof(f))
		{
			fgets(buffer, MAX_SIZE, f);
			if (_strcmpi(buffer, Pbuffer) != 0)
			{
				sscanf(buffer, " %s %d %d", tempImeArtikla, &tempKolicina, &tempCijena);
				tempAr = CreateNewArtikl(tempImeArtikla, tempKolicina, tempCijena);
				UnosUListuArtikala(&tempR->headAR, tempAr);
			}
			strcpy(Pbuffer, buffer);
		}
		tempR = tempR->nextR;
		fclose(f);
	}
	return 0;
}
/// <summary>
/// Daje informacije koje trazi korisnik, treba nadograditi kod da ne puca ako je korisnik zao ali previse špageta bude.
/// </summary>
/// <param name="head"></param>
/// <returns></returns>
int Info(PositionR head)
{
	//Razdoblje koje korisnik odredjuje
	int DonjaGod = 0;
	int GornjaGod = 0;
	int suma = 0;
	//Brojac za provjeru dali je ijedan artikal ispisan
	int br = 0;
	//Naziv artikla koje zanima korisnika
	char ime[MAX_CHAR] = "";
	//Pokazivaci pomocu kojih trazimo info
	PositionR tempR = head->nextR;
	PositionA tempA = NULL;

	printf("Upisite od koje godine vas zanima:");
	scanf("%d", &DonjaGod);
	printf("Upisite do koje godine vas zanima:");
	scanf("%d", &GornjaGod);
	printf("Koji artikl vas zanima:");
	scanf(" %s", ime);
	printf("\n");
	while (tempR != NULL)
	{
		if (tempR->God >= DonjaGod && tempR->God <= GornjaGod)
		{
			tempA = tempR->headAR.nextA;
			while (tempA != NULL)
			{
				if (_strcmpi(tempA->Ime, ime) == 0)
				{
					printf("Na trazeni artikal %s je potroseno %d. Izvor:%s\n", ime, (tempA->Cijena) * (tempA->Kolicina), tempR->ImeRacuna);
					suma += (tempA->Cijena) * (tempA->Kolicina);
					br++;
				}
				tempA = tempA->nextA;
			}
		}
		tempR = tempR->nextR;
	}
	printf("Sveukupna zarada:%d", suma);
	if (br == 0)
	{
		printf("Trazeni artikal ili ne postoji ili nije prodan u razdoblju od %d do %d\n", DonjaGod, GornjaGod);
	}
	return 0;
}

/// <summary>
/// Obicni unos u listu. Treba napraviti sortirani.
/// </summary>
/// <param name="head"></param>
/// <param name="temp"></param>
void UnosUListu(PositionR head, PositionR temp)
{
	while (head->nextR != NULL)
	{
		head = head->nextR;
	}

	temp->nextR = head->nextR;
	head->nextR = temp;
}
/// <summary>
/// Ovo je sortirani unos artikala iako naslov funkcije ne kaze to.
/// </summary>
/// <param name="head"></param>
/// <param name="newArtikal"></param>
void UnosUListuArtikala(PositionA head, PositionA newArtikal)
{
	PositionA temp = NULL;

	if (head->nextA == NULL)
	{
		head->nextA = newArtikal;
	}
	else
	{
		temp = head;
		while (temp->nextA != NULL && _strcmpi(temp->nextA->Ime, newArtikal->Ime) < 0)
		{
			temp = temp->nextA;
		}

		newArtikal->nextA = temp->nextA;
		temp->nextA = newArtikal;
	}
}

void IspisiRacune(PositionR first)
{
	while (first != NULL)
	{
		printf("--------------------------------------\n");
		printf("%s %d-%d-%d\n", first->ImeRacuna, first->God, first->Mjesec, first->Dan);
		IspisiArtikle(first->headAR.nextA);
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

void OslobodiMemoriju(PositionR head)
{
	PositionA tempA = NULL;
	PositionR tempR = NULL;
	while (head->nextR != NULL)
	{
		tempR = head->nextR;
		while (tempR->headAR.nextA != NULL)
		{
			tempA = tempR->headAR.nextA;
			tempR->headAR.nextA = tempA->nextA;
			free(tempA);
		}
		head->nextR = tempR->nextR;
		free(tempR);
	}
}

/// <summary>
/// Stvaranje novog elementa. Primjenjene za novi head u strukturi racun
/// </summary>
/// <param name="imeracuna"></param>
/// <returns></returns>

PositionR CreateNewRacun(char* imeracuna)
{
	PositionR newRacun = NULL;

	newRacun = (PositionR)malloc(sizeof(racun));
	if (newRacun == NULL)
	{
		perror("Greska u create new racun\n");
		return NULL;
	}

	strcpy(newRacun->ImeRacuna, imeracuna);
	strcpy(newRacun->headAR.Ime, "");
	newRacun->God = 0;
	newRacun->Mjesec = 0;
	newRacun->Dan = 0;
	newRacun->nextR = NULL;
	newRacun->headAR.Kolicina = 0;
	newRacun->headAR.Cijena = 0;
	newRacun->headAR.nextA = NULL;

	return newRacun;
}

PositionA CreateNewArtikl(char* imeArtikla, int kolicina, int cijena)
{
	PositionA newArtikl = NULL;

	newArtikl = (PositionA)malloc(sizeof(artikl));

	if (NULL == newArtikl)
	{
		perror("Greska u cna\n");
		return 0;
	}

	strcpy(newArtikl->Ime, imeArtikla);
	newArtikl->Kolicina = kolicina;
	newArtikl->Cijena = cijena;
	newArtikl->nextA = NULL;
	return newArtikl;

}