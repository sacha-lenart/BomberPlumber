
CC = gcc

CFLAGS = -Wall #options du compilateur

INC_PATH = `sdl-config --libs --cflags` -lSDL_ttf 

OBJS = main.o map.o player.o game.o interface.o

LIB =  `sdl-config --libs --cflags` -lSDL_ttf 

PROG = BomberPlumber 


all : $(OBJS) 
	$(CC) $(INC_PATH) -o $(PROG) $(OBJS) $(LIB) 

main.o : ./src/main.c
	$(CC) $(CFLAGS)  -c ./src/main.c
map.o : ./src/map.c
	$(CC) $(CFLAGS) $(INC_PATH) -c $< 

game.o : ./src/game.c
	$(CC) $(CFLAGS) $(INC_PATH) -c $<

interface.o : ./src/interface.c
	$(CC) $(CFLAGS) $(INC_PATH) -c $<

player.o : ./src/player.c
	$(CC) $(CFLAGS) $(INC_PATH) -c $<

clean :
	-rm *.o *~ $(PROG) 
