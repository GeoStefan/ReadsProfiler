
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>
#include "ValidareComanda.h"

#define COM_N " Comanda necunoscuta."
#define DELIMIT_CAUTA "$"

char tipComanda(char comanda[1024])
{
    char tip= 'n';
    if(strcmp(comanda,"login") == 0)
        return tip = 'l';
    if(strcmp(comanda,"cauta") == 0)
        return tip = 'c';
    if(strcmp(comanda,"descarca") == 0)
        return tip = 'd';
    if(strcmp(comanda,"review") == 0)
        return tip = 'r';
    if(strcmp(comanda,"recomanda") == 0)
        return tip = 'p';
    if(strcmp(comanda,"adauga_carte") == 0)
        return tip = 'a';
    if(strcmp(comanda,"sterge_carte") == 0)
        return tip = 's';
    if(strcmp(comanda,"adauga_user") == 0)
        return tip = 'u';
    if(strcmp(comanda,"istoric") == 0)
        return tip = 'h';
    if(strcmp(comanda,"logout") == 0)
        return tip = 'x';
    if(strcmp(comanda,"exit") == 0)
        return tip = 't';
    return tip = 'n';
}

bool validareTip(char comanda[1024])
{
    char *t = strtok(comanda," ");
    if(t == NULL)
        return false;
    char c[1024];
    strcpy(c,t);
    t = strtok(NULL," ");
    if(t != NULL)
        return false;
    strcpy(comanda,c);
    return true;
}

bool trimSpatiu(char comanda[1024])
{
    int i =0;
    char enter[3];
    enter[0] ='\n';
    enter[1] = '\0';
    if(strcmp(comanda,enter) == 0)
    {
        strcpy(comanda,"ALL");
        return true;
    }
    while(strlen(comanda)>0 && i < strlen(comanda) && comanda[i] == ' ')
    {
        i++;
    }
    strcpy(comanda,(comanda)+i);
    if(strcmp(comanda,"") == 0)
        return false;
    if(comanda[strlen(comanda)-1] == '\n')
        comanda[strlen(comanda)-1] = '\0';
    i = strlen(comanda) - 1;
    while(i>0 && comanda[i] == ' ')
        i--;
    comanda[i+1] = '\0';
    if(strcmp(comanda,"") == 0)
        return false;
    return true;
}

bool isNumber(char comanda[1024])
{
    int i;
    for(i = 0; i < strlen(comanda); ++i)
        if(!isdigit(comanda[i]))
            return false;
    return true;
}

bool desparteVirgula(char comanda[1024])
{
    int i = 0;
    char enter[3], rez[100], *a, aux[100];
    enter[0] ='\n';
    enter[1] = '\0';
    rez[0] = '\0';
    if(strcmp(comanda,enter) == 0)
    {
        strcpy(comanda,"ALL");
        return true;
    }
    comanda[strlen(comanda)-1] = '\0';
    a = strtok(comanda,",");
    while(a != NULL)
    {
        strcpy(aux,a);
        trimSpatiu(aux);
        if(strlen(aux)>0)
        {
            strcat(rez,aux);
            strcat(rez,",");
        }
        a = strtok(NULL,",");
    }
    rez[strlen(rez)-1] = '\0';
    strcpy(comanda,rez);
    return true;
}

