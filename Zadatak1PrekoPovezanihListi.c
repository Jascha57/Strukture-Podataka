#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR (100)

struct _Osoba;
typedef struct _Osoba* Position;
typedef struct _Osoba{
    char ime[MAX_CHAR];
    char prezime[MAX_CHAR];
    int god;
    Position next;
}Osoba;

int PrependList(Position head, char* ime, char* prezime, int god);
int IsprintajListu(Position first);
int UnosNakon(Position position, Position newperson);

Position NapraviOsobu(char* ime, char* prezime, int god);

int main(int argc, char** argv)
{
    int i=0;
    int brojredaka=1;
    char c;

    char imeosobe[MAX_CHAR];
    char prezimeosobe[MAX_CHAR];
    int godosobe=0;

    FILE* f=NULL;
    Osoba head={.next=NULL,.ime={0},.prezime={0}, .god=0};

    Position p = &head;

    f=fopen("osobe.txt","r");
    if(NULL==f)
        {
            perror("Datoteka ne postoji\n");
            free(p);
            return 0;
        }

    //Problem sa ovim brojacem je da treba staviti original int var na 1 jer je rezultat n-1
    while(!feof(f)){
        c=fgetc(f);
        if(c=='\n'){
            brojredaka++;
        }
    }
    rewind(f);

    for(i=0;i<brojredaka;i++){
        fscanf(f," %s %s %d", imeosobe, prezimeosobe, &godosobe);
        PrependList(&head, imeosobe, prezimeosobe, godosobe);
    }

    IsprintajListu(head.next);

    fclose(f);
    free(p);
    return 0;
}


int PrependList(Position head, char* ime, char* prezime, int god){
    Position newperson=NULL;

    newperson=NapraviOsobu(ime, prezime, god);

    if(!newperson){
        return -1;
    }

    UnosNakon(head, newperson);

    return EXIT_SUCCESS;
}


int IsprintajListu(Position first){
    Position temporary=first;

    while(temporary){
        printf("Ime: %s \t  Prezime: %s \t  GodinaRodjenja: %d\n", temporary->ime, temporary->prezime, temporary->god);
        temporary=temporary->next;
    }

    return EXIT_SUCCESS;
}


int UnosNakon(Position position, Position newperson){

    newperson->next=position->next;
    position->next=newperson;

    return EXIT_SUCCESS;
}


Position NapraviOsobu(char* ime, char* prezime, int god){
    Position newperson=NULL;

    newperson=(Position)malloc(sizeof(Osoba));
    if(!newperson){
        perror("Alokacija memorije nije uspijela!\n");
        return NULL;
    }

    strcpy(newperson->ime,ime);
    strcpy(newperson->prezime,prezime);
    newperson->god=god;
    newperson->next=NULL;

    return newperson;

}
