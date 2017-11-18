#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <time.h>

#include "constants.h"
#include "map.h"
#include "player.h"
#include "game.h"
#include "interface.h"


int main(int argc, char *argv[])
{
	/* Initialisation de la SDL et de SDL_ttf */
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *ecran = NULL;
    ecran = SDL_SetVideoMode(640, 640, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("Bomber Plumber", NULL);
	
	if(TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	TTF_Font* police = TTF_OpenFont("arial.ttf", 20);
	
	
	/* Déclaration des variables du main */
	char levelMap[TILES_Y][TILES_X]; // 
	int i, j; // Variables d'incréments

	int nbrPersos, nbrMap, nbCrates = 0; // Nombre de persos/maps/caisses
	char **persos, **maps; // Tableaux contenant la liste des persos/maps
	
	SDL_Color couleur={0,0,0}; // Couleur noire
	
	unsigned int startingTime; // Temps début d'une manche
	unsigned int ticksNumber; // Nombre d'images affichés au total pour une manche
	
	SDL_Event event; // Variable stockant les évenements de la SDL
	int continuer = 0; // Variable déterminant l'état du jeu
	int positionSourisX;
	int positionSourisY;
	
	// Variables pour l'affichage du menu
	Arrows fleche[NBR_ARROWS],map[2]; 
	ImagePlayer imagePlayer[NBR_PLAYERS];
	ImageChoiceMap choice ;
    SDL_Rect rognagePortrait,rognageArrow,wall,positionClassement,positionTexteClassement;
	char *str ;
	str=malloc(256*sizeof(char));
	
	// Variables de rognage des portraits des persos et des flèches
	rognagePortrait.w = 64;
	rognagePortrait.h = 64;
	rognagePortrait.x = 0;
	rognagePortrait.y = 0;
	rognageArrow.w = 47;
	rognageArrow.h = 34;
	rognageArrow.x = 0;
	rognageArrow.y = 0;
	wall.x=0;
	wall.y=0;
	
	Player playersTable[4]; // Tableau contenant les joueurs
	Crate* cratesTable; // Pointeur sur le tableau de caisses

	// Variables pour la position du bouton Valider
	choice.position.x=(ecran->w/2)-35;;
	choice.position.y=(ecran->h/2)+50;
	choice.level=1;
	choice.positionValider.x=choice.position.x-40;
	choice.positionValider.y=choice.position.y+60;

	// Variables pour le choix de la map
	map[0].position.x=choice.position.x-53;
	map[0].position.y=choice.position.y+8;
	map[0].mouseOnClick=0;
	map[0].value=-1;
	map[1].position.x=choice.position.x+53;
	map[1].position.y=map[0].position.y;
	map[1].mouseOnClick=0;
	map[1].value=1;
	
	
	/* Déclaration des surfaces à utiliser */
	SDL_Surface *classement = NULL;
	SDL_Surface *levelTiles = NULL, *cratesSprite = NULL, *bonusSprite = NULL;
	SDL_Surface *bombSprite = NULL, *explSprite = NULL;
	SDL_Surface *texte=NULL;
	SDL_Surface keysEmpty,keys,keysContour,*wallPaper;
	
	
	/* Chargement des images*/
	levelTiles = SDL_LoadBMP("level/map.bmp");
	cratesSprite = SDL_LoadBMP("level/crates.bmp");
	bombSprite = SDL_LoadBMP("level/bomb.bmp");
	explSprite = SDL_LoadBMP("level/explosion.bmp");
	bonusSprite = SDL_LoadBMP("level/bonus.bmp");
	classement = SDL_LoadBMP("interface/classement.bmp");
	wallPaper = SDL_LoadBMP("interface/title.bmp");
	keysEmpty=*(SDL_LoadBMP("interface/arrowsKeysEmpty.bmp"));
	keys=*(SDL_LoadBMP("interface/ArrowsKeys.bmp"));
	keysContour=*(SDL_LoadBMP("interface/ArrowsKeysContour.bmp"));
	choice.image=*(SDL_LoadBMP("interface/map.bmp"));
	choice.imageValider=*(SDL_LoadBMP("interface/valider.bmp"));
	map[0].image=*(SDL_LoadBMP("interface/blackLeft.bmp"));	
	map[1].image=*(SDL_LoadBMP("interface/blackRight.bmp"));
	
	/* CONVERSION 24 BITS EN 32 BITS DES LE DEBUT POUR AFFICHER SUR L'ECRAN SINON C'EST FAIT A CHAQUE BLIT ET SA RAME
	BORDEL POURQUOI C'EST PAS ECRIT SUR LA PLUPART DES TUTOS
	J'AI TROP LE SEUM AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAUHEGIUBHGDISFJVIEGFNJISDGRNDJGJNSDFVGNSDNGG
	DFNIVGNDSJGFNDKLJGNSKLJDVGFNKSJDFNK?SDNFKLSDNF?KLSDNFJIFNSNDF 
	JENFJFDKLSF?K?KZKOJOQIZJNJFDJQKC?ZQSLK2890U23JQZDJOQSDJOQSDJ */
	levelTiles = SDL_ConvertSurface(levelTiles, ecran->format, 0);
	cratesSprite = SDL_ConvertSurface(cratesSprite, ecran->format, 0);
	bombSprite = SDL_ConvertSurface(bombSprite, ecran->format, 0);
	explSprite = SDL_ConvertSurface(explSprite, ecran->format, 0);
	bonusSprite = SDL_ConvertSurface(bonusSprite, ecran->format, 0);
	
	/* Initialisations des variables de joueurs */
	persos=loadPlayerList(&nbrPersos);
	maps=loadMapList(&nbrMap);
	SDL_Surface *personnages[nbrPersos];	
	positionClassement.x=100;
	positionClassement.y=10;
	initArrows(fleche,ecran);
	initPersonnages(personnages,persos,nbrPersos, ecran);
	initImagePlayer(imagePlayer,ecran,fleche,personnages);
	
	
	/* Transparence des images */
	SDL_SetColorKey(bombSprite, SDL_SRCCOLORKEY, SDL_MapRGB(bombSprite->format, 255, 0, 255));
	SDL_SetColorKey(explSprite, SDL_SRCCOLORKEY, SDL_MapRGB(explSprite->format, 255, 0, 255));
	SDL_SetColorKey(cratesSprite, SDL_SRCCOLORKEY, SDL_MapRGB(cratesSprite->format, 255, 0, 255));
	SDL_SetColorKey(&keys, SDL_SRCCOLORKEY, SDL_MapRGB(keys.format, 255, 255, 255));	
	SDL_SetColorKey(&keysContour, SDL_SRCCOLORKEY, SDL_MapRGB(keysContour.format, 255, 255, 255));	
	SDL_SetColorKey(&keysEmpty, SDL_SRCCOLORKEY, SDL_MapRGB(keysEmpty.format, 255, 255, 255));
	SDL_SetColorKey(bonusSprite, SDL_SRCCOLORKEY, SDL_MapRGB(bonusSprite->format, 255, 0, 255));
	SDL_SetColorKey(&map[1].image, SDL_SRCCOLORKEY, SDL_MapRGB(map[1].image.format, 255, 255, 255));
	SDL_SetColorKey(&choice.imageValider, SDL_SRCCOLORKEY, SDL_MapRGB(choice.imageValider.format, 255, 255, 255));
	SDL_SetColorKey(&map[0].image, SDL_SRCCOLORKEY, SDL_MapRGB(map[0].image.format, 255, 255, 255));
	SDL_SetColorKey(&choice.image, SDL_SRCCOLORKEY, SDL_MapRGB(choice.image.format, 255, 255, 255));
	SDL_SetColorKey(classement, SDL_SRCCOLORKEY, SDL_MapRGB(classement->format, 255, 255, 255));
	SDL_SetColorKey(wallPaper, SDL_SRCCOLORKEY, SDL_MapRGB(wallPaper->format, 255, 0, 255));
	
	
	/* Initialisation des variables importantes */
	for(i = 0; i < 4; i++)
		playersTable[i].score = 0;
	
	srand(time(NULL));
	
	while(continuer != -1){
		/* Menu de séléction des persos/maps */
		positionSourisX = 0;
		positionSourisY = 0;
		while (continuer == 0)
		{
			// Récupération des évenements souris
			if(SDL_PollEvent(&event)){
				switch(event.type)
				{
					case SDL_QUIT:
						continuer = -1;
					break;
					case SDL_MOUSEMOTION:
						positionSourisX = event.motion.x;
						positionSourisY = event.motion.y;
					break;
					case SDL_MOUSEBUTTONUP:
						choosePlayer(imagePlayer,ecran,fleche,rognageArrow,positionSourisX,positionSourisY,personnages,nbrPersos);
						chooseTypePlayer(imagePlayer,positionSourisX,positionSourisY);
						chooseKeys(imagePlayer,positionSourisX,positionSourisY);
						choice.level=chooseLevelMap(ecran,map,positionSourisX,positionSourisY,choice,rognageArrow,nbrMap);
					if(modifierimage2(positionSourisX,positionSourisY, choice.positionValider)==1){
						continuer=1;
						for(i=0;i<4;i=i+1){
							if(imagePlayer[i].choiceKeys!=1&&(strcmp(imagePlayer[i].text,PLAYERS)==0))
								continuer = 0;
						}
				}
				break;	
			}
		}
		
		// Affichage des images
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 224,255,255));
		SDL_BlitSurface(&choice.image, 0, ecran, &choice.position);
		renderArrows(ecran,fleche,rognageArrow,positionSourisX,positionSourisY);
		renderArrowsMap(ecran,map,rognageArrow,positionSourisX,positionSourisY);
		renderAll(ecran,imagePlayer,rognageArrow,positionSourisX,positionSourisY,keysEmpty,keys,keysContour,rognagePortrait);
		renderLevelMap(ecran,map,positionSourisX,positionSourisY,choice,rognageArrow);
		SDL_BlitSurface(wallPaper, 0, ecran, &wall);
		SDL_BlitSurface(&choice.imageValider, 0, ecran, &choice.positionValider);
		SDL_Flip(ecran);
		}	
		

		/* Initialisation du niveau, des caisses, des variables des joueurs et des variables de jeu */
		nbCrates = 0;
		if(loadLevel(levelMap, maps[choice.level-1], &nbCrates) != 1)
		{
			printf("Failed to load level file.\n");
			return EXIT_FAILURE;
		}
		cratesTable=(Crate*) malloc(nbCrates*sizeof(Crate));
		if(loadCrates(cratesTable, levelMap, nbCrates) != 1)
		{
			printf("Failed to load level crates.\n");
			return EXIT_FAILURE;
		}
		initialisationGame(playersTable, imagePlayer, levelMap);
		startingTime = SDL_GetTicks();
		int bonusTime = 0;
		ticksNumber = SDL_GetTicks()/SCREEN_FRAMERATE;
		
		/* Boucle principale */
		while (continuer==1)
		{
			getPlayersInput(playersTable, &event, &continuer); // Récupération des touches des joueurs humains
			for(i = 0; i < 4; i++)
			{
				if(playersTable[i].type == AI && playersTable[i].health > 0)
					setAIInput(playersTable, i, levelMap, cratesTable, nbCrates); // Assignation des touches pour l'AI
				if(playersTable[i].type != NO_PLAYER && playersTable[i].health > 0)
					movePlayer(&playersTable[i], levelMap, cratesTable, nbCrates); // Déplacements des joueurs
			}
			putBomb(playersTable, levelMap); // Pose des bombes pour les joueurs voulant et pouvant en poser
			
			bonusTime += SCREEN_FRAMERATE; // Incrémentation du timer du nouveau bonus
			if (bonusTime >= NEW_BONUS)
			{
				newBonus(levelMap); // Spawn un bonus sur la map
				bonusTime = 0; // Réinitialise le timer du nouveau bonus
			}
			
			if(SDL_GetTicks()/SCREEN_FRAMERATE < ticksNumber + 1) // On affiche rien si le joueur a eu un microfreeze: l'execution doit rattraper avec plusieurs boucles sans affichage
			{
				SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); // Effacement de l'écran
				
				renderLevel(levelMap, ecran, levelTiles); // Affichage du sol
				
				for(i = 0; i < TILES_Y; i++)
				{
					for(j = 0; j < 4; j++)
					{
						if(playersTable[j].type != NO_PLAYER && playersTable[j].health > 0 && playersTable[j].posy == i)
							renderPlayer(&playersTable[j], ecran); // Affichage d'un joueur
					}
					for(j = 0; j < nbCrates; j++)
					{
						if(cratesTable[j].active == 1 && cratesTable[j].posy == i)
							renderCrate(&cratesTable[j], cratesSprite, ecran, levelMap); // Affichage d'une caisse
					}
				}
				
				renderBonus(levelMap, ecran, bonusSprite); // Affichage des bonus
				renderBomb(playersTable, ecran, bombSprite, explSprite, levelMap, cratesTable, nbCrates, &continuer); // Affichage des bombes
				renderWall(levelMap, ecran, levelTiles); // Affichage des murs
				printRemainingTime(ecran, police, startingTime, &continuer); // Affichage du temps restant avant la fin de la partie
				
				SDL_Flip(ecran); // Rafraichissement de l'écran
			}
			
			if(SDL_GetTicks()/SCREEN_FRAMERATE < ticksNumber + 1) // Si cette boucle à pris moins de SCREEN_FRAMERATE ms a s'executer, on attends pour compenser
				SDL_Delay((ticksNumber+1)*SCREEN_FRAMERATE - SDL_GetTicks());
			ticksNumber++;
		}
		
		/* Affichage des scores en fin de partie */
		if(continuer == 0)
		{
			free(cratesTable);
			cratesTable = NULL;
			tri(classement,ecran,playersTable,texte,police,positionTexteClassement,positionClassement,couleur,str,rognagePortrait);
			SDL_Flip(ecran);
			SDL_Delay(DURATION_CLASSEMENT);
		}
		
	}
	
	
	/* Libération de la mémoire allouée dynamiquement */
	SDL_FreeSurface(classement);
	SDL_FreeSurface(levelTiles);
	SDL_FreeSurface(cratesSprite);
	SDL_FreeSurface(bonusSprite);
	SDL_FreeSurface(bombSprite);
	SDL_FreeSurface(explSprite);
	SDL_FreeSurface(texte);
	SDL_FreeSurface(&keysEmpty);
	SDL_FreeSurface(&keys);
	SDL_FreeSurface(&keysContour);
	SDL_FreeSurface(wallPaper);
	SDL_FreeSurface(levelTiles);
	TTF_CloseFont(police);
    SDL_Quit();

    return 0;
}
