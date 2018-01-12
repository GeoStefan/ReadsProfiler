#ifndef Autor_H
#define Autor_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;

class Autor
{
    private:
        char nume[20];
        int id_autor;
        int nr_carti;
        void adaugaCA(char isbn[14],int id);
        void adaugaAG(int id,vector<string>genuri);
    public:
        Autor(char n[20]);
        bool adaugaAutor(char n[20],int id,char isbn[14],vector<string>gen);
        bool stergeAutor(int id);
};

#endif