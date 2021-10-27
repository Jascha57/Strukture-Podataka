//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR (50)

struct _Osoba;
typedef struct _Osoba* Pozicija;
typedef struct _Osoba {
    char ime[MAX_CHAR];
    char prezime[MAX_CHAR];
    int god;
    Pozicija next;
}osoba;

int UnosNaPrvoMjesto(Pozicija head, char* ime, char* prezime, int god);
int UnosNaZadnjeMjesto(Pozicija head, char* ime, char* prezime, int god);
int IsprintajListu(Pozicija first);
int UnosNakon(Pozicija pozicija, Pozicija novaosoba);
int IzbrisiOdredjeniElement(char* prezime, Pozicija pozicija);
int UnosNakonOdredjenogElementa(Pozicija first, char* prezime1, char* ime, char* prezime, int god);
int UnosPrijeOdredjenogElementa(Pozicija first, char* prezime1, char* ime, char* prezime, int god);
int UcitajListuIzDatoteke(Pozicija head, char* imedatoteke);
int UnesiListuUDatoteku(Pozicija first, char* imedatoteke);

void OslobodiMemoriju(Pozicija head);
void SortirajPoPrezimenu(Pozicija head);

Pozicija NapraviOsobu(char* ime, char* prezime, int god);
Pozicija GdjeJeZadnji(Pozicija pozicija);
Pozicija NadjiOdredjenuOsobu(char* prezime, Pozicija first);
Pozicija PronadjiPrethodni(char* prezime, Pozicija pozicija);

int main(int argc, char** argv) {
    //Sve ove test varijable su samo za testiranje. Vazno je samo provjeriti da funkcije rade
    //Treba isto dodati provjeru funkcija. Ako nisu uradile da stopiraju program odmah.
    int testgod=1555;
    char testime[]="Abraham";
    char testprezime[]="Van Hellsing";

    char nazivdatoteke[] = "osobe.txt";
    char nazivdatotekezapisanje[]="osobe2.txt";
    char izbrisati[]="Janjic";

    osoba head = { .next = NULL,.ime = {0},.prezime = {0}, .god = 0 };
    Pozicija p = &head;

    UcitajListuIzDatoteke(p, nazivdatoteke);

    //IzbrisiOdredjeniElement(izbrisati, p);

    UnosNakonOdredjenogElementa(head.next, izbrisati, testime, testprezime, testgod);
    //UnosPrijeOdredjenogElementa(head.next, izbrisati, testime, testprezime, testgod);
    SortirajPoPrezimenu(p);
    UnesiListuUDatoteku(head.next, nazivdatotekezapisanje);

    IsprintajListu(head.next);
    OslobodiMemoriju(p);
    return 0;
}
//"Napravimo" novu osobu i posaljemo head i novu osobu u unosnakon tako da se dodaje na prvo mjesto liste.
int UnosNaPrvoMjesto(Pozicija head, char* ime, char* prezime, int god)
{
    Pozicija novaosoba = NULL;

    novaosoba = NapraviOsobu(ime, prezime, god);

    if (!novaosoba) {
        return -1;
    }

    UnosNakon(head, novaosoba);

    return EXIT_SUCCESS;
}
//Unosi na zadnje mjesto. Prvo "napravimo" osobu i nakon toga, pronadjemo zadnjeg i dodamo nakon njega.
int UnosNaZadnjeMjesto(Pozicija head, char* ime, char* prezime, int god)
{
    Pozicija novaosoba = NULL;
    Pozicija last = NULL;

    novaosoba = NapraviOsobu(ime, prezime, god);
    if (NULL == novaosoba) {
        return -1;
    }

    last = GdjeJeZadnji(head);
    UnosNakon(last, novaosoba);

    return EXIT_SUCCESS;
}
//Prolaci kro cijelu listu i printa vrijednosti. Koristimo temporary kao alat da prolazimo kroz listu.
int IsprintajListu(Pozicija first)
{
    Pozicija temp = first;

    while (temp) {
        printf("Ime: %s \t  Prezime: %s \t  GodinaRodjenja: %d\n", temp->ime, temp->prezime, temp->god);
        temp = temp->next;
    }

    return EXIT_SUCCESS;
}
//Osnovna funkcija za unos nakon nekog elementa. Radi za početak, kraj i izmedju. Jedina je razlika kako je pozovemo u drugim funkcijama.
int UnosNakon(Pozicija pozicija, Pozicija novaosoba)
{
    novaosoba->next = pozicija->next; //Povezijemo novu osobu sa elementom koji je prije bio na tom mjestu kako ga ne bi izgubili
    pozicija->next = novaosoba;   //Ovdje tek povezujemo prijasnji i novu osobu

    return EXIT_SUCCESS;
}

int IzbrisiOdredjeniElement(char* prezime, Pozicija pozicija)
{
    Pozicija temp = NULL;
    pozicija = PronadjiPrethodni(prezime, pozicija);
    if (NULL == pozicija) {
        printf("Greska u IzbrisiOdredjeniElement!\n");
        return 0;
    }
    else {
        temp = pozicija->next;
        pozicija->next = pozicija->next->next;
        free(temp);
    }
    return EXIT_SUCCESS;
}
int UnosNakonOdredjenogElementa(Pozicija first, char* prezime1, char* ime, char* prezime, int god) {
    Pozicija novaosoba = NULL;
    Pozicija temp = NULL;

    novaosoba = NapraviOsobu(ime, prezime, god);
    temp = NadjiOdredjenuOsobu(prezime1, first);

    if (NULL == temp || NULL == novaosoba) {
        printf("Greska u UnosNakonOdredjenogElementa!\n");
        return 0;
    }

    UnosNakon(temp, novaosoba);

    return EXIT_SUCCESS;
}

