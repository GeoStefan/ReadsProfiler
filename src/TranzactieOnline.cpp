#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sqlite3.h>
#include "../include/Carte.h"
#include "../include/Autor.h"
#include "../include/Gen.h"
#include "../include/Client.h"
#include "../include/Cautari.h"
#include "../include/Descarcari.h"
#include "../include/Recomandare.h"
#include "../include/BazaDate.h"
#include "../include/TranzactieOnline.h"

using namespace std;


bool TranzactieOnline::Login(char mesaj[1024], char raspuns[3000], char user[20], int &tip_user)
{
    bzero(raspuns,3000);
    char nume[20], parola[20], *p;
    p = strtok(mesaj," ");
    strcpy(nume,p);
    p = strtok(NULL," ");
    strcpy(parola,p);
    nume[strlen(nume)] = 0;
    parola[strlen(parola)] = 0;

    Client *c = new Client();
    if(c->verificaClient(nume,parola))
    {
        strcpy(raspuns,"    Te-ai logat cu succes.\n");
        strcpy(user,nume);
        tip_user = c->getUserType(user);
        printf("User:%s Usertype:%d\n",user,tip_user);
        return true;
    }
    else
    {
        strcpy(raspuns,"    Nume de utilizator sau parola incorecte.\n");
        return false;
    }

}
bool TranzactieOnline::AdaugaUser(char mesaj[1024], char raspuns[3000])
{
    bzero(raspuns,3000);
    char nume[20], parola[20], *p, adm[3];
    int admin;
    p = strtok(mesaj," ");
    strcpy(nume,p);
    p = strtok(NULL," ");
    strcpy(parola,p);
    nume[strlen(nume)] = 0;
    parola[strlen(parola)] = 0;
    p = strtok(NULL," ");
    strcpy(adm,p);
    if(adm[0] == '1')
        admin = 1;
    else
        admin = 0;
    Client *c = new Client();
    if(c->adaugaClient(nume,parola,admin))
    {
        strcpy(raspuns,"    Clientul a fost adaugat cu succes.\n");
        return true;
    }
    else
    {
        strcpy(raspuns,"    Numele de utilizator exista in baza de date.\n");
        return false;
    }

}
bool TranzactieOnline::AdaugaCarte(char mesaj[1024], char raspuns[3000], char fisier[30])
{
    bzero(raspuns,3000);
    char isbn[14],titlu[20],fis[30], gen[20], *p, *p2, aux[1024],year[5], aux2[20], saut[200],sid_aut[200],ssubgen[200];
    int a,id;
    
    vector<int>id_aut;
    vector<string> aut2;
    vector<string> subgen2;
    string aux3;
    p = strtok(mesaj,"$");
    strcpy(isbn,p);     
    isbn[strlen(isbn)] = 0; printf("%s",isbn); fflush(stdout);
    p = strtok(NULL,"$");
    strcpy(titlu,p);       
    titlu[strlen(titlu)] =0; printf("%s",titlu); fflush(stdout);
    p = strtok(NULL,"$");
    strcpy(year,p);         
    a = atoi(year);     printf("%d",a); fflush(stdout);

    p = strtok(NULL,"$");
    strcpy(saut,p);
    p = strtok(NULL,"$");
    strcpy(sid_aut,p);
    p = strtok(NULL,"$");
    strcpy(gen,p);                  
    gen[strlen(gen)] = 0;   printf("%s",gen); fflush(stdout);
    p = strtok(NULL,"$");   
    strcpy(ssubgen,p);
    p = strtok(NULL,"$");
    strcpy(fis,p);                 
    fis[strlen(fis)] = 0;   printf("%s",fis); fflush(stdout);

    p2 = strtok(saut,",");
    while(p2 != NULL)
    {
        strcpy(aux2,p2);        printf("%s",aux2); fflush(stdout);
        aux2[strlen(aux2)] = 0;
        aux3 = aux2;
        aut2.push_back(aux3);
        p2 = strtok(NULL,",");
    }
    
    p2 = strtok(sid_aut,",");
    while(p2 != NULL)
    {
        strcpy(aux2,p2);            printf("%s",aux2); fflush(stdout);
        id = atoi(aux2);
        id_aut.push_back(id);
        p2 = strtok(NULL,",");
    }
    p2 = strtok(ssubgen,",");
    while(p2 != NULL)
    {
        strcpy(aux2,p2);
        aux2[strlen(aux2)] = 0;        printf("%s",aux2); fflush(stdout);
        aux3 = aux2;
        subgen2.push_back(aux3);    
        p2 = strtok(NULL,",");
    }

    Carte *c = new Carte();
    if(c->adaugaCarte(isbn,titlu,a,aut2,id_aut,gen,subgen2,fis))
    {
        strcpy(raspuns,"    Cartea a fost adaugata cu succes.\n");
        strcpy(fisier,fis);
        return true;
    }
    else
    {
        strcpy(raspuns,"    Cartea nu a fost adaugata.\n");
        return false;
    }

}
bool TranzactieOnline::CautaCarte(char mesaj[1024], char raspuns[3000], char user[20])
{
    bzero(raspuns,3000);
    char isbn[14],titlu[20],fis[20], *p, *p2, aux[1024],an[5],rating[2], aux2[20], saut[200], sgen[200],ssubgen[200];
    
    vector<int>id_aut;
    vector<string> aut2;
    vector<string> gen2;
    vector<string> subgen2;
    string aux3;
    p = strtok(mesaj,"$");
    strcpy(isbn,p);     
    isbn[strlen(isbn)] = 0; //printf("%s",isbn); fflush(stdout);
    p = strtok(NULL,"$");
    strcpy(titlu,p);       
    titlu[strlen(titlu)] =0; //printf("%s",titlu); fflush(stdout);

    p = strtok(NULL,"$");
    strcpy(saut,p);
    
    p = strtok(NULL,"$");
    strcpy(sgen,p);                  //printf("%s",sgen); fflush(stdout);
    p = strtok(NULL,"$");   
    strcpy(ssubgen,p);

    p = strtok(NULL,"$");
    strcpy(an,p);         
    an[strlen(an)] =0;    //printf("%s",an); fflush(stdout);

    p = strtok(NULL,"$");
    strcpy(rating,p);         
    rating[strlen(rating)] =0;     //printf("%s",rating); fflush(stdout);

    p2 = strtok(saut,",");
    while(p2 != NULL)
    {
        strcpy(aux2,p2);        //printf("%s",aux2); fflush(stdout);
        aux2[strlen(aux2)] = 0;
        aux3 = aux2;
        aut2.push_back(aux3);
        p2 = strtok(NULL,",");
    }
    
    p2 = strtok(sgen,",");
    while(p2 != NULL)
    {
        strcpy(aux2,p2);
        aux2[strlen(aux2)] = 0;        //printf("%s",aux2); fflush(stdout);
        aux3 = aux2;
        gen2.push_back(aux3);    
        p2 = strtok(NULL,",");
    }

    p2 = strtok(ssubgen,",");
    while(p2 != NULL)
    {
        strcpy(aux2,p2);
        aux2[strlen(aux2)] = 0;        //printf("%s",aux2); fflush(stdout);
        aux3 = aux2;
        subgen2.push_back(aux3);    
        p2 = strtok(NULL,",");
    }

    Carte *c = new Carte();
    if(c->cautaCarte(isbn,titlu,aut2,gen2,subgen2,an,rating,raspuns))
    {
        printf("%s",raspuns);
        
        Cautari *ca = new Cautari();
        ca->adaugaCautare(user,isbn,titlu,aut2,gen2,subgen2,an,rating);
        Client *cl = new Client(user);
        char pa1[20], pa2[20], pg1[20], pg2[20];
        printf("TranzacieOnline::CautaCarte: afisarea noilor preferinte ale userului:\n");
        cl->selectPrefAut1(pa1);
        printf("%s\n",pa1);
        cl->selectPrefAut2(pa2);
        printf("%s\n",pa2);
        cl->selectPrefGen1(pg1);
        printf("%s\n",pg1);
        cl->selectPrefGen2(pg2);
        printf("%s\n",pg2);
        cl->adaugaPrefClient(pg1,pg2,pa1,pa2);
        delete c;
        delete ca;
        delete cl;
        return true;
    }
    else
    {
        delete c;
        return false;
    }
}
bool TranzactieOnline::StergeCarte(char mesaj[1024], char raspuns[3000])
{
    bzero(raspuns,3000);
    char fisier[30];
    Carte *c = new Carte();
    if(c->stergeCarte(mesaj,fisier))
    {
        char locatie[100];
        strcpy(locatie,"Carti/");
        strcat(locatie,fisier);
        if(remove(locatie) != 0)
        {
            strcpy(raspuns,"    A aparut o eroare la stergerea fisierului ");
            strcat(raspuns,fisier);
            raspuns[strlen(raspuns)] = '\n';
            raspuns[strlen(raspuns)] = 0;
            printf("Eroare la stergerea fisierului: %s\n",fisier);
            return false;
        }
        strcpy(raspuns,"    Cartea a fost stearsa cu succes.\n");
        return true;
    }
    else
    {
        strcpy(raspuns,"    Cartea nu a fost gasita in baza de date.\n");
        return false;
    }
}
bool TranzactieOnline::Review(char mesaj[1024], char raspuns[3000], char user[20])
{
    bzero(raspuns,3000);
    Carte *c = new Carte();
    char *p, isbn[14], rating[2];
    p = strtok(mesaj," ");
    strcpy(isbn,p);
    p = strtok(NULL," ");
    strcpy(rating,p);
    if(c->voteazaCarte(isbn,user,rating))
    {
        strcpy(raspuns,"    Cartea a fost votata.\n");
        return true;
    }
    else
    {
        strcpy(raspuns,"    Poti vota o singura data o carte descarcata.\n");
        return false;
    }

}
bool TranzactieOnline::Descarca(char mesaj[1024], char raspuns[3000], char fisier[30])
{
    bzero(raspuns,3000);
    Carte *c = new Carte();
    char isbn[14];
    strcpy(isbn,mesaj);
    strcpy(fisier,"fisier inexistent");
    if(c->existaCarte(isbn,fisier))
    {   
        strcpy(raspuns,"    Cartea a fost gasita. Incepe descarcarea.\n");
        return true;
    }
    else
    {
        strcpy(raspuns,"    Cartea nu a fost gasita.\n");
        return false;
    }
}
bool TranzactieOnline::Istoric(char mesaj[1024], char raspuns[3000], char user[20])
{
    bzero(raspuns,3000);
    Descarcari *d = new Descarcari();
    Cautari *c = new Cautari();
    char desc[3000];
    char caut[3000];
    d->istoricDescarcari(user,desc);
    c->istoricCautari(user,caut);
    strcpy(raspuns,caut);
    strcat(raspuns,desc);
    return true;
}
bool TranzactieOnline::Recomanda(char mesaj[1024], char raspuns[3000], char user[20])
{
    bzero(raspuns,3000);
    Client *c = new Client(user);
    Recomandare *r = new Recomandare();
    r->recomanda(c,raspuns);
    delete c;
    delete r;
    return true;
}

