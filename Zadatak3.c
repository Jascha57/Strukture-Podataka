#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE (1024)
#define MAX_CHAR (100)

struct _Osoba;
typedef struct _Osoba* Pozicija;
typedef struct _Osoba {
    char ime[MAX_CHAR];
    char prezime[MAX_CHAR];
    int god;
    Pozicija next;
}osoba;

void UcitavanjeIzDatoteke(Pozicija head, char* imedatoteke, int* provjera);
void UnosNaZadnjeMjesto(Pozicija head, char* ime, char* prezime, int god, int* provjera);
void UnosNakon(Pozicija pozicija, Pozicija novaosoba);
void UnosNakonOdredjenogPrezimena(Pozicija first, char* trazenoprezime, char* ime, char* prezime, int god, int* provjera);
void Sortiraj(Pozicija head);
void UpisUDatoteku(Pozicija first, char* imedatoteke, int* provjera);
void IsprintajListu(Pozicija first);
void OslobodiMemoriju(Pozicija head);
void ProvjeriUneseneVarijable(char* ime, char* prezime ,int god, int* provjera);

Pozicija GdjeJeZadnji(Pozicija pozicija);
Pozicija NapraviOsobu(char* ime, char* prezime, int god);
Pozicija NadjiOsobu(char* prezime, Pozicija first);

int main(int argc, char** argv)
{
    //Varijable koje cemo ubaciti u listu
    int testgod = 0;
    char testime[MAX_CHAR] = {0};
    char testprezime[MAX_CHAR] = {0};
    char unosnakonprezime[] = "Karamatic";

    int provjera = 0;
    int i=0;

    int nime=0;
    int nprezime=0;

    char datotekazacitanje[MAX_CHAR] = "";
    char datotekazapisanje[MAX_CHAR]="";
    char txt[] = ".txt";

    osoba head = { .next = NULL,.ime = {0},.prezime = {0}, .god = 0 };

    printf("Unesite ime datoteke koju zelite ucitati u listu:");
    if (scanf(" %s", datotekazacitanje) != 1) {
        printf("Greska upisivanja ime datoteke za citanje\n");
        return 0;
    }
    else {
        strncat(datotekazacitanje, txt, 4);
    }

    UcitavanjeIzDatoteke(&head, datotekazacitanje, &provjera);
    if (provjera == 1)
    {
        printf("Greska u funkciji UcitavanjeIzDatoteke\n");
        return 0;
    }
    else if(provjera == 2)
    {
        printf("Greska u funkciji UnosNaZadnjeMjesto\n");
        return 0;
    }
    else
    {
        printf("Sve u redu za sad\n");
    }

    printf("Unesite ime:");
    scanf(" %[^\n]s", testime);
    printf("Unesite prezime:");
    scanf(" %[^\n]s", testprezime);
    printf("Unesite godiste:");
    scanf(" %d", &testgod);

    ProvjeriUneseneVarijable(testime, testprezime, testgod, &provjera);

    if(provjera!=5)
        {
            UnosNakonOdredjenogPrezimena(head.next, unosnakonprezime, testime, testprezime, testgod, &provjera);
        }
    else
        {
            printf("Unesene varijable su grozne mislim kakva su ovo imena i godine\n");
        }

    if (provjera == 3)
    {
        printf("Greska u funkciji UnosNakonOdredjenogPrezimena\n");
        return 0;
    }

    Sortiraj(&head);

    printf("Unesite ime datoteke u koju zelite upisati listu:");
    if (scanf(" %s", datotekazapisanje) != 1) {
        printf("Greska upisivanja ime datoteke za pisanje\n");
        return 0;
    }
    else {
        strncat(datotekazapisanje, txt, 4);
    }

    UpisUDatoteku(head.next, datotekazapisanje, &provjera);
    if (provjera == 4)
    {
        printf("Greska u funkciji UpisiUListu\n");
        return 0;
    }

    IsprintajListu(head.next);
    OslobodiMemoriju(&head);

    return 0;
}

void UcitavanjeIzDatoteke(Pozicija head, char* imedatoteke, int* provjera)
{
    //Temporary varijable koje koristimo da spremino i posaljemo da napravimo novi clan liste
    char tempime[MAX_CHAR] = "";
    char tempprezime[MAX_CHAR] = "";
    int tempgod = 0;
    //Ovo je rekao profesor da koristimo i znam da daje duljinu charova u stringu ali ne znam sa sto to koristimo
    int i = 0;
    int n = 0;
    int m = 0;
    //Koristi se za citanje liste
    char buffer[MAX_SIZE] = "";
    char c;

    FILE* f = NULL;

    f = fopen(imedatoteke, "r");
    if (f == NULL) {
        *provjera = 1;
    }
    else {
        c = fgetc(f);
        if (c == EOF)
        {
            printf("Datoteka je prazna\n");
            *provjera = 1;
        }
        else
        {
            ungetc(c, f);
        }
        while (!feof(f) && *provjera==0)
        {
            fgets(buffer, MAX_SIZE, f);
            if (sscanf(buffer, "%s %s %n %d", tempime, tempprezime, &n, &tempgod) !=EOF && tempgod != 0)
            {
                for (i = 0; i < n; i++)
                {
                    if (isspace(buffer[i])==0 && isalpha(buffer[i]) != 0 )
                    {
                        m++;
                    }
                }
                if (m == n - 2)
                {
                    //Ovdje treba osigurat napraviti novu osobu. Ovdje samo oni koji ispunjavaju uvjete
                    UnosNaZadnjeMjesto(head, tempime, tempprezime, tempgod, provjera);
                }
                m = 0;
                tempgod = 0;
            }
        }
        fclose(f);
    }
}

