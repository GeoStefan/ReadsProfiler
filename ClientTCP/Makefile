CC=g++
CFLAGS=-I.
DEPS = ValidareComanda.h
OBJ = clientTCP.o ValidareComanda.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clientTCP: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)
