#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

class TranzactieOnline
{
    public:
    
    static bool VerificaTip(char tip,char mesaj[1024], char raspuns[3000], char user[20], int tip_user, char fisier[30]);
    static bool Login(char mesaj[1024], char raspuns[3000], char user[20], int &tip_user);
    static bool AdaugaUser(char mesaj[1024], char raspuns[3000]);
    static bool AdaugaCarte(char mesaj[1024], char raspuns[3000], char fisier[30]);
    static bool CautaCarte(char mesaj[1024], char raspuns[3000], char user[20]);
    static bool StergeCarte(char mesaj[1024], char raspuns[3000]);
    static bool Review(char mesaj[1024], char raspuns[3000], char user[20]);
    static bool Descarca(char mesaj[1024], char raspuns[3000], char fisier[30]);
    static bool Istoric(char mesaj[1024], char raspuns[3000], char user[20]);
    static bool Recomanda(char mesaj[1024], char raspuns[3000], char user[20]);
    static bool DescarcareReusita(char mesaj[1024], char user[20]);
};