void UnosNaZadnjeMjesto(Pozicija head, char* ime, char* prezime, int god, int* provjera)
{
    Pozicija novaosoba = NULL;
    Pozicija zadnji = NULL;

    novaosoba = NapraviOsobu(ime, prezime, god);

    if (novaosoba == NULL)
    {
        *provjera = 2;
    }
    else
    {
        zadnji = GdjeJeZadnji(head);
        UnosNakon(zadnji, novaosoba);
    }
}


void UnosNakon(Pozicija pozicija, Pozicija novaosoba)
{
    novaosoba->next = pozicija->next;
    pozicija->next = novaosoba;
}

void UnosNakonOdredjenogPrezimena(Pozicija first, char* trazenoprezime, char* ime, char* prezime, int god, int* provjera)
{
    Pozicija novaosoba = NULL;
    Pozicija temp = NULL;

    novaosoba = NapraviOsobu(ime, prezime, god);
    temp = NadjiOsobu(trazenoprezime, first);

    if (NULL == temp || NULL == novaosoba) {
        *provjera = 3;
    }
    else
    {
        UnosNakon(temp, novaosoba);
    }
}

void Sortiraj(Pozicija head)
{
    Pozicija p = NULL;
    Pozicija q = NULL;
    Pozicija temp = NULL;
    Pozicija end = NULL;

    // Ne znam zasto ali ako pise malo i ide na kraj u prezimenu. Ako je veliko I radi sve uredu.
    //strcmp(p->prezime, p->next->prezime)>0
    // p->god>p->next->god

    while (head->next != end) {
        q = head;
        p = head->next;
        while (p->next != end) {
            if (strcmp(p->prezime, p->next->prezime) > 0) {
                temp = p->next;
                q->next = temp;
                p->next = temp->next;
                temp->next = p;
                p = temp;
            }
            q = p;
            p = p->next;
        }
        end = p;
    }
}

void UpisUDatoteku(Pozicija first, char* imedatoteke, int* provjera)
{
    Pozicija temp = first;
    FILE* f = NULL;
    f = fopen(imedatoteke, "w");

    if (NULL == f) {
        *provjera=4;
    }
    else
    {
        while (temp->next != NULL) {
            fprintf(f, "%s %s %d\n", temp->ime, temp->prezime, temp->god);
            temp = temp->next;
        }
        fprintf(f, "%s %s %d\n", temp->ime, temp->prezime, temp->god);
        fclose(f);
    }
}

void IsprintajListu(Pozicija first)
{
    Pozicija temp = first;

    while (temp) {
        printf("Ime: %s \t  Prezime: %s \t  GodinaRodjenja: %d\n", temp->ime, temp->prezime, temp->god);
        temp = temp->next;
    }
}

void OslobodiMemoriju(Pozicija head)
{
    Pozicija temp = head->next;
    if (NULL == temp->next) {
        free(temp);
    }
    else{
        while(head!= NULL) {
            temp = head;
            head = head->next;
            free(temp);
        }
    }
}

void ProvjeriUneseneVarijable(char* ime, char* prezime ,int god, int* provjera)
{
    int i=0;
    int n=0;
    char buffer[MAX_SIZE]="";

    strncat(buffer, ime, strlen(ime));
    strncat(buffer, prezime, strlen(prezime));
    //Ovaj n ne treba ali cu ga ostaviti
    n=strlen(buffer);

    for(i=0;i<n;i++)
    {
        if (isalpha(buffer[i]) == 0 )
        {
            *provjera=5;
        }
    }
    if(god<0)
    {
        *provjera=5;
    }
}

Pozicija GdjeJeZadnji(Pozicija pozicija)
{
    Pozicija zadnji = pozicija;

    while (zadnji->next) {
        zadnji = zadnji->next;
    }

    return zadnji;
}

Pozicija NapraviOsobu(char* ime, char* prezime, int god)
{
    Pozicija novaosoba = NULL;

    novaosoba = (Pozicija)malloc(sizeof(osoba));
    if (!novaosoba) {
        perror("Alokacija memorije nije uspijela!\n");
        return NULL;
    }
    else
    {
        strcpy(novaosoba->ime, ime);
        strcpy(novaosoba->prezime, prezime);
        novaosoba->god = god;
        novaosoba->next = NULL;
    }
    return novaosoba;
}

Pozicija NadjiOsobu(char* prezime, Pozicija first)
{
    Pozicija temp = first;

    while (temp) {
        if (strcmp(temp->prezime, prezime) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return temp;
}
