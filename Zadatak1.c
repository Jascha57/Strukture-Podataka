#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (128)
#define MAX_LINE (1024)

typedef struct _Student{
    char ime[MAX_SIZE];
    char prezime[MAX_SIZE];
    double bodovi;
}student;

int ProcitajBrojRedakaIzDatoteke(char* nazivDatoteke);
student* AlocirajMemorijuIProcitajStudente(char* nazivDatoteke, int BrojStudenata);

int main(){
    int brojRedaka=0;
    int brojac = 0;
    char imeDatoteke[]="studenti.txt";
    student* x=NULL;

    brojRedaka=ProcitajBrojRedakaIzDatoteke(imeDatoteke);

    if(brojRedaka<=0){
            printf("Datoteka je prazna!");
            return 0;
    }

    x=AlocirajMemorijuIProcitajStudente(imeDatoteke, brojRedaka);

    if(x==NULL){
        printf("Greska alociranja memorije u main!");
        return EXIT_FAILURE;
    }

    for(brojac=0;brojac<brojRedaka;brojac++){
        printf("Ime:%s  \t  Prezime:%s   \t   Abs:%lf   \t   Rel:%lf%%\n", x[brojac].ime, x[brojac].prezime, x[brojac].bodovi, (x[brojac].bodovi)/2);
    }

    free(x);

    return 0;
}

int ProcitajBrojRedakaIzDatoteke(char* nazivDatoteke){
    int brojac = 0;
    char DaliJeDatotekaPrazana;
    FILE* datoteka=NULL;
    char buffer[MAX_LINE]={0};
    //Standard otvaranje datoteke
    datoteka=fopen(nazivDatoteke,"r");

    if(!datoteka){
        printf("Greska otvaranja datoteke!");
        return EXIT_FAILURE;
    }
    //Provjerimo dali je prazna
    DaliJeDatotekaPrazana=fgetc(datoteka);
    if(DaliJeDatotekaPrazana==EOF){
        return 0;
    }
    else{
        ungetc(DaliJeDatotekaPrazana, datoteka);
    }
    //Broji ako nije prazna
    while(!feof(datoteka)){
        fgets(buffer,MAX_LINE,datoteka);
        brojac++;
    }
    //Zatvaranje datoteke
    fclose(datoteka);
    return brojac;
}

student* AlocirajMemorijuIProcitajStudente(char* nazivDatoteke, int BrojStudenata){
    int brojac = 0;
    FILE* datoteka=NULL;
    student* studenti=NULL;

    studenti=(student*)malloc(BrojStudenata*sizeof(student));
    datoteka=fopen(nazivDatoteke,"r");
    if(!datoteka){
        printf("Greska otvaranja datoteke!");
        free(studenti);
        return NULL;
    }

    if(studenti==NULL){
        printf("Greska alociranja memorije!");
        free(studenti);
        fclose(datoteka);
        return NULL;
    }

    while(!feof(datoteka)){
        fscanf(datoteka," %s %s %lf", studenti[brojac].ime, studenti[brojac].prezime, &studenti[brojac].bodovi);
        brojac++;
    }

    fclose(datoteka);
    return studenti;
}
