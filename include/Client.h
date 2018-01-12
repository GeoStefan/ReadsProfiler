#ifndef CLIENT_H
#define CLIENT_H

class Client 
{
    public:
        char nume[20];
        Client();
        Client(char *n);
        void setNume(char n[20]);
        bool adaugaClient(char nume[20], char parola[20], int admin);
        bool stergeClient(char nume[20]);
        void selectPrefGen1(char rezultat[20]);
        void selectPrefGen2(char rezultat[20]);
        void selectPrefAut1(char rezultat[20]);
        void selectPrefAut2(char rezultat[20]);
        void adaugaPrefClient(char gen1[20], char gen2[20], char aut1[20], char aut2[20]);
        void extragePreferinte(char gen1[20], char gen2[20], char aut1[20], char aut2[20]);
        void getPrefGen1(char rezultat[20]);
        void getPrefGen2(char rezultat[20]);
        void getPrefAut1(char rezultat[20]);
        void getPrefAut2(char rezultat[20]);
        bool verificaClient(char user[20], char pass[20]);
        int getUserType(char user[20]);
};
#endif

