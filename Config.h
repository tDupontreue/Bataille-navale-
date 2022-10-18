#pragma once
#include <string.h>
#include <fstream>

const int SHIP_LENGTH_MAX = 5; // TAILLE MAX DES BATEAUX
const int SIZE_OF_PG = 10; // TAILLE DE LA GRILLE DE JEUX
const int KINDS_OF_SHIPS[5] = { 1,1,2,2,0 }; // 1 porte avion de 5 de long, 1 cuirassé de 4 de long, 2 croiseur de 3 de long, 2 petit navire de 2 de long.
const int NUMBER_OF_SHIPS = 6; //  nombre total des bateaux

const int NUMBER_OF_KINDS = sizeof(KINDS_OF_SHIPS) / sizeof(int); // NOMBRE DE SORTE DE BATEAUX
const bool SHIP_VISIBILITY = false; // visibilité des bateaux enemies sur la grille de droite

namespace cfg
{
	const int WINDOW_WIDTH = 1000;  // largeur de la fenetre du menu en px
	const int WINDOW_HEIGHT = 600;  // hauteur de la fenetre de menu en px
	const bool my_ship_visibility = false; // visibilité de ses propre bateaux sur la grille de gauche
	const int game_window_width = 1000; // largeur de la fenetre de jeux en px
	const int game_window_height = 600; // hauteur de la fenetre de jeux en px

	const double scale = 0.4; // échelle 
	const int first_pg_x = game_window_width/2 - (2300*scale)/2;
	const int first_pg_y = 100;
	const int second_pg_x = 300*scale + first_pg_x + scale * 1000;
	const int second_pg_y = first_pg_y;

	const int fps = 60; // nombre d'image par seconde dans le jeux
}