bool TranzactieOnline::DescarcareReusita(char mesaj[1024], char user[20])
{
    Descarcari *d = new Descarcari();
    char isbn[14];
    strcpy(isbn,mesaj);
    isbn[strlen(isbn)] = 0;
    d->adaugaDescarcare(user,isbn);
    Client *c = new Client(user);
    char pa1[20], pa2[20], pg1[20], pg2[20];
    printf("TranzactieOnline::descReusita:afisare noile preferinte ale userului.\n");
    c->selectPrefAut1(pa1);
    printf("%s\n",pa1);
    c->selectPrefAut2(pa2);
    printf("%s\n",pa2);
    c->selectPrefGen1(pg1);
    printf("%s\n",pg1);
    c->selectPrefGen2(pg2);
    printf("%s\n",pg2);
    c->adaugaPrefClient(pg1,pg2,pa1,pa2);
    delete d;
    delete c;
    return true;
}

bool TranzactieOnline::VerificaTip(char tip,char mesaj[1024], char raspuns[3000], char user[20], int tip_user, char fisier[30])
{
    bool tip_raspuns;
    /*if(tip == 'l')
        tip_raspuns = TranzactieOnline::Login(mesaj,raspuns);*/
    /*if(tip == 'u')
        tip_raspuns = TranzactieOnline::AdaugaUser(mesaj,raspuns);*/
    if(tip == 'a')
        tip_raspuns = TranzactieOnline::AdaugaCarte(mesaj,raspuns,fisier);
    if(tip == 's')
        tip_raspuns = TranzactieOnline::StergeCarte(mesaj,raspuns);
    if(tip == 'r')
        tip_raspuns = TranzactieOnline::Review(mesaj,raspuns,user);
    if(tip == 'd')
        tip_raspuns = TranzactieOnline::Descarca(mesaj,raspuns,fisier);
    if(tip == 'h')
        tip_raspuns = TranzactieOnline::Istoric(mesaj,raspuns,user);
    if(tip == 'p')
        tip_raspuns = TranzactieOnline::Recomanda(mesaj,raspuns,user);
    if(tip == 'c')
        tip_raspuns = TranzactieOnline::CautaCarte(mesaj,raspuns,user);
    return tip_raspuns;
}
