#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "game.h"
#include "player.h"

void applyDamages(Bomb* bomb, Player* playersTable, Crate* cratesTable, int nbCrates, char levelMap[TILES_Y][TILES_X], int l, int* continuer)
{
	SDL_Rect pos;
	//convertion de la position qui est en pixels
	pos.x = bomb->position.x/TILE_SIZE;
	pos.y = bomb->position.y/TILE_SIZE;
	
	int i, j;
	//on parcourt l'axe des x
	for(j=pos.x; j<=(pos.x)+l; j++)
	{
		//si on rencontre un obstable, on arrête la boucle
		if (levelMap[pos.y][j] == MAP_WALL_ID || levelMap[pos.y][j] == MAP_CRATE_ID)
			break; 

		//on parcourt le tableau de joueurs
		for(i=0; i<4; i++)
		{
			//si on rencontre le joueur, on lui retire une vie
			if ((playersTable[i].posx == j) && (playersTable[i].posy == pos.y))
			{
				playersTable[i].health--;
				checkPlayerHealth(playersTable, i, levelMap, continuer);
			}
		}
		
		//on vérifie qu'il n'y a pas de caisses destructibles
		if (levelMap[pos.y][j] == MAP_CRATE_D_ID)
		{
			levelMap[pos.y][j] = MAP_GROUND_ID;
			//il faut aussi retirer la caisse du tableau de caisses
			for(i=0; i<nbCrates; i++)
			{
				if ((cratesTable[i].posx == j) && (cratesTable[i].posy == pos.y)
					&& (cratesTable[i].isDestructible == 1))
					cratesTable[i].expStart = SDL_GetTicks();
			}
		}
	}
	for(j=pos.x-1; j>=(pos.x)-l; j--)
	{
		//si on rencontre un obstable, on arrête la boucle
		if (levelMap[pos.y][j] == MAP_WALL_ID || levelMap[pos.y][j] == MAP_CRATE_ID)
			break; 

		//on parcourt le tableau de joueurs
		for(i=0; i<4; i++)
		{
			//si on rencontre le joueur, on lui retire une vie
			if ((playersTable[i].posx == j) && (playersTable[i].posy == pos.y))
			{
				playersTable[i].health--;
				checkPlayerHealth(playersTable, i, levelMap, continuer);
			}
		}
		
		//on vérifie qu'il n'y a pas de caisses destructibles
		if (levelMap[pos.y][j] == MAP_CRATE_D_ID)
		{
			levelMap[pos.y][j] = MAP_GROUND_ID;
			//il faut aussi retirer la caisse du tableau de caisses
			for(i=0; i<nbCrates; i++)
			{
				if ((cratesTable[i].posx == j) && (cratesTable[i].posy == pos.y)
					&& (cratesTable[i].isDestructible == 1))
					cratesTable[i].expStart = SDL_GetTicks();
			}
		}
	}


	//on parcourt l'axe des y (même raisonnement)
	for(j=pos.y; j<=(pos.y)+l; j++)
	{
		//si on rencontre un obstable, on arrête la boucle
		if (levelMap[j][pos.x] == MAP_WALL_ID || levelMap[j][pos.x] == MAP_CRATE_ID)
			break; 

		//on a déjà enlevé une vie au joueur placé sur la bombe
		if (j!=pos.y)
		{
			for(i=0; i<4; i++)
			{
				//si on rencontre le joueur, on lui retire une vie
				if ((playersTable[i].posx == pos.x) && (playersTable[i].posy == j))
				{
					playersTable[i].health--;
					checkPlayerHealth(playersTable, i, levelMap, continuer);
				}
			}
		}
		
		if (levelMap[j][pos.x] == MAP_CRATE_D_ID)
		{
			levelMap[j][pos.x] = MAP_GROUND_ID;
			for(i=0; i<nbCrates; i++)
			{
				if ((cratesTable[i].posx == pos.x) && (cratesTable[i].posy == j)
					&& (cratesTable[i].isDestructible == 1))
					cratesTable[i].expStart = SDL_GetTicks();
			}
		}
	}
	for(j=pos.y-1; j>=(pos.y)-l; j--)
	{
		//si on rencontre un obstable, on arrête la boucle
		if (levelMap[j][pos.x] == MAP_WALL_ID || levelMap[j][pos.x] == MAP_CRATE_ID)
			break; 

		//on a déjà enlevé une vie au joueur placé sur la bombe
		if (j!=pos.y)
		{
			for(i=0; i<4; i++)
			{
				//si on rencontre le joueur, on lui retire une vie
				if ((playersTable[i].posx == pos.x) && (playersTable[i].posy == j))
				{
					playersTable[i].health--;
					checkPlayerHealth(playersTable, i, levelMap, continuer);
				}
			}
		}
		
		if (levelMap[j][pos.x] == MAP_CRATE_D_ID)
		{
			levelMap[j][pos.x] = MAP_GROUND_ID;
			for(i=0; i<nbCrates; i++)
			{
				if ((cratesTable[i].posx == pos.x) && (cratesTable[i].posy == j)
					&& (cratesTable[i].isDestructible == 1))
					cratesTable[i].expStart = SDL_GetTicks();
			}
		}
	}
	//on modifie damagesApplied pour ne pas appliquer les dégâts plusieurs fois
	bomb->damagesApplied = 1;
}



