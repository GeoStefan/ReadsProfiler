#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "../include/Recomandare.h"
#include "../include/BazaDate.h"

using namespace std;

Recomandare::Recomandare()
{
}

void Recomandare::recomanda(Client *client, char rezultat[2000])
{
    //char *rezultat1 = new char[1500];
    char rezultat1[1500];
    char gen1[20], gen2[20], aut1[20], aut2[20];
    client->getPrefGen1(gen1);
    client->getPrefGen2(gen2);
    client->getPrefAut1(aut1);
    client->getPrefAut2(aut2);

    //printf("%s %s %s %s\n",gen1,gen2,aut1,aut2);

    char *sql = new char[1500], sqli[1500];
    strcpy(sqli,"SELECT c.ISBN, titlu, nume, gen_parinte, g.nume_gen, an, rating_s / rating_n, COUNT(d.ISBN) FROM CARTE c " \
            " JOIN PUBLICATII p ON p.ISBN = c.ISBN " \
            " JOIN AUTOR a ON a.id_autor = p.id_autor " \
            " JOIN CARTEGENURI g1 on g1.ISBN = c.ISBN " \
            " JOIN GEN g on g.nume_gen = g1.nume_gen " \
            " LEFT JOIN DESCARCARI d on d.ISBN = c.ISBN ");
    int ok = 0;
    if(strcmp(gen1,"ALL") != 0 && strcmp(gen2,"ALL") !=0)
    {
        strcat(sqli," WHERE ");
        strcat(sqli,"( gen_parinte = '");
        strcat(sqli,gen1);
        strcat(sqli,"' OR gen_parinte ='");
        strcat(sqli,gen2);
        strcat(sqli,"') ");
        ok = 1;
    }
    else
    {
        if(strcmp(gen1,"ALL") != 0)
            {
                strcat(sqli," WHERE ");
                strcat(sqli," gen_parinte = '");
                strcat(sqli,gen1);;
                strcat(sqli,"' ");
                ok = 1;
            }
        if(strcmp(gen2,"ALL") != 0)
            {
                strcat(sqli," WHERE ");
                strcat(sqli," gen_parinte = '");
                strcat(sqli,gen2);;
                strcat(sqli,"' ");
                ok = 1;
            }
    }
    if(strcmp(aut1,"ALL") != 0 && strcmp(aut2,"ALL") !=0)
    {
        if(ok)
            strcat(sqli," AND ");
        else
            strcat(sqli," WHERE ");
        strcat(sqli,"( a.id_autor = '");
        strcat(sqli,aut1);
        strcat(sqli,"' OR a.id_autor ='");
        strcat(sqli,aut2);
        strcat(sqli,"') ");
        ok = 1;
    }
    else
    {
        if(strcmp(aut1,"ALL") != 0)
            {
                if(ok)
                    strcat(sqli," AND ");
                else
                    strcat(sqli," WHERE ");
                strcat(sqli," a.id_autor = '");
                strcat(sqli,aut1);;
                strcat(sqli,"' ");
                ok = 1;
            }
        if(strcmp(aut2,"ALL") != 0)
            {
                if(ok)
                    strcat(sqli," AND ");
                else
                    strcat(sqli," WHERE ");
                strcat(sqli," a.id_autor = '");
                strcat(sqli,aut2);;
                strcat(sqli,"' ");
                ok = 1;
            }
    }
    if(ok)
        strcat(sqli," AND (rating_n IS NULL OR rating_s / rating_n > 2) ");
    else
        strcat(sqli," WHERE (rating_n = NULL OR rating_s / rating_n > 2) ");
    strcat(sqli," GROUP BY c.ISBN, titlu, nume, gen_parinte, g.nume_gen, an, rating_s / rating_n " \
                " ORDER BY c.ISBN, titlu, nume, gen_parinte, COUNT(d.ISBN) DESC LIMIT 6;");
    strcpy(sql,sqli);
    BazaDate *gg = new BazaDate();
    gg->cautareCarte(sql,rezultat1);
    rezultat1[strlen(rezultat1)] = 0;

    strcpy(rezultat,"   Carti recomandate in functie de preferintele dvs. :");
    rezultat[strlen(rezultat)]='\n';
    //rezultat[strlen(rezultat)]='\0';
    strcat(rezultat,rezultat1);
    rezultat[strlen(rezultat)] = 0;
    //delete [] rezultat1;


    char alt_client[300];
    strcpy(alt_client," SELECT nume FROM CLIENT ");
    ok = 0;
    if(strcmp(gen1,"ALL") != 0 && strcmp(gen2,"ALL") !=0)
    {
        strcat(alt_client," WHERE ");
        strcat(alt_client,"(( pref_gen1 = '");
        strcat(alt_client,gen1);
        strcat(alt_client,"' OR pref_gen1 ='");
        strcat(alt_client,gen2);
        strcat(alt_client,"') OR ");
        strcat(alt_client,"( pref_gen2 = '");
        strcat(alt_client,gen1);
        strcat(alt_client,"' OR pref_gen2 ='");
        strcat(alt_client,gen2);
        strcat(alt_client,"')) ");

        ok = 1;
    }
    else
    {
        if(strcmp(gen1,"ALL") != 0)
            {
                strcat(alt_client," WHERE ");
                strcat(alt_client,"( pref_gen1 = '");
                strcat(alt_client,gen1);;
                strcat(alt_client,"' ");
                strcat(alt_client," OR ");
                strcat(alt_client," pref_gen2 = '");
                strcat(alt_client,gen1);;
                strcat(alt_client,"') ");
                ok = 1;                
            }
        if(strcmp(gen2,"ALL") != 0)
            {
                strcat(alt_client," WHERE ");
                strcat(alt_client,"( pref_gen1 = '");
                strcat(alt_client,gen2);;
                strcat(alt_client,"' ");
                strcat(alt_client," OR ");
                strcat(alt_client," pref_gen2 = '");
                strcat(alt_client,gen2);;
                strcat(alt_client,"') ");
                ok = 1;
            }
    }
    if(strcmp(aut1,"ALL") != 0 && strcmp(aut2,"ALL") !=0)
    {
        if(ok)
            strcat(alt_client," AND ");
        else
            strcat(alt_client," WHERE ");
        strcat(alt_client,"(( pref_aut1 = '");
        strcat(alt_client,aut1);
        strcat(alt_client,"' OR pref_aut1 ='");
        strcat(alt_client,aut2);
        strcat(alt_client,"') ");
        ok = 1;
        strcat(alt_client," OR ");
        strcat(alt_client,"( pref_aut2 = '");
        strcat(alt_client,aut1);
        strcat(alt_client,"' OR pref_aut2 ='");
        strcat(alt_client,aut2);
        strcat(alt_client,"')) ");
    }
    else
    {
        if(strcmp(aut1,"ALL") != 0)
            {
                if(ok)
                    strcat(alt_client," AND ");
                else
                    strcat(alt_client," WHERE ");
                strcat(alt_client,"( pref_aut1 = '");
                strcat(alt_client,aut1);;
                strcat(alt_client,"' ");
                strcat(alt_client," OR ");
                strcat(alt_client," pref_aut2 = '");
                strcat(alt_client,aut1);;
                strcat(alt_client,"') ");   
                ok= 1;                 
            }
        if(strcmp(aut2,"ALL") != 0)
            {
                if(ok)
                    strcat(alt_client," AND ");
                else
                    strcat(alt_client," WHERE ");
                strcat(alt_client,"( pref_aut1 = '");
                strcat(alt_client,aut2);;
                strcat(alt_client,"' ");
                strcat(alt_client," OR ");
                strcat(alt_client," pref_aut2 = '");
                strcat(alt_client,aut2);
                strcat(alt_client,"') ");   
                ok= 1;                  
            }
    }
    if(ok)
        strcat(alt_client," AND ");
    else
        strcat(alt_client," WHERE");
    strcat(alt_client," nume != '");
    strcat(alt_client,client->nume);
    strcat(alt_client,"' ");
    strcpy(sqli,"SELECT c.ISBN, titlu, nume, gen_parinte, g.nume_gen, an, rating_s / rating_n, COUNT(d.ISBN) FROM CARTE c " \
            " JOIN PUBLICATII p ON p.ISBN = c.ISBN " \
            " JOIN AUTOR a ON a.id_autor = p.id_autor " \
            " JOIN CARTEGENURI g1 on g1.ISBN = c.ISBN " \
            " JOIN GEN g on g.nume_gen = g1.nume_gen " \
            " LEFT JOIN DESCARCARI d on d.ISBN = c.ISBN ");
    strcat(sqli," WHERE client IN ( ");
    strcat(sqli,alt_client);
    strcat(sqli,") AND (rating_n IS NULL OR rating_s / rating_n > 2) "\
                " GROUP BY c.ISBN, titlu, nume, gen_parinte, g.nume_gen, an, rating_s / rating_n " \
                " ORDER BY c.ISBN, titlu, nume, gen_parinte, COUNT(d.ISBN) DESC LIMIT 6;");

    strcpy(sql,sqli);
    //char *rezultat2 = new char[1000];
    char rezultat2[1000];
    gg->cautareCarte(sql,rezultat2);
    rezultat2[strlen(rezultat)] = 0;

    strcat(rezultat,"   Carti descarcate si apreciate de utilizatori cu gusturi similare:");
    rezultat[strlen(rezultat)]='\n';
    rezultat[strlen(rezultat)]='\0';
    strcat(rezultat,rezultat2);
    rezultat[strlen(rezultat)] = 0;

    //delete [] rezultat2;
    delete [] sql;
}