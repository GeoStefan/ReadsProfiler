#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <sqlite3.h> 
#include "../include/Carte.h"
#include "../include/BazaDate.h"
#include "../include/Descarcari.h"

extern sqlite3* db;

Carte::Carte()
{
}

bool Carte::adaugaCarte(char isbn[14],char t[20], int a, vector<string> aut,vector<int>id_aut, char g[20], vector<string> subgen,char fis[30])
{
    char year[33];
    snprintf(year, sizeof(year), "%d", a);
    char *sql = new char[200],sqli[200];

    strcpy(sqli,"INSERT INTO CARTE (ISBN,titlu,an,fisier) ");
    strcat(sqli," VALUES (");
    strcat(sqli,"'");
    strcat(sqli,isbn);
    strcat(sqli,"'");
    strcat(sqli,", ");
    strcat(sqli,"'");
    strcat(sqli,t);
    strcat(sqli,"'");
    strcat(sqli,", ");
    strcat(sqli,year);
    strcat(sqli,", ");
    strcat(sqli,"'");
    strcat(sqli,fis);
    strcat(sqli,"'");
    strcat(sqli," );");
    sqli[strlen(sqli)]='\0';

    strcpy(sql,sqli);

    printf("%s\n",sqli);

    BazaDate *gg = new BazaDate();
    if(gg->inserare(sql))
    {
        vector<int>::iterator it1 = id_aut.begin();
        for(vector<string>::iterator it = aut.begin() ; it != aut.end() && it1 != id_aut.end(); ++it, ++it1)
        {
            char aux[20];
            strcpy(aux,(char*)((*it).c_str()));
            aux[strlen(aux)] = 0;
            autor->adaugaAutor(aux,*(it1),isbn,subgen);
            
            int x = *(it1);
        }
        for(vector<string>::iterator it2 = subgen.begin() ; it2 != subgen.end(); ++it2)
        {
            char aux[20];
            strcpy(aux,(char*)((*it2).c_str()));
            aux[strlen(aux)] = 0;
            gen->adaugaGen(g,aux,isbn);
        }
    }
    else
        return false;
    return true;
}

void Carte::stergeCG(char isbn[14])
{
    char *sql = new char[200],sqli[200];
    strcpy(sqli,"DELETE FROM CARTEGENURI WHERE ISBN = '");
    strcat(sqli,isbn);
    strcat(sqli,"';");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->stergere(sql);
}

void Carte::stergePublicatie(char isbn[14])
{
    char *sql = new char[200],sqli[200];
    strcpy(sqli,"DELETE FROM PUBLICATII WHERE ISBN = '");
    strcat(sqli,isbn);
    strcat(sqli,"';");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->stergere(sql);
}

bool Carte::stergeCarte(char isbn[14], char fis[30])
{
    if(existaCarte(isbn,fis))
    {
        char *sql = new char[200],sqli[200];
        strcpy(sqli,"DELETE FROM CARTE WHERE ISBN = '");
        strcat(sqli,isbn);
        strcat(sqli,"';");
        strcpy(sql,sqli);

        BazaDate *gg = new BazaDate();
        if(gg->stergere(sql))
        {
            stergeCG(isbn);
            int id_aut = 0;
            stergePublicatie(isbn);
        }
        else
            return false;

        return true;
    }
    else
        return false;

}

void Carte::updateRatingCarte(char isbn[14],int rating)
{
    char *sql = new char[200],sqli[200];
    int rat_s, rat_n;
    strcpy(sqli,"SELECT rating_s, rating_n FROM CARTE WHERE ISBN = ");
    strcat(sqli,"'");
    strcat(sqli,isbn);
    strcat(sqli,"';");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    gg->selectRatingCarte(rat_s,rat_n,sql);

    rat_s += rating;
    rat_n ++;
    char rs[33], rn[33];
    snprintf(rs, sizeof(rs), "%d", rat_s);
    snprintf(rn, sizeof(rn), "%d", rat_n);

    strcpy(sqli,"UPDATE CARTE SET rating_s = ");
    strcat(sqli,rs);
    strcat(sqli,", rating_n = ");
    strcat(sqli,rn);
    strcat(sqli," WHERE ISBN = ");
    strcat(sqli,"'");
    strcat(sqli,isbn);
    strcat(sqli,"';");
    strcpy(sql,sqli);

    gg->update(sql);
}

bool Carte::voteazaCarte(char isbn[14], char client[20], char rating[2])
{
    int r = atoi(rating);
    if(r>0 && r<6)
    {
        Descarcari *d = new Descarcari();
        int x = d->selectRating(client,isbn);
        if(x == 0)
        {
            if(d->updateDescarcare(client,isbn,r))
                updateRatingCarte(isbn,r);
        }
        else 
            return false;
    }
    else
        return false;
    return true;
}

