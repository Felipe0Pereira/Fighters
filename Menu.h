#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>																																								//Biblioteca de figuras básicas
#include <stdio.h>
#include "Square.h"

unsigned char menu (ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp);

int menuCharacter (square **player_1, square **player_2, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp);

int menuMap (ALLEGRO_BITMAP **background, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp);

unsigned char endGameMenu (unsigned char winner, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp);

int menu_pause (ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp);




