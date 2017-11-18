#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "interface.h"

void recupererTouche(SDL_keysym *touche)
{
	SDL_Event event;
	int continuer = TRUE;
	while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = FALSE;
                break;
			case SDL_KEYUP: // Lorsqu'on relache une touche
				// Copie de event.key.keysym dans touche (faut copier tout les paramètres 1 par 1)
				touche->scancode = event.key.keysym.scancode;
				touche->sym = event.key.keysym.sym;
				touche->mod = event.key.keysym.mod;
				touche->unicode = event.key.keysym.unicode;
				continuer = FALSE;
				break;
        }
    }
}
int modifierimage2(int positionSourisX,int positionSourisY, SDL_Rect position){
	/*Si la position de la souris est compris entre le debut de la structure en x et sa largeur ainsi que le debut de la structure en y et de sa hauteur alors on retourne 1 sinon 0 */	
	if(positionSourisX >= position.x && positionSourisX <= position.x + position.w && positionSourisY >= position.y && positionSourisY <= position.y + position.h){
		return 1; 
	}
	else{	
		return 0;
	}

}

int verificationTouche(SDL_keysym *touche,ImagePlayer *imagePlayer){
	int i ; 
	for(i=0;i<NBR_PLAYERS;i=i+1){
		if(touche->sym==imagePlayer[i].bindings.up.sym){/*Si la touche existe deja pour la touche du haut d'un joueur on retourne 1*/
			return 1;
		}
		if(touche->sym==imagePlayer[i].bindings.down.sym){/*Si la touche existe deja pour la touche du bas d'un joueur on retourne 1*/
			return 1;
		}
		if(touche->sym==imagePlayer[i].bindings.left.sym){/*Si la touche existe deja pour la touche de gauche d'un joueur on retourne 1*/		
			return 1;
		}
		if(touche->sym==imagePlayer[i].bindings.right.sym){/*Si la touche existe deja pour la touche de droite d'un joueur on retourne 1*/	
			return 1;
		}
		if(touche->sym==imagePlayer[i].bindings.bomb.sym){/*Si la touche existe deja pour la touche de la bombe d'un joueur on retourne 1*/	
			return 1;
		}
	}
	return 0 ;

}