int Carte::cautaCarte(char isbn[14],char t[20], vector<string> autori, vector<string> genuri, vector<string> subgenuri, char a[5], char rating[2], char rezultat[2000])
{
    char *sql = new char[500],sqli[500];
    strcpy(sqli,"SELECT c.ISBN, titlu, nume, gen_parinte, g.nume_gen, an, rating_s / rating_n FROM CARTE c " \
            " JOIN PUBLICATII p ON p.ISBN = c.ISBN " \
            " JOIN AUTOR a ON a.id_autor = p.id_autor " \
            " JOIN CARTEGENURI g1 on g1.ISBN = c.ISBN " \
            " JOIN GEN g on g.nume_gen = g1.nume_gen ");
    
    bool ok=0;
    if(strcmp(isbn,"ALL")!=0)
    { 
        strcat(sqli," WHERE ");
        strcat(sqli," c.ISBN = '");
        strcat(sqli,isbn);
        strcat(sqli,"' ");
        ok=1;
    }
    if(strcmp(t,"ALL")!=0)
    { 
        if(ok)
            strcat(sqli," AND ");
            else strcat(sqli," WHERE ");
        strcat(sqli,"titlu = '");
        strcat(sqli,t);
        strcat(sqli,"' ");
        ok=1;
    }
    if(autori.front() != "ALL")
    { 
        if(ok)
            strcat(sqli," AND ");
        else strcat(sqli," WHERE ");
        strcat(sqli," nume IN ( ");
        vector<string>::iterator it = autori.begin();
        strcat(sqli,"'");
        char aux[20];
        strcpy(aux,(char*)((*it).c_str()));
        aux[strlen(aux)] = 0;
        strcat(sqli,aux);
        strcat(sqli,"'");
        ++it;
        for(; it != autori.end(); ++it)
        {
            strcat(sqli,", ");
            strcat(sqli,"'");
            strcpy(aux,(char*)((*it).c_str()));
            aux[strlen(aux)] = 0;
            strcat(sqli,aux);
            strcat(sqli,"'");
        }
        strcat(sqli,")");
        ok=1;
    }
    if(genuri.front() != "ALL")
    { 
        if(ok)
            strcat(sqli," AND ");
        else strcat(sqli," WHERE ");
        strcat(sqli,"gen_parinte IN ( ");
        vector<string>::iterator it = genuri.begin();
        strcat(sqli,"'");
        char aux[20];
        strcpy(aux,(char*)((*it).c_str()));
        aux[strlen(aux)] = 0;
        strcat(sqli,aux);
        strcat(sqli,"'");
        ++it;
        for(; it != genuri.end(); ++it)
        {
            strcat(sqli,", ");
            strcat(sqli,"'");
            strcpy(aux,(char*)((*it).c_str()));
            aux[strlen(aux)] = 0;
            strcat(sqli,aux);
            strcat(sqli,"'");
        }
        strcat(sqli,")");
        ok=1;
    }
    if(subgenuri.front() != "ALL")
    { 
        if(ok)
            strcat(sqli," AND ");
        else strcat(sqli," WHERE ");
        strcat(sqli," g.nume_gen IN ( ");
        vector<string>::iterator it = subgenuri.begin();
        strcat(sqli,"'");
        char aux[20];
        strcpy(aux,(char*)((*it).c_str()));
        aux[strlen(aux)] = 0;
        strcat(sqli,aux);
        strcat(sqli,"'");
        ++it;
        for(; it != subgenuri.end(); ++it)
        {
            strcat(sqli,", ");
            strcat(sqli,"'");
            strcpy(aux,(char*)((*it).c_str()));
            aux[strlen(aux)] = 0;
            strcat(sqli,aux);
            strcat(sqli,"'");
        }
        strcat(sqli,")");
        ok=1;
    }
    if(strcmp(a,"ALL")!=0)
    { 
        if(ok)
            strcat(sqli," AND ");
            else strcat(sqli," WHERE ");
        strcat(sqli,"an = ");
        strcat(sqli,a);
        ok=1;
    }
    if(strcmp(rating,"ALL")!=0)
    { 
        if(ok)
            strcat(sqli," AND ");
            else strcat(sqli," WHERE ");
        strcat(sqli,"rating_s/rating_n >= ");
        strcat(sqli,rating);
    }
    strcat(sqli," ORDER BY c.ISBN, titlu, nume, gen_parinte;");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    //char rezultat[1000];
    int nr_carti = gg->cautareCarte(sql,rezultat);

    //printf("%s",rezultat);

    return nr_carti;
}

bool Carte::existaCarte(char isbn[14],char fis[30])
{
    char *sql = new char[200],sqli[200];
    strcpy(sqli,"   SELECT fisier FROM CARTE WHERE ISBN ='");
    strcat(sqli,isbn);
    strcat(sqli,"';");
    strcpy(sql,sqli);

    BazaDate *gg = new BazaDate();
    if(gg->selectFisCarte(sql,fis))
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