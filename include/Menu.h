#ifndef __MENU__
#define __MENU__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "Player.h"
#include "Essentials.h"

// menu do jogo
unsigned char menu (Essentials *essentials);

// carrega os sprites e ataques do personagem
Player *character_load (int player, int character);

// menu de selecao de personagem
int menuCharacter (Player **player_1, Player **player_2, Essentials *essentials);

// menu de selecao de mapa
int menuMap (ALLEGRO_BITMAP **background, unsigned char *background_count, Essentials *essentials);

// menu de fim de jogo
unsigned char endGameMenu (unsigned char winner, Essentials *essentials);

// menu de pause
int menu_pause (Essentials *essentials);

#endif