int UnosPrijeOdredjenogElementa(Pozicija first, char* prezime1, char* ime, char* prezime, int god) {
    Pozicija temp = NULL;
    Pozicija novaosoba = NULL;

    temp = PronadjiPrethodni(prezime1, first);
    novaosoba = NapraviOsobu(ime, prezime, god);

    if (NULL == temp || NULL == novaosoba) {
        printf("Greska u UnosPrijeOdredjenogElementa!\n");
        return 0;
    }

    UnosNakon(temp, novaosoba);

    return EXIT_SUCCESS;
}

int UcitajListuIzDatoteke(Pozicija head, char* imedatoteke) {
    char tempime[MAX_CHAR] = { 0 };
    char tempprezime[MAX_CHAR] = { 0 };
    char c;
    char buffer[MAX_CHAR] = { 0 };
    int i = 0;
    int tempgod = 0;
    int brojredova = 0;
    FILE* f = NULL;

    f = fopen(imedatoteke,"r");

    if (NULL == f) {
        printf("Nije pronadjena datoteka\n");
        free(head);
        return 0;
    }

    c = fgetc(f);
    if (c == EOF) {
        return 0;
    }
    else {
        ungetc(c, f);
    }

    while (!feof(f)) {
        fgets(buffer, MAX_CHAR, f);
        brojredova++;
    }

    rewind(f);

    for (i = 0; i < brojredova; i++) {
        fscanf(f, " %s %s %d", tempime, tempprezime, &tempgod);
        UnosNaZadnjeMjesto(head, tempime, tempprezime, tempgod);
    }

    fclose(f);

    return EXIT_SUCCESS;
}

int UnesiListuUDatoteku(Pozicija first, char* imedatoteke){
    Pozicija temp=first;
    FILE* f=NULL;
    f=fopen(imedatoteke, "w");

    if(NULL==f){
        printf("Greska otvaranja datoteke za writing");
        return 0;
    }
    //Ovo moram pitati jer moram dodati jos jedan fprintf poslije pa kako ljepse upisat u datoteku
    while(temp->next!=NULL){
        fprintf(f,"%s %s %d\n", temp->ime, temp->prezime, temp->god);
        temp=temp->next;
    }
    fprintf(f,"%s %s %d\n", temp->ime, temp->prezime, temp->god);

    fclose(f);
    return 0;
}

void OslobodiMemoriju(Pozicija head) {
    Pozicija temp=head->next;
    if(NULL==temp->next){
        free(temp);
    }
    else if (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void SortirajPoPrezimenu(Pozicija head){
    Pozicija p=NULL;
    Pozicija q=NULL;
    Pozicija temp=NULL;
    Pozicija end=NULL;
    // Ne znam zasto ali ako pise malo i ide na kraj u prezimenu. Ako je veliko I radi sve uredu.
    //strcmp(p->prezime, p->next->prezime)>0
    // p->god>p->next->god
    while(head->next!=end){
        q=head;
        p=head->next;
        while(p->next!=end){
            if(strcmp(p->prezime, p->next->prezime)>0){
                temp = p->next;
                q->next=temp;
                p->next=temp->next;
                temp->next=p;
                p=temp;
            }
            q=p;
            p=p->next;
        }
        end=p;
    }
}

Pozicija NapraviOsobu(char* ime, char* prezime, int god) {
    Pozicija novaosoba = NULL;

    novaosoba = (Pozicija)malloc(sizeof(osoba));
    if (!novaosoba) {
        perror("Alokacija memorije nije uspijela!\n");
        return NULL;
    }

    strcpy(novaosoba->ime, ime);
    strcpy(novaosoba->prezime, prezime);
    novaosoba->god = god;
    novaosoba->next = NULL;

    return novaosoba;
}

Pozicija GdjeJeZadnji(Pozicija pozicija) {
    Pozicija last = pozicija;

    while (last->next) {
        last = last->next;
    }

    return last;
}

Pozicija NadjiOdredjenuOsobu(char* prezime, Pozicija first) {
    Pozicija temp = first;

    while (temp) {
        if (strcmp(temp->prezime, prezime) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return temp;
}
//-> Operator se moze ulancati tj. s njim se moze dodji do elementa bez da mjenjamo pozicija stalno
Pozicija PronadjiPrethodni(char* prezime, Pozicija pozicija) {
    while (pozicija->next != NULL && strcmp(pozicija->next->prezime, prezime)!=0) {
        pozicija = pozicija->next;
    }
    if (NULL == pozicija) {
        return NULL;
    }
    else {
        return pozicija;
    }
}

//Problemi zbog kojih sam izgubio živce:
//1.) Nakon brojenja redova nisam koristio rewind funkciju
//2.) U funkciji ispis, stavio sam temp=NULL kad je trebalo temp=first
//TO BE CONTINUED
//3.) Imao sam problem sa pronadjiprethodni. Nisam koristio strcmp sto bi crashalo program. Jos je spor ali mislim da radi za sad.
