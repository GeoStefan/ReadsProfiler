#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include "ValidareComanda.h"

#define COM_N " Comanda necunoscuta."
#define DELIMIT_CAUTA "$"
extern int errno;
int port;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("executati <adresa_server> <port> \n");
        return -1;
    }
    
    int sd;
    struct sockaddr_in server;
    port = atoi(argv[2]);

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

    printf("Bun venit in aplicatie!\n");
    fflush(stdout);

    bool exit = 0;
    char comanda[1024], aux[100];
    char *mesaj = new char[1024];
    char *raspuns = new char[3000], *raspuns_partial = new char[3000];
    char tip, tip_raspuns;
    long lungime, lungime_raspuns;
    bool logat = false;
    while(!exit)
    {
        printf("\n  Introduceti numele comenzii.\n");
        fflush(stdout);
        if(fgets(comanda,1024,stdin) == NULL)
        {
            printf("Eroare la citirea comenzii.\n");
            fflush(stdout);
            return -1;
        }
        comanda[strlen(comanda)-1]='\0';
        if(validareTip(comanda) == false)
        {
            printf("%s\n",COM_N);
            continue;
        }
        tip = tipComanda(comanda);
        //printf("%c\n",tipComanda(comanda));
        if(tip == 'n')
        {
            printf("%s\n",COM_N);
            continue;
        }
        if(tip != 'l' && !logat && tip != 't')
        {
            printf("    Trebuie sa te loghezi mai intai.\n");
            continue;
        }
        
        switch(tip)
        {
            case 'l':
                if(vLogin(mesaj) == -1)
                    return -1;
                break;
            case 'c':
                if(vCautaCarte(mesaj) == -1)
                    return -1;
                break;
            case 'a':
                if(vAdaugaCarte(mesaj) == -1)
                    return -1;
                break;
            case 's':
                if(vStergeCarte(mesaj) == -1)
                    return -1;
                break;
            case 'u':
                if(vAdaugaUser(mesaj) == -1)
                    return -1;
                break;
            case 'r':
                if(vVoteaza(mesaj) == -1)
                    return -1;
                break;

            case 'p':
                strcpy(mesaj,"-");
                break;
            case 'h':
                strcpy(mesaj,"-");
                break;
            case 'x':
                strcpy(mesaj,"-");
                break;
            case 't':
                strcpy(mesaj,"-");
                break;
            case 'd':
                if(vDescarca(mesaj) == -1)
                    return -1;
                break;
        }
        //printf("%s",mesaj);

        lungime = strlen(mesaj) +1;

        if(write(sd,&lungime,sizeof(long)) < 0)
        {
            perror ("Eroare la scriere in socket.\n");
            return errno;
        }

        if(write(sd,&tip,sizeof(char)) < 0)
        {
            perror ("Eroare la scriere in socket.\n");
            return errno;
        }

        if(write(sd,mesaj,lungime) < 0)
        {
            perror ("Eroare la scriere in socket.\n");
            return errno;
        }

        if(tip == 't')
        {
            close(sd);
            exit = 1;
            break;
        }

        if(read(sd,&lungime_raspuns,sizeof(long)) <0)
        {
            perror ("Eroare la citire din socket.\n");
            return errno;
        }

        if(read(sd,&tip_raspuns,sizeof(char)) < 0)
        {
            perror ("Eroare la citire din socket.\n");
            return errno;
        }

        long rest;
        raspuns[0] = 0;
        if((rest = read(sd,raspuns,lungime_raspuns)) < 0)
            {
                perror ("Eroare la citire din socket.\n");
                return errno;
            }
        
        printf("%s",raspuns);

        if(tip == 'l' && tip_raspuns == 'o')
            logat = true;
        if(tip == 'x' && tip_raspuns == 'o')
            logat = false;

        if(tip == 'd' && tip_raspuns == 'o')
        {
            char fisier[30];
            long len;
            long long lungime_fisier, citit_nou = 1, citit_total =0, bufer_dim =4096; 

            if(read(sd,&len,sizeof(long)) <0)
            {
                perror ("Eroare la citire din socket.\n");
                return errno;
            }

            if((rest = read(sd,fisier,len)) < 0)
            {
                perror ("Eroare la citire din socket.\n");
                return errno;
            }

            char bufer[4098];
            int fd;
            char locatie[100];
            strcpy(locatie,"Carti/");
            strcat(locatie,fisier);
            creat(locatie, S_IRUSR | S_IWUSR);
            if((fd = open(locatie,O_WRONLY)) == -1)
    	        perror("eroare la deschidere fisier");
    
            lseek(fd,SEEK_SET,0);

            while(citit_nou > 0)
            {
                if(read(sd,&citit_nou,sizeof(long long)) < 0)
                {
                    perror ("Eroare la citire din socket.\n");
                    return errno;
                }
                if(citit_nou > 0)
                {
                    read(sd,bufer,citit_nou);
                    write(fd,bufer,citit_nou);
                }
                bzero(bufer,sizeof(bufer));
            }

            printf("Am primit fisierul: %s\n",fisier); 
            close(fd);
        }

        if(tip == 'a' && tip_raspuns == 'o')
        {
            char fisier[30];
            getFisier(mesaj,fisier);
            printf("fisier:%s\n",fisier);
            long long citit_nou = 0;
            char bufer[4096];

            int fd;
            char locatie[100];
            strcpy(locatie,"Carti/");
            strcat(locatie,fisier);
            if((fd = open(locatie,O_RDONLY)) == -1)
    	        perror("eroare la deschidere fisier");
    
            lseek(fd,SEEK_SET,0);

            while((citit_nou = read(fd,bufer,sizeof(bufer)))>0)
            {
                write(sd,&citit_nou,sizeof(long long));
                write(sd,bufer,citit_nou);
            }
            if(write(sd,&citit_nou,sizeof(long long)) >=0)
                printf("    S-a trimis cartea la server cu succes.\n");
            else
                printf("Eroare la trimiterea cartii.\n");
            close(fd);
        }
	
	printf("    S-a terminat procesarea comenzii.\n");

    }
    printf("    Aplicatia client se inchide. Reveniti cat mai curand!\n");
    return 0;
}