void initArrows(Arrows *fleche , SDL_Surface *ecran){
	int i ;
	//Chargement des fleches
	fleche[0].image=*SDL_LoadBMP("interface/blueLeft.bmp");
	fleche[1].image=*SDL_LoadBMP("interface/blueRight.bmp");
	fleche[2].image=*SDL_LoadBMP("interface/yellowLeft.bmp");
	fleche[3].image=*SDL_LoadBMP("interface/yellowRight.bmp");
	fleche[4].image=*SDL_LoadBMP("interface/redLeft.bmp");
	fleche[5].image=*SDL_LoadBMP("interface/redRight.bmp");
	fleche[6].image=*SDL_LoadBMP("interface/greenLeft.bmp");
	fleche[7].image=*SDL_LoadBMP("interface/greenRight.bmp");
	
	//Transparence des fleches
	for(i=0; i<8; i++)
		SDL_SetColorKey(&fleche[i].image, SDL_SRCCOLORKEY, SDL_MapRGB(fleche[i].image.format, 255, 255, 255));	
	//Position des fleches
	fleche[0].position.x=10 ; 
	fleche[0].position.y=((ecran->h/5)+30) ;
	fleche[1].position.x=140+fleche[0].position.x ; 
	fleche[1].position.y=fleche[0].position.y;
	fleche[2].position.x=10 ;
	fleche[2].position.y=(ecran->h*7/10); 
	fleche[3].position.x=140+fleche[0].position.x ; 
	fleche[3].position.y=fleche[2].position.y;
	fleche[4].position.x=ecran->w-230; ; 
	fleche[4].position.y=(ecran->h/5)+30 ;
	fleche[5].position.x=140+fleche[4].position.x; 
	fleche[5].position.y=fleche[4].position.y;
	fleche[6].position.x=ecran->w-230; ;  
	fleche[6].position.y=(ecran->h*7/10) ;
	fleche[7].position.x=140+fleche[4].position.x ; 
	fleche[7].position.y=fleche[6].position.y;
	//Definition de la longueur et largeur des fleches
	for(i=0;i<NBR_ARROWS;i=i+1){
		fleche[i].position.w=47;
		fleche[i].position.h=34;
	}



}
void initImagePlayer(ImagePlayer *imagePlayer, SDL_Surface *ecran,Arrows *fleche,SDL_Surface **personnages){
   	//Definition de la position de l'image qui permet de choisir le type de joueur 
	int i,j=0;
	for(i=0;i<NBR_PLAYERS;i=i+1){
		imagePlayer[i].choiceKeys=0;
		imagePlayer[i].position.x = fleche[j].position.x+65;
    		imagePlayer[i].position.y = fleche[j].position.y-15;
    		imagePlayer[i].position.w = 96;
    		imagePlayer[i].position.h = 32;
		imagePlayer[i].positionPlayer.x = imagePlayer[i].position.x-16;
    		imagePlayer[i].positionPlayer.y =  imagePlayer[i].position.y+70;
		imagePlayer[i].positionChoice.x =  imagePlayer[i].positionPlayer.x-60;
    		imagePlayer[i].positionChoice.y =  imagePlayer[i].positionPlayer.y+40;
    		imagePlayer[i].positionChoice.w = 240;
    		imagePlayer[i].positionChoice.h = 48;
		j=j+2;
	}
	//Chargement des images qui permet de choisir le type de joueur 
	imagePlayer[0].imagePlayer=*(SDL_LoadBMP("interface/bluePlayer.bmp")); 	
	imagePlayer[1].imagePlayer =*(SDL_LoadBMP("interface/yellowPlayer.bmp")); 	
	imagePlayer[2].imagePlayer =*(SDL_LoadBMP("interface/redPlayer.bmp")); 	
	imagePlayer[3].imagePlayer =*(SDL_LoadBMP("interface/greenPlayer.bmp")); 
	for(i=0;i<4;i=i+1){
		SDL_SetColorKey(&imagePlayer[i].imagePlayer, SDL_SRCCOLORKEY, SDL_MapRGB(imagePlayer[i].imagePlayer.format, 255,255, 255));	
	}
	//Initialisation des joueur
	imagePlayer[0].perso=MARIO;	
	imagePlayer[1].perso =BOWSER; 
	imagePlayer[2].perso =TOAD; 
	imagePlayer[3].perso =MARIO ;
	//Allocation du texte definisant le type de joueur
	for(i=0;i<NBR_PLAYERS;i=i+1){
	imagePlayer[i].text= malloc (sizeof(char));
	}
	//Initialisation des types de joueur
	strcpy(imagePlayer[0].text,PLAYERS);
	strcpy(imagePlayer[1].text,NONE);
	strcpy(imagePlayer[2].text,NONE);
	strcpy(imagePlayer[3].text,NONE);
	//Initialisation des images de joueur
	for(i=0;i<NBR_PLAYERS;i=i+1){
		imagePlayer[i].image=*personnages[i] ; 	
	}
}
void initPersonnages(SDL_Surface **personnages,char ** persos,int nbrPersos, SDL_Surface* ecran){
	int i ;
	//Charge les images des joueurs 
	for(i=0;i<nbrPersos;i=i+1){
		personnages[i]=SDL_LoadBMP(persos[i]);
		personnages[i] = SDL_ConvertSurface(personnages[i], ecran->format, 0);
	}
}