int vLogin(char mesaj[1024])
{
    char aux[100];
    strcpy(aux,"- -");
    while(validareTip(aux) == false)
    {
        printf("    Introdu numele de utilizator.(numele userului nu contine spatii)\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcpy(mesaj,aux);
    strcat(mesaj," ");
    strcpy(aux,"- -");
    while(validareTip(aux) == false)
    {
        printf("    Introdu parola.(parola nu contine spatii)\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcat(mesaj,aux);
    return 1;
}

int vCautaCarte(char mesaj[1024])
{
    char aux[100];
    strcpy(aux,"- -");
    while(validareTip(aux) == false)
    {
        printf("    Introdu ISBN.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        char enter[3];
        enter[0] ='\n';
        enter[1] = '\0';
        if(strcmp(enter,aux) == 0)
        {
            strcpy(aux,"ALL");
            break;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcpy(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);
    printf("    Introdu titlul sau 'ALL'/ENTER pentru toate titlurile.\n");
    fflush(stdout);
    if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
    trimSpatiu(aux);
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);
    printf("    Introdu autori despartiti prin virgula sau 'ALL'/ENTER pentru toti autorii.\n");
    fflush(stdout);
    if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
    desparteVirgula(aux);
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);
    printf("    Introdu genurile despartite prin virgula sau 'ALL'/ENTER pentru toate.\n");
    fflush(stdout);
    if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
    desparteVirgula(aux);
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);
    printf("    Introdu subgenurile despartite prin virgula sau 'ALL'/ENTER pentru toate.\n");
    fflush(stdout);
    if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
    desparteVirgula(aux);
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);
    
    strcpy(aux,"a");
    while(!isNumber(aux))
    {
        printf("    Introduceti anul aparitiei sau 'ALL'/ENTER pentru toti anii.\n");
        if(fgets(aux,100,stdin) == NULL)
            {
                printf("Eroare la citirea comenzii.\n");
                fflush(stdout);
                return -1;
            }
        char enter[3];
        enter[0] ='\n';
        enter[1] = '\0';
        if(strcmp(enter,aux) == 0)
        {
            strcpy(aux,"ALL");
            break;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);
    strcpy(aux,"a");
    while(!isNumber(aux))
    {
        printf("    Introduceti ratingul minim sau 'ALL'/ENTER.\n");
        if(fgets(aux,100,stdin) == NULL)
            {
                printf("Eroare la citirea comenzii.\n");
                fflush(stdout);
                return -1;
            }
        char enter[3];
        enter[0] ='\n';
        enter[1] = '\0';
        if(strcmp(enter,aux) == 0)
        {
            strcpy(aux,"ALL");
            break;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcat(mesaj,aux);
    return 1;
}

int vAdaugaCarte(char mesaj[1024])
{
    char aux[100];
    strcpy(aux,"- -");
    while(validareTip(aux) == false)
    {
        printf("    Introdu ISBN.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcpy(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);

    char enter[3];
    enter[0] ='\n';
    enter[1] = '\0';
    strcpy(aux,enter);
    while(strcmp(aux,enter) == 0)
    {
        printf("    Introdu titlul.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
            {
                printf("Eroare la citirea comenzii.\n");
                fflush(stdout);
                return -1;
            }
    }
    trimSpatiu(aux);
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);

    strcpy(aux,"a");
    while(!isNumber(aux) || strcmp(aux,enter) == 0)
    {
        printf("    Introduceti anul_aparitiei.\n");
        if(fgets(aux,100,stdin) == NULL)
            {
                printf("Eroare la citirea comenzii.\n");
                fflush(stdout);
                return -1;
            }
        /*char enter[3];
        enter[0] ='\n';
        enter[1] = '\0';
        if(strcmp(enter,aux) == 0)
        {
            strcpy(aux,"ALL");
            break;
        }*/
        if(strcmp(enter,aux) != 0)
            aux[strlen(aux)-1] = '\0';
    }
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);

    strcpy(aux,enter);
    while(strcmp(aux,enter) == 0)
    {
        printf("    Introdu autorii despartiti prin virgula.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
            {
                printf("Eroare la citirea comenzii.\n");
                fflush(stdout);
                return -1;
            }
    }
    desparteVirgula(aux);
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);

    strcpy(aux,enter);
    while(strcmp(aux,enter) == 0)
    {
        printf("    Introdu id-ul autorilor despartit prin virgula.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
            {
                printf("Eroare la citirea comenzii.\n");
                fflush(stdout);
                return -1;
            }
    }
    desparteVirgula(aux);
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);

    strcpy(aux,enter);
    while(strcmp(aux,enter) == 0)
    {
        printf("    Introdu genul.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
            {
                printf("Eroare la citirea comenzii.\n");
                fflush(stdout);
                return -1;
            }
    }
    trimSpatiu(aux);
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);

    strcpy(aux,enter);
    while(strcmp(aux,enter) == 0)
    {
        printf("    Introdu subgenurile despartite prin virgula.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
            {
                printf("Eroare la citirea comenzii.\n");
                fflush(stdout);
                return -1;
            }
    }
    desparteVirgula(aux);
    strcat(mesaj,aux);
    strcat(mesaj,DELIMIT_CAUTA);

    strcpy(aux,enter);
    while(strcmp(aux,enter) == 0)
    {
        printf("    Introdu numele fisierului.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
            {
                printf("Eroare la citirea comenzii.\n");
                fflush(stdout);
                return -1;
            }
    }
    aux[strlen(aux)-1] = '\0';
    strcat(mesaj,aux);
    return 1;
}

int vStergeCarte(char mesaj[1024])
{
    char aux[100];
    strcpy(aux,"- -");
    while(validareTip(aux) == false || !isNumber(aux))
    {
        printf("    Introdu ISBN.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcpy(mesaj,aux);
    return 1;
}

int vAdaugaUser(char mesaj[1024])
{
    char aux[100];
    strcpy(aux,"- -");
    while(validareTip(aux) == false)
    {
        printf("    Introdu numele de utilizator.(numele userului nu contine spatii)\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcpy(mesaj,aux);
    strcat(mesaj," ");

    strcpy(aux,"- -");
    while(validareTip(aux) == false)
    {
        printf("    Introdu parola.(parola nu contine spatii)\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcat(mesaj,aux);
    strcat(mesaj," ");

    strcpy(aux,"- -");
    fflush(stdout);
    while(validareTip(aux) == false || (aux[0] != '1' && aux[0] != '0') || strlen(aux)>1)
    {
        printf("    Introdu 0-user normal 1-administrator.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcat(mesaj,aux);
    return 1;
}

void vRecomanda(char mesaj[1024])
{
    char aux[100];
}

int vVoteaza(char mesaj[1024])
{
    char aux[100];
    strcpy(aux,"- -");
    while(validareTip(aux) == false || !isNumber(aux))
    {
        printf("    Introdu ISBN.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcpy(mesaj,aux);
    strcat(mesaj," ");

    char enter[3];
    enter[0] ='\n';
    enter[1] = '\0';
    strcat(aux,"- -");
    while(!isNumber(aux) || strcmp(aux,enter) == 0 || aux[0] > '5' || aux[0] < '1')
    {
        printf("    Introduceti votul (intre 1 si 5).\n");
        if(fgets(aux,100,stdin) == NULL)
            {
                printf("Eroare la citirea comenzii.\n");
                fflush(stdout);
                return -1;
            }
        if(strcmp(enter,aux) != 0)
            aux[strlen(aux)-1] = '\0';
    }
    strcat(mesaj,aux);

    return 1;
}

int vDescarca(char mesaj[1025])
{
    char aux[100];
    strcpy(aux,"- -");
    while(validareTip(aux) == false || !isNumber(aux))
    {
        printf("    Introdu ISBN.\n");
        fflush(stdout);
        if(fgets(aux,100,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        aux[strlen(aux)-1] = '\0';
    }
    strcpy(mesaj,aux);
}

void getFisier(char mesaj[1024], char fisier[30])
{
    char *p;
    p = strtok(mesaj,"$");
    while(p != NULL)
    {
        strcpy(fisier,p);
        p = strtok(NULL,"$");
    }
    fisier[strlen(fisier)] = 0;
}