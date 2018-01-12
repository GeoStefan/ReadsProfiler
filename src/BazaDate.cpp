#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <string.h> 
#include <vector>
#include <string>
#include "../include/BazaDate.h"

#define DELIMITATOR ","
#define DELIMITATOR2 "  "

using namespace std;

sqlite3* db;

BazaDate::BazaDate()
{

}

void BazaDate::deschidereBD()
{
    int rc;
    rc = sqlite3_open("Librarie.db", &db);
    if( rc ) 
    {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } 
    else 
    {
      fprintf(stderr, "Opened database successfully\n");
    }
}

void BazaDate::inchidereBD()
{
    sqlite3_close(db);
}

int BazaDate::callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

bool BazaDate::inserare(char *sql)
{
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    else 
    {
        fprintf(stdout, "Insert successfully\n");
    }
    return true;
}

bool BazaDate::stergere(char *sql)
{
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    else 
    {
        fprintf(stdout, "Delete successfully\n");
    }
    return true;
}

bool BazaDate::update(char *sql)
{
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return false;
    }
    else 
    {
        fprintf(stdout, "Update successfully\n");
    }
    return true;
}

int BazaDate::cautareCarte(char *sql, char rezultat[2000])
{
    sqlite3_stmt *stmt;
    //char rezultat[1000];
    rezultat[0]='\0';
    int nr_carti = 0;
    printf("BazaDate::Cauta:\n%s\n",sql);

    char isbn[14], titlu[20], aux[20], isbn2[14];
    int an, rating;
    vector<string> autori;
    vector<string> genuri;
    vector<string> subgenuri;
    
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, NULL);
    if (rc != SQLITE_OK) 
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
    
    if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        strcpy(isbn,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        strcpy(titlu,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        strcpy(aux,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        autori.push_back(aux);
        strcpy(aux,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        genuri.push_back(aux);
        strcpy(aux,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        subgenuri.push_back(aux);
        an = sqlite3_column_int(stmt, 5);
        rating = sqlite3_column_int(stmt, 6);
        nr_carti++;
    }
    else
    {
        strcpy(rezultat,"   Cautarea nu a returnat niciun rezultat.\n");
        return nr_carti;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
        strcpy(isbn2,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        if(strcmp(isbn,isbn2) == 0)
        {
            strcpy(aux,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            if(strcmp(aux,autori.back().c_str()) != 0)
                autori.push_back(aux);
            strcpy(aux,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            if(strcmp(aux,genuri.back().c_str()) != 0)
                genuri.push_back(aux);
            strcpy(aux,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
            int ok = 1;
            for(vector<string>::iterator it = subgenuri.begin(); it != subgenuri.end();++it)
                if(strcmp(aux,(*it).c_str()) == 0)
                    ok = 0;
            if(ok)
                subgenuri.push_back(aux);
        }
        else
        {
            strcat(rezultat,isbn);
            strcat(rezultat,"  ");
            strcat(rezultat,titlu);
            strcat(rezultat,"  ");
            strcat(rezultat,autori.back().c_str());
            autori.pop_back();
            while(!autori.empty())
            {
                strcat(rezultat,DELIMITATOR);
                strcat(rezultat,autori.back().c_str());
                autori.pop_back();
            }
            strcat(rezultat,"  ");
            strcat(rezultat,genuri.back().c_str());
            genuri.pop_back();
            while(! genuri.empty())
            {
                strcat(rezultat,DELIMITATOR);
                strcat(rezultat, genuri.back().c_str());
                genuri.pop_back();
            }
            strcat(rezultat,"  ");
            strcat(rezultat,subgenuri.back().c_str());
            subgenuri.pop_back();
            while(! subgenuri.empty())
            {
                strcat(rezultat,DELIMITATOR);
                strcat(rezultat, subgenuri.back().c_str());
                subgenuri.pop_back();
            }
            strcat(rezultat,"  ");
            char number[33];
            sprintf(number,"%d",an);
            strcat(rezultat,number);
            strcat(rezultat,"  ");
            char year[33];
            sprintf(year,"%d",rating);
            strcat(rezultat,year);
            char terminator[3];
            terminator[0]='\n';
            terminator[1]='\0';
            strcat(rezultat,terminator);

            strcpy(isbn,isbn2);
            strcpy(titlu,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            strcpy(aux,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            autori.push_back(aux);
            strcpy(aux,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            genuri.push_back(aux);
            strcpy(aux,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
            subgenuri.push_back(aux);
            an = sqlite3_column_int(stmt, 5);
            rating = sqlite3_column_int(stmt, 6);
        }
        nr_carti++;
    }

    strcat(rezultat,isbn);
    strcat(rezultat,"  ");
    strcat(rezultat,titlu);
    strcat(rezultat,"  ");
    strcat(rezultat,autori.back().c_str());
    autori.pop_back();
    while(!autori.empty())
    {
        strcat(rezultat,DELIMITATOR);
        strcat(rezultat,autori.back().c_str());
        autori.pop_back();
    }
    strcat(rezultat,"  ");
    strcat(rezultat,genuri.back().c_str());
    genuri.pop_back();
    while(! genuri.empty())
    {
        strcat(rezultat,DELIMITATOR);
        strcat(rezultat, genuri.back().c_str());
        genuri.pop_back();
    }
    strcat(rezultat,"  ");
    strcat(rezultat,subgenuri.back().c_str());
    subgenuri.pop_back();
    while(! subgenuri.empty())
    {
        strcat(rezultat,DELIMITATOR);
        strcat(rezultat, subgenuri.back().c_str());
        subgenuri.pop_back();
    }
    strcat(rezultat,"  ");
    char number[33];
    sprintf(number,"%d",an);
    strcat(rezultat,number);
    strcat(rezultat,"  ");
    char year[33];
    sprintf(year,"%d",rating);
    strcat(rezultat,year);
    char terminator[3];
    terminator[0]='\n';
    terminator[1]='\0';
    strcat(rezultat,terminator);

    if (rc != SQLITE_DONE)
    {
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
        //sqlite3_finalize(stmt);
    }
    sqlite3_finalize(stmt);

    return nr_carti;
}


int BazaDate::selectRatingDesc(char *sql)
{
    sqlite3_stmt *stmt;
    int rez;
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, NULL);
    if (rc != SQLITE_OK) 
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
        rez = sqlite3_column_int(stmt, 0);
    }
    if (rc != SQLITE_DONE)
    {
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
        //sqlite3_finalize(stmt);
    }
    sqlite3_finalize(stmt);
    return rez;
}

void BazaDate::selectRatingCarte(int &rat_s, int &rat_n, char* sql)
{
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, NULL);
    if (rc != SQLITE_OK) 
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
        rat_s = sqlite3_column_int(stmt, 0);
        rat_n = sqlite3_column_int(stmt, 1);
    }
    if (rc != SQLITE_DONE)
    {
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
        //sqlite3_finalize(stmt);
    }
    sqlite3_finalize(stmt);
}

void BazaDate::selectGenClient(char* sql, char rezultat[20])
{
    sqlite3_stmt *stmt;
    char *gen =new char[20];
    int nr_rez = 0;
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, NULL);
    if (rc != SQLITE_OK) 
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
        strcpy(gen,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        nr_rez ++;
    }
    if (rc != SQLITE_DONE)
    {
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
    }
    sqlite3_finalize(stmt);
    if(nr_rez == 0)
        strcpy(gen,"ALL");
    strcpy(rezultat,gen);
    delete [] gen;
}

void BazaDate::selectAutClient(char* sql, char rezultat[20])
{
    sqlite3_stmt *stmt;
    char *aut =new char[20];
    int nr_rez = 0;
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, NULL);
    if (rc != SQLITE_OK) 
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
        strcpy(aut,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        nr_rez ++;
    }
    if (rc != SQLITE_DONE)
    {
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
    }
    sqlite3_finalize(stmt);
    if(nr_rez == 0)
        strcpy(aut,"ALL");
    strcpy(rezultat,aut);
    delete [] aut;
}

bool BazaDate::selectCautari(char* sql, char rezultat[2300])
{
    sqlite3_stmt *stmt;
    //char rezultat[2000];
    rezultat[0]='\0';
    int nr_rez = 0;
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, NULL);
   
    if (rc != SQLITE_OK) 
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
        strcat(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        strcat(rezultat,DELIMITATOR2);
        strcat(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        strcat(rezultat,DELIMITATOR2);
        strcat(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        strcat(rezultat,DELIMITATOR2);
        strcat(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        strcat(rezultat,DELIMITATOR2);
        strcat(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        strcat(rezultat,DELIMITATOR2);
        strcat(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
        strcat(rezultat,DELIMITATOR2);
        strcat(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
        strcat(rezultat,DELIMITATOR2);
        strcat(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
        strcat(rezultat,DELIMITATOR2);
        char terminator[3];
        terminator[0]='\n';
        terminator[1]='\0';
        strcat(rezultat,terminator);
        nr_rez ++;
    }
    if (rc != SQLITE_DONE)
    {
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
    }
    sqlite3_finalize(stmt);
    if(nr_rez == 0)
        strcpy(rezultat,"   Istoricul cautarilor este gol.\n");
    return true;
}

bool BazaDate::selectDescarcari(char *sql, char rezultat[300])
{
    sqlite3_stmt *stmt;
    //char rezultat[300];
    rezultat[0]='\0';
    int nr_rez = 0;
          
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, NULL);
    if (rc != SQLITE_OK) 
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
        strcat(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        strcat(rezultat,DELIMITATOR2);
        strcat(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        strcat(rezultat,DELIMITATOR2);
        char number[10];
        int x;
        x = sqlite3_column_int(stmt, 2);
        sprintf(number,"%d",x);
        if(x == 0)
            strcat(rezultat,"fara rating");
            else
            strcat(rezultat,number);
        char terminator[3];
        terminator[0]='\n';
        terminator[1]='\0';
        strcat(rezultat,terminator);
        nr_rez ++;
    }
    if (rc != SQLITE_DONE)
    {
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
    }
    sqlite3_finalize(stmt);
    if(nr_rez == 0)
        strcpy(rezultat,"   Istoricul descarcarilor este gol.\n");
    return true;
}

bool BazaDate::existaClient(char *sql)
{
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, NULL);
    if (rc != SQLITE_OK) 
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
    if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        return true;
        else
        return false; 
    if (rc != SQLITE_DONE)
    {
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
        //sqlite3_finalize(stmt);
    }
    sqlite3_finalize(stmt);
}

int BazaDate::selectClientType(char *sql)
{
    sqlite3_stmt *stmt;
    int tip;
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, NULL);
    if (rc != SQLITE_OK) 
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
    
    if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        tip = sqlite3_column_int(stmt,0);
    /*printf("%d\n",tip);
    fflush(stdout);*/
    /*if (rc != SQLITE_DONE)
    {
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }*/
    sqlite3_finalize(stmt);
    return true;
}

bool BazaDate:: selectFisCarte(char *sql, char rezultat[30])
{
    sqlite3_stmt *stmt;
    bool ok = 1;
    int rc = sqlite3_prepare_v2(db,sql,-1, &stmt, NULL);
    if (rc != SQLITE_OK) 
        {
            printf("SELECT failed: %s\n",sqlite3_errmsg(db));
            return false;
        }
    
    if((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        strcpy(rezultat,reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        else
        ok = 0;
    printf("BazaDate::fisier:\n%s\n",rezultat);
    fflush(stdout);

    /*if (rc != SQLITE_DONE)
    {
        printf("SELECT failed: %s\n",sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }*/
    sqlite3_finalize(stmt);
    return ok;
}