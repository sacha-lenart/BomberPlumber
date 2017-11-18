#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "player.h"


void renderPlayer(Player *player, SDL_Surface *ecran)
{
	// Définition du carré pour la position du sprite
	SDL_Rect position;
	position.x = player->posx*TILE_SIZE + (int)player->lagx;
	position.y = player->posy*TILE_SIZE + (int)player->lagy + OBJECTS_OFFSET;

	// Définition du carré pour le rognage de la spritesheet
	SDL_Rect spriteTrim;
	spriteTrim.x = TILE_SIZE*2;
	spriteTrim.y = 0;
	spriteTrim.w = TILE_SIZE;
	spriteTrim.h = TILE_SIZE;
	// Voir characters/template.bmp pour plus d'informations
	
	// Définition du délai entre chaque frame de l'animation
	int animDelay = PLAYER_ANIM_DELAY;
	if(player->bonus == SUPERMUSHROOM)
		animDelay /= PLAYER_MVT_BONUS;
	
	// Déplacement du carré selon la direction du joueur
	switch(player->direction)
	{
		// Par défaut, le carré est positionné sur la frame IDLE FRONT
		case 2: // IDLE FRONT -> BACK
			spriteTrim.x += TILE_SIZE;
			break;
		case 3: // IDLE FRONT -> LEFT
			spriteTrim.y += TILE_SIZE;
			break;
		case 4: // IDLE FRONT -> RIGHT
			spriteTrim.x += TILE_SIZE;
			spriteTrim.y += TILE_SIZE;
			break;
	}
	
	// Déplacement du carré si le joueur est en mouvement
	if(player->isMoving == 1)
	{
		spriteTrim.y += TILE_SIZE*2; // IDLE x -> MVT1 x
		if((SDL_GetTicks() - player->mvtStart)%(animDelay*2) > animDelay)
			spriteTrim.x -= TILE_SIZE*2; // MVT1 x -> MVT2 x
	}
	
	// Affichage du joueur sur l'écran
	SDL_BlitSurface(&player->sprite, &spriteTrim, ecran, &position);
}


