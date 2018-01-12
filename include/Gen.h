#ifndef Gen_h
#define Gen_h

#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace std;

class Gen
{
    private:
        char nume[20];
        char gen_parinte[20];
        void adaugaCG(char isbn[14],char nume_g[20]);
    public:
        Gen(char n[20]);
        bool adaugaGen( char gen[20], char subgen[20],char isbn[14]);
        bool stergeGen(char n[20]);
};

#endif