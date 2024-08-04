#ifndef __PLAYER__
#define __PLAYER__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "Joystick.h"
#include "Attacks.h"
#include "Box.h"

#define PLAYER_STEP 1
#define PROPORTION 5
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT PLAYER_WIDTH * PROPORTION

#define PLAYER_SPEED 10
#define JUMP_SPEED 33

#define MAX_HP 100
#define MAX_STAMINA 100

typedef struct Sprites {
	unsigned char quantity;
	box **props;
} Sprites;

typedef struct Actions {
	Sprites *standing;
	Sprites *walk;
	Sprites *jump;
	Sprites *crouch;
	Sprites *punch;
	Sprites *air_punch;
	Sprites *crouch_punch;
	Sprites *kick;
	Sprites *air_kick;
	Sprites *crouch_kick;
	Sprites *stuned;
	Sprites *death;
} Actions;

typedef struct Player {
	unsigned char character;
	unsigned char side;
	unsigned char height;
	unsigned char face;
	char vertSpeed;
	char movSpeed;
	unsigned char jump;
	unsigned char crouch;
	char hp;
	char stamina;
	unsigned char cooldown;
	unsigned char stuned;
	box *box;
	box *hurt_box;
	joystick *control;
	attacks *punch;
	attacks *air_punch;
	attacks *crouch_punch;
	attacks *kick;
	attacks *air_kick;
	attacks *crouch_kick;
	Actions *actions;	
	ALLEGRO_BITMAP *sprites;
	ALLEGRO_COLOR color;
} Player;

// cria um player
Player* player_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);

// move o player de acordo com as restricoes de movimento
void player_move(Player *element, char steps, unsigned char trajectory, unsigned short min_x, unsigned short min_y, unsigned short max_x, unsigned short max_y);

// desaloca memoria da estrutura player
void player_destroy(Player *element);

#endif