void choosePlayer(ImagePlayer *imagePlayer, SDL_Surface *ecran,Arrows fleche[8],SDL_Rect rognageArrow,int positionSourisX,int positionSourisY,SDL_Surface **personnages,int nbrPersos){
	int i ;
	//Permet de selectionner le personnage que l'on veut 
	for(i=0;i<NBR_ARROWS;i=i+1){
		if(modifierimage2(positionSourisX,positionSourisY,fleche[i].position)==1){/*Si l'on clique sur une fleche on va changer le personnage*/
			rognageArrow.x = 0;
			rognageArrow.y = 0;
			SDL_BlitSurface(&fleche[i].image, &rognageArrow, ecran, &fleche[i].position);
			fleche[i].mouseOnClick=1;/*Fleche enfoncée */					
			if(i%2!=0)fleche[i].value=1;/*Fleche de droite*/
			else fleche[i].value=-1;/*Fleche de gauche*/					
			if(imagePlayer[i/2].perso==0&&fleche[i].value==-1){/*Si on est au premier joueur et qu'on veut le precedent*/
				imagePlayer[i/2].perso=nbrPersos-1;
			}
			else{					
				imagePlayer[i/2].perso=(imagePlayer[i/2].perso+fleche[i].value)%nbrPersos;			
			}
			imagePlayer[i/2].image=*personnages[imagePlayer[i/2].perso];

		}
		else{
			rognageArrow.x = 47;
			rognageArrow.y = 0;
			SDL_BlitSurface(&fleche[i].image, &rognageArrow, ecran, &fleche[i].position);/*Affichage du joueur*/
			fleche[i].mouseOnClick=0;
		}			
	}
}

void chooseTypePlayer(ImagePlayer *imagePlayer,int positionSourisX,int positionSourisY){
	int i ;	
	for(i=0;i<NBR_PLAYERS;i=i+1){
		if(modifierimage2(positionSourisX,positionSourisY,imagePlayer[i].positionPlayer)==1){
			if(!strcmp(imagePlayer[i].text,NONE))strcpy(imagePlayer[i].text,PLAYERS);/*Si on est un un joueur de type None on passe Player*/
			else if((!strcmp(imagePlayer[i].text,PLAYERS))&&(i!=0))strcpy(imagePlayer[i].text,IA);/*Si on est un joueur de type Player on passe IA*/		
			else if(!strcmp(imagePlayer[i].text,IA))strcpy(imagePlayer[i].text,NONE);/*Si on est un joueur de type IA on passe None*/
		}	
	}
}
void chooseKeys(ImagePlayer *imagePlayer,int positionSourisX,int positionSourisY){
	int i ;	
	SDL_keysym touche;
	for(i=0;i<NBR_PLAYERS;i=i+1){
		if(modifierimage2(positionSourisX,positionSourisY,imagePlayer[i].positionChoice)==1){
			if(strcmp(imagePlayer[i].text,NONE)&&strcmp(imagePlayer[i].text,IA)){/*Si il s'agit d'un joueur(qui peut donc avoir des touches */
				if(imagePlayer[i].choiceKeys!=1){/*Si les touches ont été saisies*/
					/*Une fois cliqué sur le bouton de choix des touches il faut choisir les touches une par une en verifiant qu'elle n'a pas deja été saisie par un autre joueur */
					do{	
						recupererTouche(&touche);
					}
					while(verificationTouche(&touche,imagePlayer)!=0);
		
					imagePlayer[i].bindings.left=touche;
				
					do{
						recupererTouche(&touche);
					}
					while(verificationTouche(&touche,imagePlayer)!=0);
					imagePlayer[i].bindings.up=touche;
					
					do{
						recupererTouche(&touche);
					}
					while(verificationTouche(&touche,imagePlayer)!=0);
					imagePlayer[i].bindings.right=touche;
					do{
						recupererTouche(&touche);
					}
					while(verificationTouche(&touche,imagePlayer)!=0);
					imagePlayer[i].bindings.down=touche;
					
					do{
						recupererTouche(&touche);
					}
					while(verificationTouche(&touche,imagePlayer)!=0);
					imagePlayer[i].bindings.bomb=touche;
					
					imagePlayer[i].choiceKeys=1;
				}
			}
		}
	}
}
int chooseLevelMap(SDL_Surface * ecran, Arrows *map,int positionSourisX,int positionSourisY,ImageChoiceMap choice,SDL_Rect rognageArrow,int nbrMap){
	/*Fonctionne a l'identique que la fonction qui permet le choix du personnage*/
	int i,resultat=choice.level ;
	for(i=0;i<2;i=i+1){	
		if(modifierimage2(positionSourisX,positionSourisY,map[i].position)==1){
			rognageArrow.x = 0;
			rognageArrow.y = 0;
			SDL_BlitSurface(&map[i].image, &rognageArrow, ecran, &map[i].position);
			if((choice.level==1)&&(map[i].value==-1)){
				resultat=nbrMap;
				i=2;
			}
			else if ((choice.level==nbrMap)&&(map[i].value==1)){
				resultat=1;
				i=2;
			}
			else{	
				resultat=resultat+map[i].value;
				i=2;
			}
	
		}
		else {
			resultat=choice.level;
		}	
	} 
return resultat;

}

