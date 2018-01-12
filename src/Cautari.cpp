#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include "../include/Cautari.h"
#include "../include/BazaDate.h"

using namespace std;

Cautari::Cautari()
{

}

void Cautari::adaugaCautare(char client[20], char isbn[14],char t[20], vector<string> autori, vector<string>genuri, vector<string>subgenuri, char a[5], char rating[2])
{
    char *sql = new char[200], sqli[200];
    strcpy(sqli,"INSERT INTO CAUTARI (client,ISBN,titlu,an,rating,autor,gen,subgen) VALUES ( ");
    strcat(sqli,"'");
    strcat(sqli,client);
    strcat(sqli,"','");
    strcat(sqli,isbn);
    strcat(sqli,"','");
    strcat(sqli,t);
    strcat(sqli,"','");
    strcat(sqli,a);
    strcat(sqli,"','");
    strcat(sqli,rating);
    strcat(sqli,"',");
    BazaDate *gg;

    for(vector<string>::iterator ita = autori.begin(); ita != autori.end(); ++ita)
    {
        char sq[200], aux[20];
        strcpy(sq,sqli);
        strcat(sq,"'");
        strcpy(aux,(char*)((*ita).c_str()));
        aux[strlen(aux)] = 0;
        strcat(sq,aux);
        strcat(sq,"',");
        for(vector<string>::iterator itg = genuri.begin(); itg != genuri.end(); ++itg)
        {
            char sq2[200], aux2[20];
            strcpy(sq2,sq);
            strcat(sq2,"'");
            strcpy(aux2,(char*)((*itg).c_str()));
            aux2[strlen(aux2)] = 0;
            strcat(sq2,aux2);
            strcat(sq2,"',");
            for(vector<string>::iterator its = subgenuri.begin(); its != subgenuri.end(); ++its)
            {
                char sq3[200], aux3[20];
                strcpy(sq3,sq2);
                strcat(sq3,"'");
                strcpy(aux3,(char*)((*its).c_str()));
                aux3[strlen(aux3)] = 0;
                strcat(sq3,aux3);
                strcat(sq3,"');");
                strcpy(sql,sq3);
                gg->inserare(sql);
            }
        }
    }

    delete [] sql;
}

void Cautari::istoricCautari(char client[20], char rezultat[2300])
{
    char *sql = new char[100], sqli[100];
    //char rezultat[2100];
    strcpy(sqli,"SELECT * FROM CAUTARI WHERE client ='");
    strcat(sqli,client);
    strcat(sqli,"' LIMIT 20;");

    strcpy(sql,sqli);
    BazaDate *gg = new BazaDate();
    gg->selectCautari(sql,rezultat);
    char *rez = rezultat;
    delete [] sql;
}