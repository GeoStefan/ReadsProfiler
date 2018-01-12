#ifndef Carte_H
#define Carte_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include "Autor.h"
#include "Gen.h"

using namespace std;

class Carte
{
    private:
        char ISBN[14];
        char titlu[20];
        int an;
        int rating_s;
        int rating_n;
        int nr_descarcari;
        char fisier[30];
        Autor* autor;
        Gen* gen;
        void stergeCG(char isbn[14]);
        void stergePublicatie(char isbn[14]);
        void updateRatingCarte(char isbn[14],int rating);
    public:
        Carte();
        bool adaugaCarte(char isbn[14],char t[20], int a, vector<string> aut,vector<int>id_aut, char g[20], vector<string> subgen,char fis[30]);
        bool stergeCarte(char isbn[14], char fis[30]);
        bool voteazaCarte(char isbn[14],char client[20],char rating[2]);
        int cautaCarte(char isbn[14],char t[20], vector<string> autori, vector<string>genuri, vector<string>subgenuri, char a[5], char rating[2], char rezultat[2000]);
        bool existaCarte(char isbn[14],char fis[30]);
};

#endif