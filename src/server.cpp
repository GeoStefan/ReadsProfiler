
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include "../include/BazaDate.h"
#include "../include/TranzactieOnline.h"

#define PORT 2909
#define LIMITA 4

extern sqlite3* db;

extern int errno;

static void *treat(void *);

typedef struct {
	pthread_t idThread; //id-ul thread-ului
	int thCount; //nr de conexiuni servite
}Thread;

Thread *threadsPool; //un array de structuri Thread

int sd; //descriptorul de socket de ascultare
int nthreads;//numarul de threaduri
int activeThreads = 0;
pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER;              

void raspunde(int cl,int idThread);

int main (int argc, char *argv[])
{
  struct sockaddr_in server;	 	
  void threadCreate(int);

    if(argc<2)
    {
        fprintf(stderr,"Eroare: Primul argument este numarul de fire de executie...");
        exit(1);
    }
    nthreads=atoi(argv[1]);
    if(nthreads <=0)
        {
            fprintf(stderr,"Eroare: Numar de fire invalid...");
        exit(1);
        }
        threadsPool = new Thread[nthreads*10];

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        {
        perror ("[server]Eroare la socket().\n");
        return errno;
        }

    int on = 1;
    setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    
    bzero (&server, sizeof (server));

        server.sin_family = AF_INET;	
        server.sin_addr.s_addr = htonl (INADDR_ANY);
        server.sin_port = htons (PORT);
    
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
        {
            perror ("[server]Eroare la bind().\n");
            return errno;
        }

    if (listen (sd, 2) == -1)
        {
            perror ("[server]Eroare la listen().\n");
            return errno;
        }

    BazaDate::deschidereBD();

    printf("Nr threaduri %d \n", nthreads); fflush(stdout);

    for(int i=0; i<nthreads;i++)
            threadCreate(i);

        
    /* servim in mod concurent clientii...folosind thread-uri */
    while(1)
        {
            if(activeThreads == nthreads)
            {
                printf("Se creeaza noi threaduri");
                for(int i=nthreads; i<nthreads*2;i++)
                    threadCreate(i);
                nthreads *= 2;
            }
            //printf ("[server]Asteptam la portul %d...\n",PORT);
            //pause();				
        }
    BazaDate::inchidereBD();  
};	

void threadCreate(int i)
{
	void *treat(void *);
	
	pthread_create(&threadsPool[i].idThread,NULL,&treat,(void*)i);
	return; /* threadul principal returneaza */
}

void *treat(void * arg)
{		
		int client;
		        
		struct sockaddr_in from; 
		bzero (&from, sizeof (from));
      	
        long long nr = reinterpret_cast<long long>(arg);
        int argi = static_cast<int>(nr);

		printf ("[thread]- %d - pornit...\n", (int) argi);fflush(stdout);
		
		while(1)
		{		
			socklen_t length = sizeof (from);

			pthread_mutex_lock(&mlock);
			if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
				{
	 			    perror ("[thread]Eroare la accept().\n");	  			
				}
			pthread_mutex_unlock(&mlock);

            activeThreads++;
			threadsPool[(int)argi].thCount++;

			raspunde(client,(int)argi); //procesarea cererii
			/* am terminat cu acest client, inchidem conexiunea */
			close (client);
            sleep(5);
            activeThreads--;
            if(activeThreads < nthreads/2 && nthreads > LIMITA)
            {
                nthreads--;
                break;
            }			
		}
        printf ("[thread]- %d - a terminat executia...\n", (int) argi);fflush(stdout);	
};


