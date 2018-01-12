CC=g++
CFLAGS=-I. -l sqlite3 -l pthread
DEPS = include/Carte.h include/Autor.h include/Gen.h include/BazaDate.h include/Client.h include/Cautari.h include/Descarcari.h include/Recomandare.h include/TranzactieOnline.h
OBJ = src/server.o src/Carte.o src/Autor.o src/Gen.o src/BazaDate.o src/Client.o src/Cautari.o src/Descarcari.o src/Recomandare.o src/TranzactieOnline.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)