void renderLevelMap(SDL_Surface * ecran, Arrows *map,int positionSourisX,int positionSourisY,ImageChoiceMap choice,SDL_Rect rognageArrow){	
	char str[10];
	SDL_Surface *texte=NULL;
	SDL_Rect positionLevelMap;
	
	//Definition des coordonnées de l'image qui affiche la zone
	positionLevelMap.x=choice.position.x+19;
	positionLevelMap.y=choice.position.y+12;

	//Ecriture du niveau sur l'image 
	TTF_Font *police = NULL;
	police = TTF_OpenFont("arial.ttf", 20);
	SDL_Color couleurBlanche = {255, 255, 255};
	sprintf(str, "%d", choice.level);	
	texte = TTF_RenderText_Blended(police,str, couleurBlanche);
	SDL_BlitSurface(texte, NULL, ecran, &positionLevelMap);
	SDL_FreeSurface(texte);
	TTF_CloseFont(police);
}
void renderArrows(SDL_Surface *ecran,Arrows *fleche,SDL_Rect rognageArrow,int positionSourisX,int positionSourisY){
	int i ; 	
	for(i=0;i<8;i=i+1){
		if(fleche[i].mouseOnClick==0){//Si la fleche n'est pas appuyée on affiche la fleche normale 
			if(modifierimage2(positionSourisX,positionSourisY,fleche[i].position)==1){//Si on passe la souris sur la fleche on affiche celle avec le contour 
				rognageArrow.x = 47;
				rognageArrow.y = 0;			
				SDL_BlitSurface(&fleche[i].image, &rognageArrow, ecran, &fleche[i].position);	
			}
			else{//Sinon la fleche normale 
				rognageArrow.x =0;
				rognageArrow.y =0;
				SDL_BlitSurface(&fleche[i].image, &rognageArrow, ecran, &fleche[i].position);
			}
		}
		else{//Sinon la fleche normale
			rognageArrow.x = 0;
			rognageArrow.y = 0;
			SDL_BlitSurface(&fleche[i].image, &rognageArrow, ecran, &fleche[i].position);
			fleche[i].mouseOnClick=0;
		}
							
	}
}
void renderArrowsMap(SDL_Surface *ecran,Arrows *map,SDL_Rect rognageArrow,int positionSourisX,int positionSourisY){
	//Fonction a l'identique que renderArrows	
	int i ;	
	for(i=0;i<2;i=i+1){
		if(map[i].mouseOnClick==0){
			if(modifierimage2(positionSourisX,positionSourisY,map[i].position)==1){
				rognageArrow.x = 47;
				rognageArrow.y = 0;			
				SDL_BlitSurface(&map[i].image, &rognageArrow, ecran, &map[i].position);	
				}
			else{
				rognageArrow.x =0;
				rognageArrow.y =0;
				SDL_BlitSurface(&map[i].image, &rognageArrow, ecran, &map[i].position);	
			}
		}
		else{
			rognageArrow.x = 0;
			rognageArrow.y = 0;
			SDL_BlitSurface(&map[i].image, &rognageArrow, ecran, &map[i].position);	
			map[i].mouseOnClick=0;
		}
	}

}

