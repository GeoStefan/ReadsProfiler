#ifndef DESCARCARI_H
#define DESCARCARI_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

class Descarcari 
{
    public:
        Descarcari();
        void adaugaDescarcare(char client[20], char isbn[14]);
        int selectRating(char client[20], char isbn[14]);
        bool updateDescarcare(char client[20], char isbn[14], int rating);
        bool istoricDescarcari(char client[20], char rezultat[300]);
};

#endif