void movePlayer(Player *player, char levelMap[TILES_Y][TILES_X], Crate* crates, int nbCrates)
{
	// Si le joueur à eu un bonus de type SUPERMUSHROOM, sa vitesse est démultipliée
	float playerMvtSpeed = PLAYER_MVT_SPEED;
	if(player->bonus == SUPERMUSHROOM)
		playerMvtSpeed *= PLAYER_MVT_BONUS;
	int i;

	if(player->directionPressed == 0)
		player->isMoving = 0;

	if((int)player->lagx == 0 && (int)player->lagy == 0 && player->directionPressed != 0) // Si le joueur veut bouger et qu'il est immobile
	{
		if(player->directionPressed == 1) // Vers le bas
		{
			player->direction = 1;
			if(player->posy < TILES_Y)
			{
				if((levelMap[player->posy+1][player->posx] == MAP_GROUND_ID 
				|| (levelMap[player->posy+1][player->posx] == MAP_INVINCIBLE_ID)
				|| (levelMap[player->posy+1][player->posx] == MAP_SUPERMUSHROOM_ID)
				|| (levelMap[player->posy+1][player->posx] == MAP_BONUS_BOMB_ID)
				|| (levelMap[player->posy+1][player->posx] == MAP_CRATE_ID && levelMap[player->posy+2][player->posx] == MAP_GROUND_ID))
				&& SDL_GetTicks() > player->inputStart + MVT_INPUT_DELAY)
				{
					player->posy++; // Le joueur se déplace d'une case vers le bas
					player->lagy = -1 * TILE_SIZE; // Le joueur est décalé de la taille d'une case en pixels vers le haut
					// Sa position n'a pas changé lorsqu'on l'affiche, mais sa position est quand même virtuellement une case plus bas
					
					//On regarde s'il y a un bonus dans la nouvelle position
					getBonus(levelMap, player);
					
					// La case d'avant prends l'id du sol et la nouvelle prends l'id du joueur
					levelMap[player->posy][player->posx] = MAP_PLAYER_ID;
					levelMap[player->posy-1][player->posx] = MAP_GROUND_ID;
					
				}
			}
		}
		if(player->directionPressed == 2) // Vers le haut
		{
			if(player->posy > 0)
			{
				player->direction = 2;
				if((levelMap[player->posy-1][player->posx] == MAP_GROUND_ID
				|| (levelMap[player->posy-1][player->posx] == MAP_INVINCIBLE_ID)
				|| (levelMap[player->posy-1][player->posx] == MAP_SUPERMUSHROOM_ID)
				|| (levelMap[player->posy-1][player->posx] == MAP_BONUS_BOMB_ID)
				|| (levelMap[player->posy-1][player->posx] == MAP_CRATE_ID && levelMap[player->posy-2][player->posx] == MAP_GROUND_ID))
				&& SDL_GetTicks() > player->inputStart + MVT_INPUT_DELAY)
				{
					player->posy--;
					player->lagy = TILE_SIZE;
					
					getBonus(levelMap, player);
					
					levelMap[player->posy][player->posx] = MAP_PLAYER_ID;
					levelMap[player->posy+1][player->posx] = MAP_GROUND_ID;
				}
			}
		}
		if(player->directionPressed == 3) // Vers la gauche
		{
			if(player->posx > 0)
			{
				player->direction = 3;
				if((levelMap[player->posy][player->posx-1] == MAP_GROUND_ID
				|| (levelMap[player->posy][player->posx-1] == MAP_INVINCIBLE_ID)
				|| (levelMap[player->posy][player->posx-1] == MAP_SUPERMUSHROOM_ID)
				|| (levelMap[player->posy][player->posx-1] == MAP_BONUS_BOMB_ID)
				|| (levelMap[player->posy][player->posx-1] == MAP_CRATE_ID && levelMap[player->posy][player->posx-2] == MAP_GROUND_ID))
				&& SDL_GetTicks() > player->inputStart + MVT_INPUT_DELAY)
				{
					player->posx--;
					player->lagx = TILE_SIZE;
					
					getBonus(levelMap, player);
					
					levelMap[player->posy][player->posx] = MAP_PLAYER_ID;
					levelMap[player->posy][player->posx+1] = MAP_GROUND_ID;
				}
			}
		}
		if(player->directionPressed == 4) // Vers la droite
		{
			if(player->posx < TILES_X)
			{
				player->direction = 4;
				if((levelMap[player->posy][player->posx+1] == MAP_GROUND_ID
				|| (levelMap[player->posy][player->posx+1] == MAP_INVINCIBLE_ID)
				|| (levelMap[player->posy][player->posx+1] == MAP_SUPERMUSHROOM_ID)
				|| (levelMap[player->posy][player->posx+1] == MAP_BONUS_BOMB_ID)
				|| (levelMap[player->posy][player->posx+1] == MAP_CRATE_ID && levelMap[player->posy][player->posx+2] == MAP_GROUND_ID))
				&& SDL_GetTicks() > player->inputStart + MVT_INPUT_DELAY)
				{
					player->posx++;
					player->lagx = -1 * TILE_SIZE;
					
					getBonus(levelMap, player);
					
					levelMap[player->posy][player->posx] = MAP_PLAYER_ID;
					levelMap[player->posy][player->posx-1] = MAP_GROUND_ID;
				}
			}
		}
	}
	
	for(i  = 0; i < nbCrates; i++) // Déplacement d'une caisse devant le joueur s'il y en a une
	{
		if(crates[i].active == 1 && player->posx == crates[i].posx && player->posy == crates[i].posy)
		{
			if(player->direction == 1)
				crates[i].posy++;
			if(player->direction == 2)
				crates[i].posy--;
			if(player->direction == 3)
				crates[i].posx--;
			if(player->direction == 4)
				crates[i].posx++;
			
			// Les caisses prennent alors la même valeur de décalage que le joueur
			crates[i].lagx = player->lagx;
			crates[i].lagy = player->lagy;
			player->crateDeplaced = &crates[i];
			levelMap[crates[i].posy][crates[i].posx] = MAP_CRATE_ID;
		}
	}
	
	// Si le joueur est décalé sur x, il faut le faire bouger dans la direction qui annulera le décalage
	if((int)player->lagx != 0)
	{
		player->isMoving = 1;
		if((int)player->lagx> 0)
			player->lagx -= playerMvtSpeed;
		else
			player->lagx += playerMvtSpeed;
	}
	
	// De même pour la direction y
	if((int)player->lagy != 0)
	{
		player->isMoving = 1;
		if((int)player->lagy > 0)
			player->lagy -= playerMvtSpeed;
		else
			player->lagy += playerMvtSpeed;
	}
	
	// S'il y a un décalage sur x ou y et que le joueur déplace une caisse, alors la caisse prends le même décalage que le joueur
	if(player->crateDeplaced != NULL && player->crateDeplaced->active == 1 && 
	((int) player->crateDeplaced->lagx != 0 || (int) player->crateDeplaced->lagy != 0))
	{
		player->crateDeplaced->lagx = player->lagx;
		player->crateDeplaced->lagy = player->lagy;
	}
	
	// Si le joueur déplaçait une caisse mais que le décalage est terminé, on enlève le pointeur vers la caisse déplacée
	if(player->crateDeplaced != NULL && player->crateDeplaced->active == 1 && 
	(int) player->crateDeplaced->lagx == 0 && (int) player->crateDeplaced->lagy == 0)
		player->crateDeplaced = NULL;
	
	// Si le joueur commence à bouger, on initialise le temps de début de mouvement
	if(player->isMoving == 0)
		player->mvtStart = 0;
	else if(player->mvtStart == 0)
		player->mvtStart = SDL_GetTicks();
}