void renderAll(SDL_Surface *ecran,ImagePlayer *imagePlayer,SDL_Rect rognageArrow,int positionSourisX,int positionSourisY,SDL_Surface keysEmpty,SDL_Surface keys,SDL_Surface keysContour,SDL_Rect rognagePortrait){
	int i ;
	SDL_Color couleur={0,0,0};
	SDL_Rect positionKeys;
	TTF_Font *police = NULL,*police2 = NULL;
	SDL_Surface *texte=NULL;
	SDL_Rect decalage;
	police = TTF_OpenFont("arial.ttf", 20);
	SDL_Color couleurBlanche = {255, 255, 255};
	for(i=0;i<NBR_PLAYERS;i=i+1){
		//Affichage de l'image du personnage du joueur 		
		SDL_BlitSurface(&imagePlayer[i].image , &rognagePortrait, ecran, &imagePlayer[i].position);
		//Affichage de l'image de type de joueur 
		SDL_BlitSurface(&imagePlayer[i].imagePlayer , 0, ecran, &imagePlayer[i].positionPlayer);
		//Affichage de l'image de choix des touches du joueurs 		
		SDL_BlitSurface(&keys, 0, ecran, &imagePlayer[i].positionChoice);		
		
		texte=TTF_RenderText_Solid(police,imagePlayer[i].text,couleurBlanche);
		decalage.x=imagePlayer[i].positionPlayer.x+13;
		decalage.y=imagePlayer[i].positionPlayer.y+5;
		SDL_BlitSurface(texte, NULL, ecran,&decalage);
		if(imagePlayer[i].choiceKeys==1){//Si l'on a choisit les touches on va les afficher sur l'image des touches
			//Definition position des Touches 
			positionKeys.x=imagePlayer[i].positionChoice.x+10;
			positionKeys.y=imagePlayer[i].positionChoice.y+13;
			police2 = TTF_OpenFont("arial.ttf", 12);
			//Affichage des touches vides pour pouvoir les écrire			
			SDL_BlitSurface(&keysEmpty, NULL, ecran, &imagePlayer[i].positionChoice);
			//Affichage de la touche gauche
			texte = TTF_RenderText_Blended(police2, 	SDL_GetKeyName(imagePlayer[i].bindings.left.sym), couleur);
			SDL_BlitSurface(texte, NULL, ecran, &positionKeys);
			//Affichage de la touche du haut
			positionKeys.x+=48;
			texte = TTF_RenderText_Blended(police2, 	SDL_GetKeyName(imagePlayer[i].bindings.up.sym), couleur);
			SDL_BlitSurface(texte, NULL, ecran, &positionKeys);
			//Affichage de la touche de droite 
			positionKeys.x+=48;
			texte = TTF_RenderText_Blended(police2, 	SDL_GetKeyName(imagePlayer[i].bindings.right.sym), couleur);
			SDL_BlitSurface(texte, NULL, ecran, &positionKeys);
			//Affichage de la touche du bas
			positionKeys.x+=48;
			texte = TTF_RenderText_Blended(police2, 	SDL_GetKeyName(imagePlayer[i].bindings.down.sym), couleur);
			SDL_BlitSurface(texte, NULL, ecran, &positionKeys);
			//Affichage de la touche bombe
			positionKeys.x+=48;
			texte = TTF_RenderText_Blended(police2, 	SDL_GetKeyName(imagePlayer[i].bindings.bomb.sym), couleur);
			SDL_BlitSurface(texte, NULL, ecran, &positionKeys);
			TTF_CloseFont(police2);	
		}
		else{
			if(modifierimage2(positionSourisX,positionSourisY,imagePlayer[i].positionChoice)==1){/*Si l'on passe la souris sur le choix de touches cela affiche l'image avec contour*/
				if(strcmp(imagePlayer[i].text,NONE)&&strcmp(imagePlayer[i].text,IA)){
					SDL_BlitSurface(&keysContour, NULL, ecran, &imagePlayer[i].positionChoice);
				}
			}
			else{
				SDL_BlitSurface(&keys, NULL, ecran, &imagePlayer[i].positionChoice);
			
			}
		}
	SDL_FreeSurface(texte);

	}
TTF_CloseFont(police);

}