void checkPlayerHealth(Player *playersTable, int playerId, char levelMap[TILES_Y][TILES_X], int *continuer)
{
	if(playersTable[playerId].health == 0) // Si un joueur n'a plus de vie, il est éliminé
	{
		int i;
		playersTable[playerId].bombPress = 0;
		levelMap[playersTable[playerId].posy][playersTable[playerId].posx] = MAP_GROUND_ID; // Le joueur est rayé de la carte
		for(i = 0; i < 4; i++) // Les joueurs encore en vie gagnent un point
		{
			if(i != playerId && playersTable[i].health > 0)
				playersTable[i].score++;
		}
	}
	
	// Comptage du nombre de joueurs restant
	int nbPlayersAlive = 4;
	int i;
	for(i = 0; i < 4; i++)
	{
		if( playersTable[i].type == NO_PLAYER || playersTable[i].health == 0)
			nbPlayersAlive--;
	}
	if(nbPlayersAlive <= 1) // S'il ne reste plus qu'un joueur (voir moins), la partie est finie
		*continuer = 0;
}


void renderBomb(Player *playersTable, SDL_Surface* ecran, SDL_Surface* bombSprite, SDL_Surface* explSprite, char levelMap[TILES_Y][TILES_X], Crate* cratesTable, int nbCrates, int *continuer) 
{
	int i, t;
	SDL_Rect spriteTrim;
	spriteTrim.y = 0;
	spriteTrim.w = TILE_SIZE;
	spriteTrim.h = TILE_SIZE;
	
	for (i=0; i<4; i++)
	{
		if (playersTable[i].bomb.active == 1)
		{
			t = SDL_GetTicks();
			if (t - playersTable[i].bomb.timein < 3000)
			{
				//affichage de la bombe allumée
				if (((t - playersTable[i].bomb.timein) % 250) < 125)
					spriteTrim.x = 0;
					
				else
					spriteTrim.x = TILE_SIZE;
				
				SDL_BlitSurface(bombSprite, &spriteTrim, ecran, &playersTable[i].bomb.position);
			}
			else if (t - playersTable[i].bomb.timein < 3500) //affichage de l'explosion
			{	
				int j;
				spriteTrim.x = 0;
				SDL_BlitSurface(explSprite, &spriteTrim, ecran, &playersTable[i].bomb.position);
				SDL_Rect pos = playersTable[i].bomb.position;
				
				//on stock dans l la portée de la bombe
				int l = 2;
				//si le joueur a un bonus BOMB, la portée est plus grande
				if (playersTable[i].bonus == BOMB)
					l = 4;
					
				//application des dégâts de la bombe
				if (playersTable[i].bomb.damagesApplied == 0)
				{
					applyDamages(&playersTable[i].bomb, playersTable, cratesTable, nbCrates, levelMap, l, continuer);
				}
				
				//affichage de l'explosion pour une direction
				spriteTrim.x = 2*TILE_SIZE;
				pos.x -= TILE_SIZE;
				for (j=1; j<=l; j++)
				{
					//si on tombe sur un obstacle, on arrête l'affichage de l'explosion 
					//dans cette direction.
					if((levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_WALL_ID)
						|| (levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_CRATE_ID)
						|| (levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_BOMB_ID))
					{
						break;
					}
					
					//si on atteint la portée de la bombe, on charge une image différente
					if (j==l)
						spriteTrim.x = 6*TILE_SIZE;
					
					SDL_BlitSurface(explSprite, &spriteTrim, ecran, &pos);
					
					//on passe a la case suivante (à gauche)
					pos.x -= TILE_SIZE;
				}
				
				//affichage pour une autre direction
				spriteTrim.x = 2*TILE_SIZE;
				pos = playersTable[i].bomb.position;
				pos.x += TILE_SIZE;
				for (j=1; j<=l; j++)
				{
					if((levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_WALL_ID)
						|| (levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_CRATE_ID)
						|| (levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_BOMB_ID))
						break;
					
					if (j==l)
						spriteTrim.x = 5*TILE_SIZE;
					
					SDL_BlitSurface(explSprite, &spriteTrim, ecran, &pos);
					pos.x += TILE_SIZE;
				}
				
				//affichage 3eme direction
				spriteTrim.x = TILE_SIZE;
				pos = playersTable[i].bomb.position;
				pos.y += TILE_SIZE;
				for (j=1; j<=l; j++)
				{
					if((levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_WALL_ID)
						|| (levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_CRATE_ID)
						|| (levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_BOMB_ID))
							break;
					
					if (j==l)
						spriteTrim.x = 4*TILE_SIZE;
					
					SDL_BlitSurface(explSprite, &spriteTrim, ecran, &pos);
					pos.y += TILE_SIZE;
				}
				
				//affichage dernière direction
				spriteTrim.x = TILE_SIZE;
				pos = playersTable[i].bomb.position;
				pos.y -= TILE_SIZE;
				for (j=1; j<=l; j++)
				{
					if((levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_WALL_ID)
						|| (levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_CRATE_ID)
						|| (levelMap[(pos.y)/TILE_SIZE][((pos.x)/TILE_SIZE)] == MAP_BOMB_ID))
						break;
					
					if (j==l)
						spriteTrim.x = 3*TILE_SIZE;
					
					SDL_BlitSurface(explSprite, &spriteTrim, ecran, &pos);
					pos.y -= TILE_SIZE;
				}
				//on peut remarcher directement à l'endroit ou la bombe vient d'exploser
				levelMap[(playersTable[i].bomb.position.y)/TILE_SIZE]
					[(playersTable[i].bomb.position.x)/TILE_SIZE] = MAP_GROUND_ID;
			}
			else //la bombe a finit d'exploser
			{
				playersTable[i].bomb.active = 0;
			}
		}
	}
}


