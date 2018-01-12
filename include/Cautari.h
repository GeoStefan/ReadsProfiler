#ifndef CAUTARI_H
#define CAUTARI_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;

class Cautari
{
    public:
        Cautari();
        void adaugaCautare(char client[20], char isbn[14],char t[20], vector<string> autori, vector<string>genuri, vector<string>subgenuri, char a[5], char rating[2]);
        void istoricCautari(char client[20], char rezultat[2300]);
};

#endif