void tri(SDL_Surface *classement,SDL_Surface *ecran,Player *playersTable,SDL_Surface *texte,TTF_Font* police,SDL_Rect positionTexteClassement,SDL_Rect positionClassement,SDL_Color couleur,char *str,SDL_Rect rognagePortrait){
	int i, j, k;	
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 224,255,255));
	
	SDL_BlitSurface(classement, 0, ecran, &positionClassement);//On affiche le podium
	//Definition des positions des resultats	
	positionTexteClassement.x=positionClassement.x+20;
	positionTexteClassement.y=positionClassement.y+90;
	int joueurAffiche[4] = {0};
	Player* joueurPremier;
	for(i=0;i<4;i=i+1){
		joueurPremier = NULL;
		for(j=0;j<4;j++)//On fait le tri decroissant des scores des joueurs
		{
			if(joueurAffiche[j] == 0 && (joueurPremier == NULL || joueurPremier->score < playersTable[j].score))
			{
				joueurPremier = &playersTable[j];
				k = j;
			}
		}
		joueurAffiche[k] = 1;
		if(joueurPremier->type==AI||joueurPremier->type==PLAYER){/*On affiche le classement s'il ne s'agit pas d'un joueur de type NO_PLAYER*/
			positionTexteClassement.x=positionClassement.x+20;
			positionTexteClassement.y+=70;
			sprintf(str, "%d", i);
			texte = TTF_RenderText_Blended(police,"Joueur", couleur);
			//Affichage de "joueur"			
			SDL_BlitSurface(texte, 0, ecran, &positionTexteClassement);
			positionTexteClassement.x+=70;
			sprintf(str, "%d", k+1);
			//Affichage de sa position
			texte = TTF_RenderText_Blended(police,str, couleur);
			SDL_BlitSurface(texte, 0, ecran, &positionTexteClassement);
			positionTexteClassement.x+=35;
			positionTexteClassement.y-=25;	   
			//Affichage de son image			
			SDL_BlitSurface(&joueurPremier->sprite, &rognagePortrait, ecran, &positionTexteClassement);
			positionTexteClassement.x+=85;	
			positionTexteClassement.y+=25;	   
			sprintf(str, "%d", joueurPremier->score);			
			texte = TTF_RenderText_Blended(police,str, couleur);
			//Affichage de son score
			SDL_BlitSurface(texte, 0, ecran, &positionTexteClassement);

		}
	}
}

char** loadMapList(int* mapCount)
{
    FILE* file = NULL;
    int i = 0, j = 0;
    char currentChar;
    //Ouvre le fichier txt contenant les maps pour conter le nombre de lignes
    file = fopen("maps/mapslist.txt", "r");
    *mapCount = 0;
    i = 0;
    do {
		i++;
		currentChar = fgetc(file);
        if(currentChar == '\n') (*mapCount)++;
    } while(currentChar != EOF);
    
    char** mapList = (char**)malloc(sizeof(char*)*(*mapCount));
    for(i = 0; i < *mapCount; i++){
		mapList[i] = (char*)malloc(sizeof(char)*FILE_MAX_LENGTH);
	}

    fclose(file);
    //Ouvre le fichier txt contenant les maps pour lire les lignes
    file = fopen("maps/mapslist.txt", "r");

	for (i = 0 ; i < *mapCount; i++)
    {
		j = -1;
		do {
			j++;
			mapList[i][j] = fgetc(file);
		} while(mapList[i][j] != '\n');
		mapList[i][j] = '\0';
    }

    fclose(file);
    return mapList;
}