void putBomb(Player *playersTable, char levelMap[TILES_Y][TILES_X])
{
	int i;
	SDL_Rect target;
	for (i=0; i<4; i++)
	{	
		//on vérifie tout d'abord que le joueur veut poser une bombe, qu'il ne bouge pas
		//et qu'il n'a pas déjà posé une bombe
		if ((playersTable[i].bombPress == 1) && (playersTable[i].isMoving == 0)
			&& (playersTable[i].bomb.active != 1))
		{
			//on vérifie que rien ne se trouve devant le joueur
			target.x = playersTable[i].posx;
			target.y = playersTable[i].posy;
			//la cible du joueur se trouve en face de lui
			switch(playersTable[i].direction)
			{
				case 1: //FRONT
					target.y++;
					break;
				case 2: //BACK
					target.y--;
					break;
				case 3: //LEFT
					target.x--;
					break;
				case 4: //RIGHT
					target.x++;
					break;
				default:
					break;
			}
			
			if(levelMap[target.y][target.x] == MAP_GROUND_ID)
			{
				//on peut poser la bombe sur la map
				levelMap[target.y][target.x] = MAP_BOMB_ID;
				
				//il reste à modifier les données de la bombe du joueur
				playersTable[i].bomb.active = 1;
				playersTable[i].bomb.timein = SDL_GetTicks();
				playersTable[i].bomb.position.x = target.x * TILE_SIZE;
				playersTable[i].bomb.position.y = target.y * TILE_SIZE;
				playersTable[i].bomb.damagesApplied = 0;
			}
		}
	}
}


void printRemainingTime(SDL_Surface* ecran, TTF_Font* police, unsigned int startingTime, int* continuer)
{
	int time = startingTime + GAME_TIME - SDL_GetTicks(); // Calcul du temps restant
	if(time <= 0) // Si le temps est à 0 ou moins, la partie est fini
	{
		*continuer = 0;
		return;
	}
	SDL_Color couleurBlanche = {255, 255, 255};
	
	char intToString[4];
	char texteAEcrire[10] = "";
	
	// Concaténation des minutes, secondes et millisecondes dans une même chaine
	snprintf(intToString, 4, "%02d", (time/60000)%60);
	strcat(texteAEcrire, intToString);
	strcat(texteAEcrire, ":");
	snprintf(intToString, 4, "%02d", (time/1000)%60);
	strcat(texteAEcrire, intToString);
	strcat(texteAEcrire, ".");
	snprintf(intToString, 4, "%03d", time%1000);
	strcat(texteAEcrire, intToString);
	
	// définition des variables pour le texte et le fond
	SDL_Surface* texte = TTF_RenderText_Blended(police, texteAEcrire, couleurBlanche);
	SDL_Surface* fond = NULL;
	fond = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 28, 32, 0, 0, 0, 0);
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
	SDL_Rect positionTexte;
	positionTexte.x = 541;
	positionTexte.y = 7;
	SDL_Rect positionFond;
	positionFond.x = 536;
	positionFond.y = 4;
	
	// Affichage du fond et du texte et libération de la mémoire
	SDL_BlitSurface(fond, NULL, ecran, &positionFond);
	SDL_BlitSurface(texte, NULL, ecran, &positionTexte);
	SDL_FreeSurface(fond);
	SDL_FreeSurface(texte);
}