void getPlayersInput(Player player[], SDL_Event *event, int* stop)
{
	if(SDL_PollEvent(event) == 1) { // Si aucun nouvel évenement n'est arrivé, rien ne se passe
		int i;
		switch(event->type)
		{
			case SDL_QUIT: // Cas où on veut quitter la partie
				*stop = 0;
				break;
				
			case SDL_KEYDOWN: // Cas où il y a appuie sur une touche quelconque
				for(i = 0; i < 4; i++)
				{
					if(player[i].type == PLAYER && player[i].health != 0)
					{
						if(event->key.keysym.sym == player[i].bindings.down)
							player[i].directionPressed = 1;
						if(event->key.keysym.sym == player[i].bindings.up)
							player[i].directionPressed = 2;
						if(event->key.keysym.sym == player[i].bindings.left)
							player[i].directionPressed = 3;
						if(event->key.keysym.sym == player[i].bindings.right)
							player[i].directionPressed = 4;
						if(event->key.keysym.sym == player[i].bindings.bomb)
							player[i].bombPress = 1;
							
						if(player[i].directionPressed == player[i].direction)
							player[i].inputStart = 0;
						else
							player[i].inputStart = SDL_GetTicks();
					}
				}
				break;
				
			case SDL_KEYUP: // Cas où on relache une touche quelconque
				for(i = 0; i < 4; i++)
				{
					if(player[i].type == PLAYER && player[i].health != 0)
					{
						if(event->key.keysym.sym == player[i].bindings.down
						&& player[i].directionPressed == 1)
							player[i].directionPressed = 0;
						if(event->key.keysym.sym == player[i].bindings.up
						&& player[i].directionPressed == 2)
							player[i].directionPressed = 0;
						if(event->key.keysym.sym == player[i].bindings.left
						&& player[i].directionPressed == 3)
							player[i].directionPressed = 0;
						if(event->key.keysym.sym == player[i].bindings.right
						&& player[i].directionPressed == 4)
							player[i].directionPressed = 0;
						if(event->key.keysym.sym == player[i].bindings.bomb)
							player[i].bombPress = 0;
					}
				}
				break;
		}
	}
}


