#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "player.h"
#include "constants.h"
#include "map.h"


int loadCrates(Crate* cratesTable, char levelMap[TILES_Y][TILES_X], int nbCrates)
{
	int i, j, k=0; //on va parcourir le tableau levelMap
	
	for(i=0; i<TILES_Y; i++)
	{
		for(j=0; j<TILES_X; j++)
		{	
			if ((levelMap[i][j]== MAP_CRATE_ID) || (levelMap[i][j]== MAP_CRATE_D_ID)) 
			{
				cratesTable[k].posx = j;
				cratesTable[k].posy = i;
				cratesTable[k].lagx = 0;
				cratesTable[k].lagy = 0;
				cratesTable[k].active = 1;
				cratesTable[k].expStart = 0;
				if(levelMap[i][j] == MAP_CRATE_ID)
					cratesTable[k].isDestructible = 0;
				else
					cratesTable[k].isDestructible = 1;
				k++;
			}
		}
	}
	
	if (k != nbCrates)
		return 0;
	else
		return 1;
}


int loadLevel(char levelMap[TILES_Y][TILES_X], char levelFile[], int *nbCrates)
{
    FILE* file = NULL;
    char fileChar;
    int i = 0, j = 0;

    file = fopen(levelFile, "r"); //"r" indique une ouverture en lecture seule
    if (file == NULL)
        return 0;

    //on remplit le tableau levelMap avec les différents chiffres du fichier
    for (i = 0 ; i < TILES_Y; i++)
    {
        for (j = 0 ; j < TILES_X ; j++)
        {
			do
				fileChar = fgetc(file);
			while(fileChar == '\n');
			if(fileChar == EOF)
				return 0;
			levelMap[i][j] = fileChar;
			if(levelMap[i][j] == MAP_CRATE_ID || levelMap[i][j] == MAP_CRATE_D_ID)
				(*nbCrates)++;
        }
    }

    //on n'oublie pas de fermer le fichier ouvert
    fclose(file);
    return 1;
}


void renderLevel(char levelMap[TILES_Y][TILES_X], SDL_Surface* ecran, SDL_Surface* spritesLevel)
{
	SDL_Rect position;
	SDL_Rect spriteTrim;
	spriteTrim.w = TILE_SIZE;
	
	int i, j;
	
	//on blitte toute la map
	for (i=0; i<TILES_Y; i++)
	{
		for(j=0; j<TILES_X; j++)
		{
			position.x = j*TILE_SIZE; //on convertit la position en pixels
			position.y = i*TILE_SIZE;

			switch (levelMap[i][j])
			{
				case MAP_GROUND_ID: case MAP_PLAYER_ID: case MAP_CRATE_ID: 
				case MAP_CRATE_D_ID: case MAP_BOMB_ID: case MAP_BONUS_BOMB_ID: 
				case MAP_INVINCIBLE_ID: case MAP_SUPERMUSHROOM_ID:
					spriteTrim.x = TILE_SIZE;
					spriteTrim.h = TILE_SIZE;
					spriteTrim.y = 0;
					if (i-1 >= 0)
					{
						if (levelMap[i-1][j] == MAP_WALL_ID)
						{
							spriteTrim.h = TILE_SIZE/2;
							spriteTrim.y = TILE_SIZE/2;
							position.y = position.y + TILE_SIZE/2;
						}
					}
					SDL_BlitSurface(spritesLevel, &spriteTrim, ecran, &position);
					break;
				case MAP_WALL_ID:
					spriteTrim.x = 0;
					spriteTrim.y = 0;
					spriteTrim.h = TILE_SIZE + (TILE_SIZE/2);
					SDL_BlitSurface(spritesLevel, &spriteTrim, ecran, &position);
					break;
				default:
					break;
			}
		}
	}
	
}


void renderWall(char levelMap[TILES_Y][TILES_X], SDL_Surface* ecran, SDL_Surface* spritesLevel)
{
	SDL_Rect position;
	SDL_Rect spriteTrim;
	spriteTrim.x = 0;
	spriteTrim.y = 0;
	spriteTrim.h = TILE_SIZE;
	spriteTrim.w = TILE_SIZE;
	int i, j;
	
	//on blitte uniquement le haut des murs
	for (i=0; i<TILES_Y; i++)
	{
		for(j=0; j<TILES_X; j++)
		{
			position.x = j*TILE_SIZE; //on convertit la position en pixels
			position.y = i*TILE_SIZE;

			if (levelMap[i][j] == MAP_WALL_ID)
				SDL_BlitSurface(spritesLevel, &spriteTrim, ecran, &position);
		}
	}
	
}


void renderCrate(Crate* crate, SDL_Surface* crateSprite, SDL_Surface* ecran, char levelMap[TILES_Y][TILES_X])
{
	if(!crate->active){		
		return;
	}
	//Définition de la position de la caisse
	SDL_Rect position;
	position.x = crate->posx*TILE_SIZE + (int)crate->lagx;
	position.y = crate->posy*TILE_SIZE + (int)crate->lagy;
	
	// Définition du carré pour le rognage de la spritesheet
	SDL_Rect spriteTrim;
	spriteTrim.x = 0;
	spriteTrim.y = 0;
	spriteTrim.w = TILE_SIZE;
	spriteTrim.h = TILE_SIZE+(TILE_SIZE/2);

	//Defintion du délai
	if(crate->isDestructible){
		spriteTrim.x += TILE_SIZE;
		if(crate->expStart!=0){
			if((SDL_GetTicks()-crate->expStart)>2*CRATE_ANIM_DELAY)
			{
				crate->active=0;
				levelMap[crate->posy][crate->posx]=MAP_GROUND_ID;
				return;
			}
			else
			{
				spriteTrim.x += TILE_SIZE;
				if((SDL_GetTicks()-crate->expStart)>CRATE_ANIM_DELAY)
					spriteTrim.x += TILE_SIZE;
			}
		}
	}
	
	SDL_BlitSurface(crateSprite, &spriteTrim, ecran, &position);	
} 
