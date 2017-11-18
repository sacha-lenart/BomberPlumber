#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include "constants.h"
#include "player.h"
#define TRUE		1
#define FALSE		0
#define IA 		"IA"
#define PLAYERS		"Player"
#define NONE 		"None"
#define NBR_PLAYERS 	4
#define NBR_ARROWS 		8
#define MAP_NAME_LENGTH	100

//Structure pour afficher les fleches du menu 
typedef struct {
	SDL_Rect position;
	SDL_Surface image;
	int sizex,sizey;
	int value;
	int mouseOnClick;
}Arrows;

enum namePerso{
	MARIO=0,BOWSER=1,TOAD=2
};
//Structure pour le choix des fleches du joueur dans le menu
typedef struct {
	SDL_keysym down;
	SDL_keysym up;
	SDL_keysym left;
	SDL_keysym right;
	SDL_keysym bomb;
} PlayerBinding;
//Structure pour le choix des images et du type de joueur dans le menu
typedef struct {
	SDL_Rect position;
	SDL_Surface image;
	
	int perso;
	SDL_Rect positionPlayer;
	SDL_Surface imagePlayer;
	char *text;	

	SDL_Rect positionChoice;
	char touches[5][100];
	
	int choiceKeys;
	PlayerBinding bindings;
}ImagePlayer;
//Structure du choix du level de la map et du bouton valider 
typedef struct {
	SDL_Rect position;
	SDL_Surface image;
	int level;	
	SDL_Rect positionValider;
	SDL_Surface imageValider;
}ImageChoiceMap;

void recupererTouche(SDL_keysym *touche);//Obtenir une touche saisie au clavier


int modifierimage2(int positionSourisX,int positionSourisY, SDL_Rect position);//Savoir si la souris est situé entre deux positions 
int verificationTouche(SDL_keysym *touche,ImagePlayer *imagePlayer);//Verifie si la touche n'est pas utilisée par un autre joueur

void initArrows(Arrows *fleche , SDL_Surface *ecran);//Initialise la position et les images des fleches du menu
void initPersonnages(SDL_Surface **personnages,char ** persos,int nbrPersos, SDL_Surface* ecran);//Initialise les photos des joueurs dans le menu
void initImagePlayer(ImagePlayer *imagePlayer,  SDL_Surface *ecran,Arrows *fleche,SDL_Surface **personnages);//Initialise la position de l'image des joueurs ainsi que le type de joueur
void initialisationGame(Player *playersTable,ImagePlayer *imagePlayer, char levelMap[TILES_Y][TILES_X]); //Fait le lien entre le menu et le jeu en initialisant toutes les variables


void choosePlayer(ImagePlayer *imagePlayer, SDL_Surface *ecran,Arrows *fleche,SDL_Rect rognageArrow,int positionSourisX,int positionSourisY,SDL_Surface **personnages,int nbrPersos);//Permet de selectionner le joueur que l'on veut
void chooseTypePlayer(ImagePlayer *imagePlayer,int positionSourisX,int positionSourisY);//Permet de selectionner si l'on veut une IA , un joueur ou personne .
void chooseKeys(ImagePlayer *imagePlayer,int positionSourisX,int positionSourisY);//Permet de choisir ses touches 
int chooseLevelMap(SDL_Surface * ecran, Arrows *map,int positionSourisX,int positionSourisY,ImageChoiceMap choice,SDL_Rect rognageArrow,int nbrMap); //Permet de choisir le niveau de la map

void renderArrowsMap(SDL_Surface *ecran,Arrows *map,SDL_Rect rognageArrow,int positionSourisX,int positionSourisY);//Permet d'afficher les fleches du choix du level
void renderArrows(SDL_Surface *ecran,Arrows *fleche,SDL_Rect rognageArrow,int positionSourisX,int positionSourisY);//Permet d'afficher toutes les fleches sauf celles du menu
void renderAll(SDL_Surface *ecran,ImagePlayer *imagePlayer,SDL_Rect rognageArrow,int positionSourisX,int positionSourisY,SDL_Surface keysEmpty,SDL_Surface keys,SDL_Surface keysContour,SDL_Rect rognagePortrait);//Permet de tout afficher sauf les fleches et le choix du level
void renderLevelMap(SDL_Surface * ecran, Arrows *map,int positionSourisX,int positionSourisY,ImageChoiceMap choice,SDL_Rect rognageArrow);//Permet d'afficher le choix du level
void tri(SDL_Surface *classement,SDL_Surface *ecran,Player *playersTable,SDL_Surface *texte,TTF_Font* police,SDL_Rect positionTexteClassement,SDL_Rect positionClassement,SDL_Color couleur,char *str,SDL_Rect rognagePortrait);

char** loadPlayerList(int* mapCount);//Permet de charger la liste des joueurs a partir d'un fichier txt
char** loadMapList(int* playerCount);//Permet de charger le fichier des maps a l'aide d'un fichier txt


#endif