char** loadPlayerList(int* playerCount)
{
    FILE* file = NULL;
    int i = 0, j = 0;
    char currentChar;
    //Ouvre le fichier txt contenant les joueurs pour conter le nombre de lignes
    file = fopen("sprites/characterslist.txt", "r");
    *playerCount = 0;
    i = 0;
    do {
		i++;
		currentChar = fgetc(file);
        if(currentChar == '\n') (*playerCount)++;
    } while(currentChar != EOF);
    
    char** mapList = (char**)malloc(sizeof(char*)*(*playerCount));
    for(i = 0; i < *playerCount; i++){
		mapList[i] = (char*)malloc(sizeof(char)*FILE_MAX_LENGTH);
	}

    fclose(file);
    //Ouvre le fichier txt contenant les joueurs pour lire les lignes
    file = fopen("sprites/characterslist.txt", "r");

	for (i = 0 ; i < *playerCount; i++)
    {
		j = -1;
		do {
			j++;
			mapList[i][j] = fgetc(file);
		} while(mapList[i][j] != '\n');
		mapList[i][j] = '\0';
    }

    fclose(file);
    return mapList;
}

void initialisationGame(Player *playersTable,ImagePlayer *imagePlayer, char levelMap[TILES_Y][TILES_X]){
	int i, j, k = 4; 
	for(i=0;i<NBR_PLAYERS;i=i+1){
		//Liaisons entre les differentes structures 
		if(!strcmp(imagePlayer[i].text,NONE)){
			playersTable[i].type=NO_PLAYER;
		}
		if(!strcmp(imagePlayer[i].text,IA)){
			playersTable[i].type=AI;
		}
		if(!strcmp(imagePlayer[i].text,PLAYERS)){
			playersTable[i].type=PLAYER;
		}
			//Initialisation des diffents attributs de la structure du joueur  
			playersTable[i].direction = 1;//Direction dans la quelle il est tournée 
			playersTable[i].isMoving = 0; // Il ne bouge pas 
			if((playersTable[i].type==PLAYER)||(playersTable[i].type==AI))
				playersTable[i].health = 3;//Initialisation de son nombre de vie
			playersTable[i].bonus=NO_BONUS;//Pas de bonus 
			playersTable[i].directionPressed = 0;//Pas d'appuie sur un bouton
			playersTable[i].crateDeplaced = NULL;//Ne bouge pas de caisses 
			playersTable[i].bomb.active = 0;//N'a pas de bombe qui est activée
			playersTable[i].bombPress = 0;//N'a pas de bombe en train d'etre posée
			playersTable[i].inputStart = 0;//Pas de compte a rebours 
			if(playersTable[i].type==PLAYER)//Initialisation des touches
			{
				playersTable[i].bindings.down = imagePlayer[i].bindings.down.sym;
				playersTable[i].bindings.up = imagePlayer[i].bindings.up.sym;
				playersTable[i].bindings.left = imagePlayer[i].bindings.left.sym;
				playersTable[i].bindings.right = imagePlayer[i].bindings.right.sym;
				playersTable[i].bindings.bomb = imagePlayer[i].bindings.bomb.sym;
			}
			playersTable[i].sprite = imagePlayer[i].image;	//Chargement de son sprite
			SDL_SetColorKey(&playersTable[i].sprite, SDL_SRCCOLORKEY, SDL_MapRGB(playersTable[i].sprite.format, 255,0, 255));	//Transparence de son sprite
			playersTable[i].lagx = 0;
			playersTable[i].lagy = 0;
	}
	//Initialisation des positions des joueurs 
	for(i = 0; i < TILES_Y; i++)
	{
		for(j = 0; j < TILES_X; j++)
		{
			if(playersTable[k-1].type == NO_PLAYER)
				k--;
			if(levelMap[i][j] == MAP_PLAYER_ID)
			{
				if(k > 0)
				{
					k--;
					playersTable[k].posx = j;
					playersTable[k].posy = i;
				}
				else
					levelMap[i][j] = MAP_GROUND_ID;
			}
		}
	}
}