void setAIInput(Player playersTable[], int playerId, char levelMap[TILES_Y][TILES_X], Crate* crates, int nbCrates)
{
	// Si le joueur est en mouvement, il ne peut de toute façon rien faire d'autre
	if(playersTable[playerId].isMoving == 0)
	{
		Bomb* closestBomb;
		Player* closestPlayer;
		int minEntityDistance = 1000000000, entityDistanceX, entityDistanceY;
		int i;
		// On regarde quelle est la bombe la plus proche
		for(i = 0; i < 4; i++)
		{
			if(playersTable[i].type != NO_PLAYER && playersTable[i].bomb.active == 1)
			{
				entityDistanceX = playersTable[playerId].posx - playersTable[i].bomb.position.x/TILE_SIZE;
				entityDistanceY = playersTable[playerId].posy - playersTable[i].bomb.position.y/TILE_SIZE;
				if(entityDistanceX < 0)
					entityDistanceX *= -1;
				if(entityDistanceY < 0)
					entityDistanceY *= -1;
				if(entityDistanceX + entityDistanceY < minEntityDistance)
				{
					minEntityDistance = entityDistanceX + entityDistanceY;
					closestBomb = &playersTable[i].bomb;
				}
			}
		}
		// Si une bombe est trop proche, le joueur doit s'enfuire: il cherche un chemin lui permettant de s'éloigner le plus possible
		if(minEntityDistance < 4)
		{
			if((levelMap[playersTable[playerId].posy+1][playersTable[playerId].posx] == MAP_GROUND_ID || 
			(levelMap[playersTable[playerId].posy+1][playersTable[playerId].posx] == MAP_CRATE_ID && levelMap[playersTable[playerId].posy+2][playersTable[playerId].posx] == MAP_GROUND_ID)) 
			&& playersTable[playerId].posy - closestBomb->position.y/TILE_SIZE >= 0)
				playersTable[playerId].directionPressed = 1;
			if((levelMap[playersTable[playerId].posy-1][playersTable[playerId].posx] == MAP_GROUND_ID || 
			(levelMap[playersTable[playerId].posy-1][playersTable[playerId].posx] == MAP_CRATE_ID && levelMap[playersTable[playerId].posy-2][playersTable[playerId].posx] == MAP_GROUND_ID))
			&& playersTable[playerId].posy - closestBomb->position.y/TILE_SIZE <= 0)
				playersTable[playerId].directionPressed = 2;
			if((levelMap[playersTable[playerId].posy][playersTable[playerId].posx-1] == MAP_GROUND_ID || 
			(levelMap[playersTable[playerId].posy][playersTable[playerId].posx-1] == MAP_CRATE_ID && levelMap[playersTable[playerId].posy][playersTable[playerId].posx-2] == MAP_GROUND_ID)) 
			&& playersTable[playerId].posx - closestBomb->position.x/TILE_SIZE <= 0)
				playersTable[playerId].directionPressed = 3;
			if((levelMap[playersTable[playerId].posy][playersTable[playerId].posx+2] == MAP_GROUND_ID || 
			(levelMap[playersTable[playerId].posy][playersTable[playerId].posx+2] == MAP_CRATE_ID && levelMap[playersTable[playerId].posy+2][playersTable[playerId].posx+2] == MAP_GROUND_ID)) 
				&& playersTable[playerId].posx - closestBomb->position.x/TILE_SIZE >= 0)
				playersTable[playerId].directionPressed = 4;
			playersTable[playerId].bombPress = 0;
		}
		else
		{
			// Sinon, il regarde quel joueur est le plus proche
			minEntityDistance = 1000000000;
			for(i = 0; i < 4; i++)
			{
				if(playersTable[i].type != NO_PLAYER && i != playerId)
				{
					entityDistanceX = playersTable[playerId].posx - playersTable[i].posx;
					entityDistanceY = playersTable[playerId].posy - playersTable[i].posy;
					if(entityDistanceX < 0)
						entityDistanceX *= -1;
					if(entityDistanceY < 0)
						entityDistanceY *= -1;
					if(entityDistanceX + entityDistanceY < minEntityDistance)
					{
						minEntityDistance = entityDistanceX + entityDistanceY;
						closestPlayer = &playersTable[i];
					}
				}
			}
			// S'il y a un joueur à proximité, il pose une bombe
			if(minEntityDistance < 3)
			{
				if(playersTable[playerId].posy - closestPlayer->posy > 0)
					playersTable[playerId].direction = 2;
				else if(playersTable[playerId].posy - closestPlayer->posy < 0)
					playersTable[playerId].direction = 1;
				if(playersTable[playerId].posx - closestPlayer->posx < 0)
					playersTable[playerId].direction = 4;
				else if(playersTable[playerId].posx - closestPlayer->posx > 0)
					playersTable[playerId].direction = 3;
				playersTable[playerId].bombPress = 1;
			}
			else
			{
				// Sinon, il se déplace aléatoirement sur la map et pose des bombes aléatoirement
				if(rand()%AI_BOMB_CHANCE == 0) playersTable[playerId].bombPress = 1;
				else
					playersTable[playerId].directionPressed = (rand()%4)+1;
			}
		}
	}
	else
	{
		// Si le joueur bouge déjà, il n'appuie sur plus aucune touche
		playersTable[playerId].directionPressed = 0;
		playersTable[playerId].bombPress = 0;
	}
}
