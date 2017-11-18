#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "constants.h"


typedef struct t_crate{
	int posx, posy;
	float lagx, lagy;
	int isDestructible;
	int active;
	unsigned int expStart;
}Crate;

typedef struct t_bomb{
	SDL_Rect position; //position en pixels, PAS DE VALEUR NEGATIVE
	int timein; //temps d'entrée de la bombe, initialisé à -1
	int active; //permet de vérifier si le joueurs a déjà sa bombe de posée
	int damagesApplied; //indique si les dégâts causés ont été appliqués
}Bomb;


int rand_a(int n);


#endif
