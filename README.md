# BomberPlumber

Lors du démarrage, une fenêtre de paramétrage de la partie s'ouvre. Sélectionnez pour chacuns des 4 personnages :
- le personnage utilisé grâce aux deux flèches.
- le type de joueur (IA, humain ou pas de joueur) en cliquant sur le type.
- les touches utilisées pour la partie, si le type de joueur est un humain. Pour cela, cliquez une fois sur les icônes des touches et entrez dans l'ordre les 5 touches que vous utiliserez pendant la partie. 

Une fois le paramétrage effectué, choisissez au milieu de l'écran la carte de la partie et cliquez sur "Valider".

Le but du jeu est d'infligez trois fois des dégâts à un adversaire pour le tuer en posant des bombes qui explosent au bout d'un certain temps. Vous pouvez déplacer certaines caisses et détruire l'autre type de caisse avec les bombes. Des bonus peuvent apparaitre pour vous aidez. Une partie dure 3 minute. 


# Compiler le code

Avec la console de commande MSYS:
En C + SDL: gcc "src/main.c" "src/map.c" "src/player.c" "src/game.c" "src/interface.c" -o main -Wall -lmingw32 -lSDLmain -lSDL -lSDL_ttf

Avec un terminal (LINUX) :
1ere option : 
	- entrer : gcc "src/main.c" "src/map.c" "src/player.c" "src/game.c" "src/interface.c" -o main -Wall -lSDLmain -lSDL -lSDL_ttf
	- cliquer sur l'executable "main" créé
2eme option (avec le makefile): 
	- entrer : make
	- cliquer sur l'executable "BomberPlumber" créé

# Tester le code

Récupérer le fichier BomberPlumber et cliquez dessus. 
