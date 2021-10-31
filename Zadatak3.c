#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_SIZE (1024)
#define MAX_CHAR (100)

//Mala slova su inicijali za imena funkcije
#define GRESKAZAuiz (1)
#define GRESKAZAunzm (2)
#define GRESKAZApuv (3)
#define GRESKAZAunop (4)
#define GRESKAZAuud (5)

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
//Pretpostavljam da zbog sigurnosti, isto tako treba na vise mjesta osloboditi funkciju ali zasad ovaj komentar ostaje
void OslobodiMemoriju(Pozicija head);
void ProvjeriUneseneVarijable(char* ime, char* prezime, int god, int* provjera);

bool DaliJeStringIspravan(char* imeiprezime, int duljinastringa);

Pozicija GdjeJeZadnji(Pozicija pozicija);
Pozicija NapraviOsobu(char* ime, char* prezime, int god);
Pozicija NadjiOsobu(char* prezime, Pozicija first);

int main(int argc, char** argv)
{
    //Varijable koje cemo ubaciti u listu
    int testgod = 0;
    char testime[MAX_CHAR] = { 0 };
    char testprezime[MAX_CHAR] = { 0 };
    char unosnakonprezime[] = "Karamatic";

    int provjera = 0;
    int jelikorisnikupisaobrojeve = 0;

    char datotekazacitanje[MAX_CHAR] = "";
    char datotekazapisanje[MAX_CHAR] = "";
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
    if (provjera == GRESKAZAuiz)
    {
        printf("Greska u funkciji UcitavanjeIzDatoteke\n");
        return 0;
    }
    else if (provjera == GRESKAZAunzm)
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

    jelikorisnikupisaobrojeve = scanf("%d", &testgod);
    //Osiguranje da korisnik ne uspije upisivat tip stringa za godine.
    //Imamo 2 scenarija
    //1.Ako korisnik pocme sa slovom isprintat ce "mislio si da si pametan i resetirati"
    //buffer pa da naciv datoteke za upisivanje ne bude ostatak smeca sto je upisano
    //2.Ako korisnik pocme sa brojevima to ce se upisati kao god rodjenja i nastavljamo sa  programom
    if (jelikorisnikupisaobrojeve == 0)
    {
        printf("Mislio si da si pametan\n");
        fseek(stdin, 0, SEEK_END);
    }
    else
    {
        //Ova funkcija se moze zamijeniti sa boolovom funkcijom koju sam napravio
        ProvjeriUneseneVarijable(testime, testprezime, testgod, &provjera);
        if(provjera==GRESKAZApuv)
        {
            printf("Unesene varijable su glupe\n");
        }
        else
        {
            UnosNakonOdredjenogPrezimena(head.next, unosnakonprezime, testime, testprezime, testgod, &provjera);
        }
    }

    //Resetiranje buffera tako da korisnik moze upisati ime datoteke za upisivanje
    fseek(stdin, 0, SEEK_END);
    if (provjera == GRESKAZAunop)
    {
        printf("Greska u funkciji UnosNakonOdredjenogPrezimena\n");
        return 0;
    }
    //Sve ovo dolje radi kako treba koliko sam provjerio.
    Sortiraj(&head);

    printf("Unesite ime datoteke u koju zelite upisati listu:");
    scanf(" %s", datotekazapisanje);
    strncat(datotekazapisanje, txt, strlen(txt));

    UpisUDatoteku(head.next, datotekazapisanje, &provjera);
    if (provjera == GRESKAZAuud)
    {
        printf("Greska u funkciji UpisiUDatoteku\n");
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
    int n = 0;
    //Koristi se za citanje liste
    char buffer[MAX_SIZE] = "";
    char c;

    FILE* f = NULL;

    f = fopen(imedatoteke, "r");
    if (f == NULL) {
        *provjera = GRESKAZAuiz;
    }
    else {
        c = fgetc(f);
        if (c == EOF)
        {
            printf("Datoteka je prazna\n");
            *provjera = GRESKAZAuiz;
        }
        else
        {
            ungetc(c, f);
        }
        while (!feof(f) && *provjera == 0)
        {
            fgets(buffer, MAX_SIZE, f);
            if (sscanf(buffer, "%s%s%n %d", tempime, tempprezime, &n, &tempgod) != EOF && tempgod != 0) {
                if (DaliJeStringIspravan(buffer, n - 1) == 1)
                {
                    UnosNaZadnjeMjesto(head, tempime, tempprezime, tempgod, provjera);
                }
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
        *provjera = GRESKAZAunzm;
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
        *provjera = GRESKAZAunop;
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
        *provjera = GRESKAZAuud;
    }
    else
    {
        while (temp != NULL) {
            fprintf(f, "%s %s %d\n", temp->ime, temp->prezime, temp->god);
            temp = temp->next;
        }
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
    else {
        while (head->next!=NULL) {
            temp = head->next;
            head->next = temp->next;
            free(temp);
        }
    }
}

void ProvjeriUneseneVarijable(char* ime, char* prezime, int god, int* provjera)
{
    int i = 0;
    int n = 0;
    char buffer[MAX_SIZE] = "";

    strncat(buffer, ime, strlen(ime));
    strncat(buffer, prezime, strlen(prezime));
    //Ovaj n ne treba ali cu ga ostaviti
    n = strlen(buffer);

    for (i = 0; i < n; i++)
    {
        if (isalpha(buffer[i]) == 0)
        {
            *provjera = GRESKAZApuv;
        }
    }
    if (god < 0)
    {
        *provjera = GRESKAZApuv;
    }
}

bool DaliJeStringIspravan(char* imeiprezime, int duljinastringa)
{
    int brojslovaustringu = 0;
    int i = 0;

    for (i = 0; i <= duljinastringa; i++)
    {
        if (isalpha(imeiprezime[i]) != 0)
        {
            brojslovaustringu++;
        }
    }

    return duljinastringa == brojslovaustringu;
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