void newBonus(char levelMap[TILES_Y][TILES_X])
{
	//on commence par tirer un nombre aléatoire entre 0 et (NB_BONUS - 1)
	int x = rand_a(NB_BONUS);
	
	//on compte le nombre de cases se type sol
	int i, j, k = 0;
	for (i=0; i<TILES_Y; i++)
	{
		for (j=0; j<TILES_X; j++)
		{
			if (levelMap[i][j] == MAP_GROUND_ID)
				k++;
		}
	}
	
	//on vérifie que le nombre de place disponible n'est pas nul
	if (k <= 0)
		return;

	//on définit ensuite a quel endroit sur la carte le bonus va apparaître
	//on tire un nombre entre 0 et le nombre de cases 'sol'
	int y = rand_a(k);
	k = -1; //initialisation à -1 de la variable incrémentée jusqu'à y 
			//car on commence par l'incrémenter
	for (i=0; i<TILES_Y; i++)
	{
		for (j=0; j<TILES_X; j++)
		{
			if(levelMap[i][j] == MAP_GROUND_ID)
				k++;
			if (k == y) 
			{
				//on change la valeur dans levelMap
				switch(x)
				{
					case 0:
						levelMap[i][j] = MAP_SUPERMUSHROOM_ID;
						break;
					case 1:
						levelMap[i][j] = MAP_BONUS_BOMB_ID;
						break;
					case 2:
						levelMap[i][j] = MAP_INVINCIBLE_ID;
						break;
					default:
						break;
				}
				//on quitte la double boucle
				j = TILES_X;
				i = TILES_Y;
			}
		}
	}
}


void getBonus(char levelMap[TILES_Y][TILES_X], Player* player)
{
	//on regarde dans levelMap si à la nouvelle position
	//du joueur on trouve un bonus
	if ((levelMap[player->posy][player->posx] == MAP_SUPERMUSHROOM_ID)
		|| (levelMap[player->posy][player->posx] == MAP_BONUS_BOMB_ID)
		|| (levelMap[player->posy][player->posx] == MAP_INVINCIBLE_ID))
	{
		//initialisation du moment de l'aquisition du bonus
		player->bonusStart = SDL_GetTicks();
		//on modifie le type de bonus
		switch (levelMap[player->posy][player->posx])
		{
			case MAP_SUPERMUSHROOM_ID:
				player->bonus = SUPERMUSHROOM;
				break;
			case MAP_BONUS_BOMB_ID:
				player->bonus = BOMB;
				break;
			case MAP_INVINCIBLE_ID:
				player->bonus = INVINCIBLE;
				break;
			default:
				break;
		}
	}
	else
	{
		if (SDL_GetTicks() - player->bonusStart > BONUS_DURATION)
			player->bonus = NO_BONUS;
	}
}


void renderBonus(char levelMap[TILES_Y][TILES_X], SDL_Surface* ecran, SDL_Surface* bonusSprite)
{
	// Définition du carré pour le rognage de la spritesheet
	SDL_Rect spriteTrim;
	spriteTrim.y = 0;
	spriteTrim.w = TILE_SIZE;
	spriteTrim.h = TILE_SIZE;
	
	SDL_Rect position;
	
	int i, j;
	//on parcourt le tableau levelMap
	for (i=0; i<TILES_Y; i++)
	{
		for(j=0; j<TILES_X; j++)
		{
			position.x = j*TILE_SIZE; //on convertit la position en pixels
			position.y = i*TILE_SIZE;
			
			//si on rencontre un bonus
			if ((levelMap[i][j] == MAP_SUPERMUSHROOM_ID) 
			 || (levelMap[i][j] == MAP_BONUS_BOMB_ID)
			 || (levelMap[i][j] == MAP_INVINCIBLE_ID))
			{
				//mise à jour de spriteTrim.x
				if (levelMap[i][j] == MAP_SUPERMUSHROOM_ID)
					spriteTrim.x = 0;
				else if (levelMap[i][j] == MAP_BONUS_BOMB_ID)
					spriteTrim.x = TILE_SIZE;
				else if (levelMap[i][j] == MAP_INVINCIBLE_ID)
					spriteTrim.x = 2*TILE_SIZE;
					
				//affichage à l'écran à la position désirée
				SDL_BlitSurface(bonusSprite, &spriteTrim, ecran, &position);
			}
		}
	}
}


int rand_a(int n)
{
    int x;
    x = rand()%(n);
    return x;
}