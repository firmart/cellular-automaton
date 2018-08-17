CC=gcc    
CFLAGS=-O3 
LDFLAGS=-lSDL -lSDLmain
EXEC=CA
SRC=calc.c main.c sq_List.c 

all: main

main: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC) $(LDFLAGS)



clean:	
	rm -rf *.o $(EXEC)
