# ReadsProfiler
Proiect Retele Calculatoare

Compliarea serverului se face cu ajutorul fisierului Makefile, executand comanda make.
Pentru a compila, trebuie sa aveti instalata libraria "sqlite3", pe care o puteti descarca de la adresa: "https://www.sqlite.org/download.html".
Executarea serverului se face cu ajutorul executabilului "server" si avand ca argument numarul de threaduri.
ex: ./server 3

Compilarea clientului se face din folderul ClientTCP cu ajutorul fisierului Makefile, executand comanda make.
Executarea clientului se face cu ajutorul executabilului "clientTCP" si avand ca argumente adresa serverului si numarul portului care este 2100.
ex: ./clientTCP 127.0.0.1 2100

Cartile descarcate de client se salveaza automat in ClientTCP/Carti.
Cartile adaugate pe server se salveaza automat in folderul Carti.
Daca doriti stergerea bazei de date actuale si creearea uneia noi cu aceeasi structura compilati in folderul src: "g++ CreareTabele.cpp -o CreareTabele -l sqlite3" si apoi executati: ./CreareTabele
