#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "../include/Autor.h"
#include "../include/BazaDate.h"

extern sqlite3* db;

Autor::Autor(char n[20])
{
    strcpy(nume,n);
}

void Autor::adaugaCA(char isbn[14],int id)
{
    //sql
    char *sql = new char[200],sqli[200];
    char id_a[33];
    sprintf(id_a,"%d",id);

    strcpy(sqli,"INSERT INTO PUBLICATII VALUES (");
    strcat(sqli,"'");
    strcat(sqli,isbn);
    strcat(sqli,"'");
    strcat(sqli,", ");
    strcat(sqli,id_a);
    strcat(sqli,");");
    
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->inserare(sql);
}

void Autor::adaugaAG(int id,vector<string>genuri)
{
    //sql
    char *sql = new char[200],sqli[200];
    char id_a[33];
    sprintf(id_a,"%d",id);
    BazaDate *gg = new BazaDate();

    for(vector<string>::iterator it = genuri.begin(); it != genuri.end(); ++it)
    {
        strcpy(sqli,"INSERT INTO AUTORGENURI VALUES (");
        strcat(sqli,id_a);
        strcat(sqli,", '");
        char aux[20];
        strcpy(aux,(char*)(*it).c_str());
        aux[strlen(aux)] = 0;
        strcat(sqli,aux);
        strcat(sqli,"');");
        strcpy(sql,sqli);
        
        gg->inserare(sql);
    }
}

bool Autor::adaugaAutor(char n[20],int id,char isbn[14],vector<string>gen)
{
    //sql
    char *sql = new char[200],sqli[200];
    char id_a[33];
    sprintf(id_a,"%d",id);

    strcpy(sqli,"INSERT INTO AUTOR (id_autor,nume) VALUES ( ");
    strcat(sqli,id_a);
    strcat(sqli,", ");
    strcat(sqli,"'");
    strcat(sqli,n);
    strcat(sqli,"');");

    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->inserare(sql);

    adaugaCA(isbn,id);
    adaugaAG(id,gen);
    return true;
}

bool Autor::stergeAutor(int id)
{
    return true;
}