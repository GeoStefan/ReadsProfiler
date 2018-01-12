#ifndef BAZADATE_H
#define BAZADATE_H

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h> 


class BazaDate
{
    public:
        BazaDate();
        static void deschidereBD();
        static void inchidereBD();
        static int callback(void *NotUsed, int argc, char **argv, char **azColName);
        bool inserare(char *sql);
        bool stergere(char *sql);
        int cautareCarte(char *sql, char rezultat[2000]);
        int selectRatingDesc(char *sql);
        bool update(char *sql);
        void selectRatingCarte(int &rat_s, int &rat_n, char* sql);
        void selectGenClient(char* sql, char rezultat[20]);
        void selectAutClient(char *sql, char rezultat[20]);
        bool selectCautari(char* sql, char rezultat[2300]);
        bool selectDescarcari(char *sql, char rezultat[300]);
        bool existaClient(char *sql);
        int selectClientType(char *sql);
        bool selectFisCarte(char *sql, char rezultat[30]);
};

#endif