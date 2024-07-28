#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>																																								//Biblioteca de figuras básicas
#include <stdio.h>
#include "Square.h"
#include "Essentials.h"	

unsigned char menu (Essentials *essentials);

square *character_load (int player, int character);

int menuCharacter (square **player_1, square **player_2, Essentials *essentials);

int menuMap (ALLEGRO_BITMAP **background, unsigned char *background_count, Essentials *essentials);

unsigned char endGameMenu (unsigned char winner, Essentials *essentials);

int menu_pause (Essentials *essentials);




