#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "map.h"
#include "game.h"
#include "constants.h"


typedef struct t_playerBindings{
	SDLKey down;
	SDLKey up;
	SDLKey left;
	SDLKey right;
	SDLKey bomb;
} PlayerBindings;

typedef enum {NO_PLAYER, PLAYER, AI} PlayerType;

typedef enum {NO_BONUS, SUPERMUSHROOM, BOMB, INVINCIBLE} PlayerBonus;

typedef struct t_player{
	SDL_Surface sprite; // Spritesheet contenant les frames du joueur
	PlayerType type; // USER, AI ou NOPLAYER
	PlayerBindings bindings; // Raccourcis clavier
	
	int health; // Points de vie (manche en cours)
	int score; // Score toutes manches confondues
	
	int posx, posy; // Position x et y sur la grille
	float lagx, lagy; // Décalage x et y par rapport à la case
	Crate *crateDeplaced; // Caisse déplacée par le joueur
	
	int direction; // Direction actuelle du personnage: FRONT (1), BACK (2), LEFT (3), RIGHT (4)
	int directionPressed; // Direction voulue par l'utilisateur (0 si aucun appui)
	int bombPress; // Est ce que l'utilisateur veut poser une bombe
	Bomb bomb; //association d'une bombe par joueur
	unsigned int inputStart; // Début de l'appui sur une touche
	int isMoving; // Statut: IDLE (0), MVT (1)
	unsigned int mvtStart; // Début du mouvement (temps absolu)
	
	PlayerBonus bonus; // Type de bonus (NOBONUS par défaut)
	unsigned int bonusStart; // Début du bonus (temps absolu)
}Player;



void renderPlayer(Player *player, SDL_Surface *ecran);

void movePlayer(Player *player, char levelMap[TILES_Y][TILES_X], Crate* crates, int nbCrates);

void getPlayersInput(Player player[], SDL_Event *event, int* stop);

void setAIInput(Player playersTable[], int playerId, char levelMap[TILES_Y][TILES_X], Crate* crates, int nbCrates);

void renderBomb(Player *playersTable, SDL_Surface* ecran, SDL_Surface* bombSprite, SDL_Surface* explSprite, char levelMap[TILES_Y][TILES_X], Crate* cratesTable, int nbCrates, int* continuer);

void checkPlayerHealth(Player *playersTable, int playerId, char levelMap[TILES_Y][TILES_X], int *continuer);

void applyDamages(Bomb* bomb, Player* playersTable, Crate* cratesTable, int nbCrates, char levelMap[TILES_Y][TILES_X], int l, int *continuer);

void putBomb(Player *playersTable, char levelMap[TILES_Y][TILES_X]);

void printRemainingTime(SDL_Surface* ecran, TTF_Font* police, unsigned int startingTime, int* continuer);

void newBonus(char levelMap[TILES_Y][TILES_X]);

void getBonus(char levelMap[TILES_Y][TILES_X], Player* player);

void renderBonus(char levelMap[TILES_Y][TILES_X], SDL_Surface* ecran, SDL_Surface* bonusSprite);

#endif
