#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "../include/Client.h"
#include "../include/BazaDate.h"
#include "../include/Carte.h"

Client::Client()
{

}

Client::Client(char *n)
{
    strcpy(nume,n);
}

void Client::setNume(char n[20])
{
    strcpy(nume,n);
}

bool Client::adaugaClient(char nume[20], char parola[20],  int admin)
{
    char *sql = new char[200], sqli[200];
    char adm[33];
    snprintf(adm,sizeof(adm),"%d",admin);

    strcpy(sqli,"INSERT INTO CLIENT (nume,parola,admin,pref_gen1,pref_gen2,pref_aut1,pref_aut2) VALUES (");
    strcat(sqli,"'");
    strcat(sqli,nume);
    strcat(sqli,"', '");
    strcat(sqli,parola);
    strcat(sqli,"', ");
    strcat(sqli,adm);
    strcat(sqli,", 'ALL', 'ALL', 'ALL', 'ALL');");

    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    if(gg->inserare(sql))
    {
        delete [] sql;
        return true;
    }
    else
    { 
        delete [] sql;
        return false;
    }
}

bool Client::stergeClient(char nume[20])
{
    char *sql = new char[200], sqli[200];
    strcpy(sqli,"DELETE FROM CLIENT WHERE nume = ");
    strcat(sqli,"'");
    strcat(sqli,nume);
    strcat(sqli,"';");

    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    if(gg->stergere(sql))
        return true;
    else 
        return false;
}

void Client::selectPrefGen1(char rezultat[20])
{
    char *sql = new char[200], sqli[200];
    
    strcpy(sqli,"SELECT gen, count(gen) FROM CAUTARI WHERE client = ");
    strcat(sqli,"'");
    strcat(sqli,nume);
    strcat(sqli,"' AND gen != 'ALL' GROUP BY gen ORDER BY COUNT(gen) DESC LIMIT 1;");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->selectGenClient(sql,rezultat);
    delete [] sql;
}

void Client::selectPrefGen2(char rezultat[20])
{
    char *sql = new char[300], sqli[300];

    strcpy(sqli,"SELECT gen_parinte, count(gen_parinte) FROM DESCARCARI d" \
            " JOIN CARTEGENURI c ON d.ISBN = c.ISBN " \
            " JOIN GEN g ON g.nume_gen = c.nume_gen " \
            " WHERE client = ");
    strcat(sqli,"'");
    strcat(sqli,nume);
    strcat(sqli,"' GROUP BY gen_parinte " \
            " ORDER BY count(gen_parinte) DESC " \
            " LIMIT 1;");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->selectGenClient(sql,rezultat);
    delete [] sql;
}

void Client::selectPrefAut1(char rezultat[20])
{
    char *sql = new char[200], sqli[200];
    strcpy(sqli,"SELECT id_autor, COUNT(id_autor) FROM CAUTARI c " \
            " JOIN AUTOR a on a.nume = c.autor " \
            " WHERE client = ");
    strcat(sqli,"'");
    strcat(sqli,nume);
    strcat(sqli,"' AND id_autor != 'ALL' GROUP BY id_autor "\
                " ORDER BY COUNT(id_autor) DESC " \
                " LIMIT 1;");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->selectAutClient(sql,rezultat);
    delete [] sql;
}

void Client::selectPrefAut2(char rezultat[20])
{
    char *sql = new char[300], sqli[300];
    strcpy(sqli,"SELECT a.id_autor, COUNT(a.id_autor) FROM AUTOR a " \
                " JOIN PUBLICATII p ON p.id_autor = a.id_autor " \
                " JOIN DESCARCARI d ON d.ISBN = p.ISBN " \
                " WHERE client = ");
    strcat(sqli,"'");
    strcat(sqli,nume);
    strcat(sqli,"' GROUP BY a.id_autor "\
                " ORDER BY COUNT(a.id_autor) DESC " \
                " LIMIT 1;");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->selectAutClient(sql,rezultat);
    delete [] sql;
}

void Client::adaugaPrefClient(char gen1[20], char gen2[20], char aut1[20], char aut2[20])
{
    char *sql = new char[300], sqli[300];
    strcpy(sqli,"UPDATE CLIENT SET pref_gen1 = ");
    strcat(sqli,"'");
    strcat(sqli,gen1);
    strcat(sqli,"', pref_gen2 = '");
    strcat(sqli,gen2);
    strcat(sqli,"', pref_aut1 = '");
    strcat(sqli,aut1);
    strcat(sqli,"', pref_aut2 = '");
    strcat(sqli,aut2);
    strcat(sqli,"' WHERE nume = '");
    strcat(sqli,nume);
    strcat(sqli,"';");

    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->inserare(sql);
    delete [] sql;
}

void Client::getPrefGen1(char rezultat[20])
{
    char *sql = new char[100], sqli[100];
    strcpy(sqli,"SELECT pref_gen1 FROM CLIENT WHERE nume ='");
    strcat(sqli,nume);
    strcat(sqli,"';");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->selectGenClient(sql,rezultat);
    delete [] sql;
}

void Client::getPrefGen2(char rezultat[20])
{
    char *sql = new char[100], sqli[100];
    strcpy(sqli,"SELECT pref_gen2 FROM CLIENT WHERE nume ='");
    strcat(sqli,nume);
    strcat(sqli,"';");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->selectGenClient(sql,rezultat);
    delete [] sql;
}

void Client::getPrefAut1(char rezultat[20])
{
    char *sql = new char[100], sqli[100];
    strcpy(sqli,"SELECT pref_aut1 FROM CLIENT WHERE nume ='");
    strcat(sqli,nume);
    strcat(sqli,"';");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->selectAutClient(sql,rezultat);
    delete [] sql;
}

void Client::getPrefAut2(char rezultat[20])
{
    char *sql = new char[100], sqli[100];
    strcpy(sqli,"SELECT pref_aut2 FROM CLIENT WHERE nume ='");
    strcat(sqli,nume);
    strcat(sqli,"';");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->selectAutClient(sql,rezultat);
    delete [] sql;
}

bool Client::verificaClient(char user[20], char pass[20])
{
    char *sql = new char[100], sqli[100];
    strcpy(sqli,"SELECT nume FROM CLIENT WHERE nume = '");
    strcat(sqli,user);
    strcat(sqli,"' AND parola = '");
    strcat(sqli,pass);
    strcat(sqli,"';");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    if(gg->existaClient(sql))
    {
        delete [] sql;
        return true;
    }
    else
    {
        delete [] sql;
        return false;
    }
}

int Client::getUserType(char user[20])
{
    char *sql = new char[100], sqli[100];
    strcpy(sqli,"SELECT admin FROM CLIENT WHERE nume = '");
    strcat(sqli,user);
    strcat(sqli,"' LIMIT 1;");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    int tip = gg->selectClientType(sql);
    return tip;
}