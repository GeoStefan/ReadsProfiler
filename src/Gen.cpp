#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "../include/Gen.h"
#include "../include/BazaDate.h"

extern sqlite3* db;

Gen::Gen(char n[20])
{
    strcpy(nume,n);
}

void Gen::adaugaCG(char isbn[14],char nume_g[20])
{
    //sql
    char *sql = new char[200],sqli[200];
    strcpy(sqli,"INSERT INTO CARTEGENURI (ISBN,nume_gen) VALUES (");
    strcat(sqli,"'");
    strcat(sqli,isbn);
    strcat(sqli,"', '");
    strcat(sqli,nume_g);
    strcat(sqli,"');");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->inserare(sql);
}

bool Gen::adaugaGen( char gen[20], char subgen[20],char isbn[14])
{
    //sql
    char *sql = new char[200],sqli[200];
    strcpy(sqli,"INSERT INTO GEN (nume_gen,gen_parinte) VALUES (");
    strcat(sqli,"'");
    strcat(sqli,subgen);
    strcat(sqli,"', '");
    strcat(sqli,gen);
    strcat(sqli,"');");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->inserare(sql);

    adaugaCG(isbn,subgen);
}

bool Gen::stergeGen(char n[20])
{
    
}