void raspunde(int cl,int idThread)
{
    char *mesaj = new char[1024];
    char *raspuns = new char[3000], *mesaj_partial = new char[3000];
    char tip, tip_raspuns;
    long lungime, lungime_raspuns;
    bool terminareClient  = 0;
    char user_name[20], fisier[30];
    int user_type =-1;
    while(!terminareClient)
    {
        if(read(cl,&lungime,sizeof(long)) <= 0)
            {
                perror ("Eroare la citire din socket.\n");
                terminareClient = 1;
                continue;
            }

        if(read(cl,&tip,sizeof(char)) <= 0)
        {
            perror ("Eroare la citire din socket.\n");
            terminareClient = 1;
            continue;
        }

        long rest;
        mesaj[0] = 0;
    
        if((rest = read(cl,mesaj,lungime)) <= 0)
        {
            perror ("Eroare la citire din socket.\n");
            terminareClient = 1;
            continue;
        }
        
        printf ("[Thread %d]Mesajul a fost receptionat...%ld%c%s\n",idThread, lungime,tip,mesaj);
        fflush(stdout);

        if(tip == 't')
        {
            terminareClient = 1;
            break;
        }
        else
            if(tip == 'u')
            {
                if(TranzactieOnline::AdaugaUser(mesaj,raspuns))
                    tip_raspuns= 'o';
                    else
                        tip_raspuns= 'e';
            }
            else
            if(tip != 'l' && user_type == -1)
            {
                strcpy(raspuns,"    Trebuie sa te loghezi mai intai.\n");
                tip_raspuns = 'e';
            }
            else
            {
                if(tip == 'l' && user_type == -1)
                {
                    if(TranzactieOnline::Login(mesaj,raspuns,user_name,user_type))
                        tip_raspuns= 'o';
                    else
                        tip_raspuns= 'e';
                }
                else
                {
                    if(tip == 'l' && user_type != -1)
                    {
                        strcpy(raspuns,"    Trebuie sa te deloghezi pentru a te loga din nou.\n");
                        tip_raspuns = 'e';
                    }
                }
                if((tip == 'a' || tip == 's') && user_type == 0)
                {
                    strcpy(raspuns,"    Nu poti poti adauga/sterge carti daca nu esti admin.\n");
                    tip_raspuns = 'e';
                }
                else
                {
                    if(tip == 'x')
                    {
                        user_type = -1;
                        strcpy(raspuns,"    Te-ai delogat cu succes.\n");
                        tip_raspuns = 'o';
                    }
                    if(tip != 'l' && tip != 'x')
                        if(TranzactieOnline::VerificaTip(tip,mesaj,raspuns,user_name,user_type,fisier) == false)
                            tip_raspuns= 'e';
                            else
                            tip_raspuns= 'o';
                }
            }
        
        lungime_raspuns = strlen(raspuns) +1;

        if(write(cl,&lungime_raspuns,sizeof(long)) < 0)
        {
            perror ("Eroare la scriere in socket.\n");
        }

        if(write(cl,&tip_raspuns,sizeof(char)) < 0)
        {
            perror ("Eroare la scriere in socket.\n");
        }

        if(write(cl,raspuns,lungime_raspuns) < 0)
        {
            perror ("Eroare la scriere in socket.\n");
        }

        if(tip == 'd' && tip_raspuns == 'o')
        {   
            long len;
            long long lungime_fisier, citit_nou = 0, citit_total =0, bufer_dim =4096;
            char bufer[4096];

            len = strlen(fisier) + 1;

            if(write(cl,&len,sizeof(long)) < 0)
            {
                perror ("Eroare la scriere in socket.\n");
            }

            if(write(cl,fisier,len) < 0)
            {
                perror ("Eroare la scriere in socket.\n");
            }

            printf("Sa trimis nume fisier\n");

            int fd;
            char locatie[100];
            strcpy(locatie,"Carti/");
            strcat(locatie,fisier);
            if((fd = open(locatie,O_RDONLY)) == -1)
    	        perror("eroare la deschidere fisier");
    
            lseek(fd,SEEK_SET,0);

            while((citit_nou = read(fd,bufer,sizeof(bufer)))>0)
            {
                write(cl,&citit_nou,sizeof(long long));
                write(cl,bufer,citit_nou);
            }
            close(fd);
            if(write(cl,&citit_nou,sizeof(long long)) >=0)
                TranzactieOnline::DescarcareReusita(mesaj,user_name);
                else
                {
                    perror ("Eroare la scriere in socket.\n");
                }
        }
        if(tip == 'a' && tip_raspuns == 'o')
        {
            char bufer[4098];
            long len;
            long long citit_nou = 1;
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
                read(cl,&citit_nou,sizeof(long long));
                if(citit_nou > 0)
                {
                    read(cl,bufer,citit_nou);
                    write(fd,bufer,citit_nou);
                }
                bzero(bufer,sizeof(bufer));
            }

            printf("Am primit fisierul: %s\n",fisier); 
            close(fd);
        }

    }

}
