#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "game.h"
#include "constants.h"


int loadCrates(Crate* cratesTable, char levelMap[TILES_Y][TILES_X], int nbCrates);

int loadLevel(char levelMap[TILES_Y][TILES_X], char levelFile[FILE_MAX_LENGTH], int* nbCrates);

void renderLevel(char levelMap[TILES_Y][TILES_X], SDL_Surface* ecran, SDL_Surface* spritesLevel);

void renderWall(char levelMap[TILES_Y][TILES_X], SDL_Surface* ecran, SDL_Surface* spritesLevel);

void renderCrate(Crate* crate, SDL_Surface* crateSprite, SDL_Surface* ecran, char levelMap[TILES_Y][TILES_X]);


#endif
