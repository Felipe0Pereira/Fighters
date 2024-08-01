#include <stdlib.h>
#include "Player.h"

Player* player_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){			//Implementação da função "square_create"

	if ((x - side/2 < 0) || (x + side/2 > max_x) || (y - side*2/2 < 0) || (y + side*2/2 > max_y)) return NULL;												//Verifica se a posição inicial é válida; caso não seja, retorna NULL
	if (face > 3) return NULL;																															//Verifica se a face principal do quadrado é válida

	Player *new_player = (Player*) malloc(sizeof(Player));																								//Aloca memória na heap para um novo quadrado
	if (!new_player) return NULL;																														//Se a alocação não deu certo, retorna erro													
	new_player->face = face;																															//Insere a indicação da face principal do quadrado
	new_player->vertSpeed = 0;
	new_player->movSpeed = 0;
	new_player->jump = 0;
	new_player->crouch = 0;
	new_player->hp = 100;
	new_player->stamina = 100;																																	//Insere o total de pontos de vida de um quadrado (!)
	new_player->cooldown = 0;
	new_player->stuned = 0;
	new_player->box = box_create(side, side * PROPORTION, x, y);
	new_player->hurt_box = box_create(side/2 * PROPORTION, side * PROPORTION, x, y);
	new_player->control = joystick_create();

	if (!(new_player->actions = malloc (sizeof (Actions))))
		return NULL;

	if (!(new_player->actions->standing = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->walk = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->jump = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->crouch = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->punch = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->air_punch = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->crouch_punch = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->kick = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->air_kick = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->crouch_kick = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->stuned = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_player->actions->death = malloc (sizeof (Sprites))))
		return NULL;
	return new_player;																																	//Retorna o novo quadrado
}

void player_move(Player *element, char steps, unsigned char trajectory, unsigned short min_x, unsigned short min_y, unsigned short max_x, unsigned short max_y){									//Implementação da função "square_move"

	if (!trajectory){ if (((element->box->x - steps*PLAYER_STEP) - element->box->width/2 >= min_x) && ((element->box->x - steps*PLAYER_STEP) - element->box->width/2 >= 0)) element->box->x = element->box->x - steps*PLAYER_STEP;} 						//Verifica se a movimentação para a esquerda é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 1){ if ((element->box->x + steps*PLAYER_STEP) + element->box->width/2 <= max_x) element->box->x = element->box->x + steps*PLAYER_STEP;}			//Verifica se a movimentação para a direita é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 2){ if (((element->box->y - steps*PLAYER_STEP) - element->box->height/2 >= min_y) && ((element->box->y - steps*PLAYER_STEP) - element->box->height/2 >= 0)) element->box->y = element->box->y - steps*PLAYER_STEP;}				//Verifica se a movimentação para cima é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 3){ if ((element->box->y + steps*PLAYER_STEP) + element->box->height/2 <= max_y) element->box->y = element->box->y + steps*PLAYER_STEP;}			//Verifica se a movimentação para baixo é desejada e possível; se sim, efetiva a mesma
}


void player_destroy(Player *element){																													//Implementação da função "square_destroy"
	free (element->actions->standing);
	free (element->actions->walk);
	free (element->actions->jump);
	free (element->actions->crouch);
	free (element->actions->punch);
	free (element->actions->air_punch);
	free (element->actions->crouch_punch);
	free (element->actions->kick);
	free (element->actions->air_kick);
	free (element->actions->crouch_kick);
	free (element->actions->stuned);
	free (element->actions->death);
	free (element->actions);
	al_destroy_bitmap (element->sprites);

	attacks_destroy (element->punch);
	attacks_destroy (element->crouch_punch);
	attacks_destroy (element->air_punch);
	attacks_destroy (element->kick);
	attacks_destroy (element->crouch_kick);
	attacks_destroy (element->air_kick);

	box_destroy(element->box);
	box_destroy(element->hurt_box);
	joystick_destroy(element->control);																													//Destrói o controle do quadrado
	free(element);																																		//Libera a memória do quadrado na heap
}