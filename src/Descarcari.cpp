#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h> 
#include "../include/Descarcari.h"
#include "../include/BazaDate.h"

using namespace std;

Descarcari::Descarcari()
{

}

void Descarcari::adaugaDescarcare(char client[20], char isbn[14])
{
    char *sql = new char[200], sqli[200];
    strcpy(sqli,"INSERT INTO DESCARCARI (client,ISBN) VALUES (");
    strcat(sqli,"'");
    strcat(sqli,client);
    strcat(sqli,"','");
    strcat(sqli,isbn);
    strcat(sqli,"');");

    BazaDate *gg;
    strcpy(sql,sqli);
    gg->inserare(sql);
    delete [] sql;
}

int Descarcari::selectRating(char client[20], char isbn[14])
{
    char *sql = new char[200], sqli[200];
    strcpy(sqli,"SELECT rating FROM DESCARCARI WHERE client = ");
    strcat(sqli,"'");
    strcat(sqli,client);
    strcat(sqli,"' AND ISBN = ");
    strcat(sqli,"'");
    strcat(sqli,isbn);
    strcat(sqli,"' LIMIT 1;");

    BazaDate *gg;
    strcpy(sql,sqli);
    return (gg->selectRatingDesc(sql));
}

bool Descarcari::updateDescarcare(char client[20], char isbn[14], int rating)
{
    char *sql = new char[200], sqli[200];
    char r[33];
    snprintf(r,sizeof(r),"%d",rating);
    strcpy(sqli,"UPDATE DESCARCARI SET rating = ");
    strcat(sqli,r);
    strcat(sqli," WHERE client =");
    strcat(sqli,"'");
    strcat(sqli,client);
    strcat(sqli,"' AND ISBN = ");
    strcat(sqli,"'");
    strcat(sqli,isbn);
    strcat(sqli,"' ;");

    BazaDate *gg;
    strcpy(sql,sqli);
    if(gg->update(sql))
    {

        return true;
        delete [] sql;
    }
    else
    {
        delete [] sql;
        return false;
    }
}

bool Descarcari::istoricDescarcari(char client[20], char rezultat[300])
{
    char *sql = new char[100], sqli[100];
    //char rezultat[300];
    strcpy(sqli,"SELECT * FROM DESCARCARI WHERE client = '");
    strcat(sqli,client);
    strcat(sqli,"';");
    strcpy(sql,sqli);
     
    BazaDate *gg = new BazaDate();
    gg->selectDescarcari(sql,rezultat);
    delete [] sql;
    return true;
}