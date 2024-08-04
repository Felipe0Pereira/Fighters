#include <stdlib.h>
#include "Player.h"

Player* player_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){

	if ((x - side/2 < 0) || (x + side/2 > max_x) || (y - side*2/2 < 0) || (y + side*2/2 > max_y)) return NULL;
	if (face > 3) return NULL;

	Player *new_player = (Player*) malloc(sizeof(Player));
	if (!new_player) return NULL;
	new_player->face = face;
	new_player->vertSpeed = 0;
	new_player->movSpeed = 0;
	new_player->jump = 1;
	new_player->crouch = 0;
	new_player->hp = 100;
	new_player->stamina = 100;
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
	return new_player;
}

void player_move(Player *element, char steps, unsigned char trajectory, unsigned short min_x, unsigned short min_y, unsigned short max_x, unsigned short max_y){

	if (!trajectory){ if (((element->box->x - steps*PLAYER_STEP) - element->box->width/2 >= min_x) && ((element->box->x - steps*PLAYER_STEP) - element->box->width/2 >= 0)) element->box->x = element->box->x - steps*PLAYER_STEP;} 						//Verifica se a movimentação para a esquerda é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 1){ if ((element->box->x + steps*PLAYER_STEP) + element->box->width/2 <= max_x) element->box->x = element->box->x + steps*PLAYER_STEP;}			//Verifica se a movimentação para a direita é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 2){ if (((element->box->y - steps*PLAYER_STEP) - element->box->height/2 >= min_y) && ((element->box->y - steps*PLAYER_STEP) - element->box->height/2 >= 0)) element->box->y = element->box->y - steps*PLAYER_STEP;}				//Verifica se a movimentação para cima é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 3){ if ((element->box->y + steps*PLAYER_STEP) + element->box->height/2 <= max_y) element->box->y = element->box->y + steps*PLAYER_STEP;}			//Verifica se a movimentação para baixo é desejada e possível; se sim, efetiva a mesma
}

void actions_destroy (Actions *actions)
{
	// libera o vetor de posicoes
	for (int i = 0; i < actions->standing->quantity; i++)
		box_destroy(actions->standing->props[i]);
	for (int i = 0; i < actions->walk->quantity; i++)
		box_destroy(actions->walk->props[i]);
	for (int i = 0; i < actions->jump->quantity; i++)
		box_destroy(actions->jump->props[i]);
	for (int i = 0; i < actions->crouch->quantity; i++)
		box_destroy(actions->crouch->props[i]);
	for (int i = 0; i < actions->punch->quantity; i++)
		box_destroy(actions->punch->props[i]);
	for (int i = 0; i < actions->air_punch->quantity; i++)
		box_destroy(actions->air_punch->props[i]);
	for (int i = 0; i < actions->crouch_punch->quantity; i++)
		box_destroy(actions->crouch_punch->props[i]);
	for (int i = 0; i < actions->kick->quantity; i++)
		box_destroy(actions->kick->props[i]);
	for (int i = 0; i < actions->air_kick->quantity; i++)
		box_destroy(actions->air_kick->props[i]);
	for (int i = 0; i < actions->crouch_kick->quantity; i++)
		box_destroy(actions->crouch_kick->props[i]);
	for (int i = 0; i < actions->stuned->quantity; i++)
		box_destroy(actions->stuned->props[i]);
	for (int i = 0; i < actions->death->quantity; i++)
		box_destroy(actions->death->props[i]);

	// libera as estrutura das acoes
	free (actions->standing);
	free (actions->walk);
	free (actions->jump);
	free (actions->crouch);
	free (actions->punch);
	free (actions->air_punch);
	free (actions->crouch_punch);
	free (actions->kick);
	free (actions->air_kick);
	free (actions->crouch_kick);
	free (actions->stuned);
	free (actions->death);
	free (actions);
}

void player_destroy(Player *element){
	// Destroi sprites
	actions_destroy (element->actions);
	al_destroy_bitmap (element->sprites);

	// destroi ataques
	attacks_destroy (element->punch);
	attacks_destroy (element->crouch_punch);
	attacks_destroy (element->air_punch);
	attacks_destroy (element->kick);
	attacks_destroy (element->crouch_kick);
	attacks_destroy (element->air_kick);

	// destroi areas, controle e libera estrutura do player
	box_destroy(element->box);
	box_destroy(element->hurt_box);
	joystick_destroy(element->control);																													//Destrói o controle do quadrado
	free(element);																																		//Libera a memória do quadrado na heap
}