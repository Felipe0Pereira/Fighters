#ifndef __MENU__
#define __MENU__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "Player.h"
#include "Essentials.h"

unsigned char menu (Essentials *essentials);

Player *character_load (int player, int character);

int menuCharacter (Player **player_1, Player **player_2, Essentials *essentials);

int menuMap (ALLEGRO_BITMAP **background, unsigned char *background_count, Essentials *essentials);

unsigned char endGameMenu (unsigned char winner, Essentials *essentials);

int menu_pause (Essentials *essentials);

#endif