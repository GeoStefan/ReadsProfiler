#ifndef Recomandare_h
#define Recomandare_h

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Carte.h"
#include "Autor.h"
#include "Gen.h"
#include "Client.h"


using namespace std;

class Recomandare 
{
    public:
        Recomandare();
        void recomanda(Client *client, char rezultat[2000]);
    private:
        Client client;
};

#endif