#include <stdlib.h>
#include "Square.h"

square* square_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){			//Implementação da função "square_create"

	if ((x - side/2 < 0) || (x + side/2 > max_x) || (y - side*2/2 < 0) || (y + side*2/2 > max_y)) return NULL;												//Verifica se a posição inicial é válida; caso não seja, retorna NULL
	if (face > 3) return NULL;																															//Verifica se a face principal do quadrado é válida

	square *new_square = (square*) malloc(sizeof(square));																								//Aloca memória na heap para um novo quadrado
	if (!new_square) return NULL;																														//Se a alocação não deu certo, retorna erro													
	new_square->face = face;																															//Insere a indicação da face principal do quadrado
	new_square->vertSpeed = 0;
	new_square->movSpeed = 0;
	new_square->jump = 0;
	new_square->crouch = 0;
	new_square->hp = 5;
	new_square->stamina = 100;																																	//Insere o total de pontos de vida de um quadrado (!)
	new_square->cooldown = 0;
	new_square->stuned = 0;
	new_square->box = box_create(side, side * PROPORTION, x, y);
	new_square->hurt_box = box_create(side/2 * PROPORTION, side * PROPORTION, x, y);
	new_square->control = joystick_create();

	new_square->gun = pistol_create();
	if (!(new_square->actions = malloc (sizeof (Actions))))
		return NULL;

	if (!(new_square->actions->standing = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->walk = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->jump = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->crouch = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->punch = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->air_punch = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->crouch_punch = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->kick = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->air_kick = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->crouch_kick = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->stuned = malloc (sizeof (Sprites))))
		return NULL;
	if (!(new_square->actions->death = malloc (sizeof (Sprites))))
		return NULL;
	return new_square;																																	//Retorna o novo quadrado
}

void square_move(square *element, char steps, unsigned char trajectory, unsigned short min_x, unsigned short min_y, unsigned short max_x, unsigned short max_y){									//Implementação da função "square_move"

	if (!trajectory){ if (((element->box->x - steps*SQUARE_STEP) - element->box->width/2 >= min_x) && ((element->box->x - steps*SQUARE_STEP) - element->box->width/2 >= 0)) element->box->x = element->box->x - steps*SQUARE_STEP;} 						//Verifica se a movimentação para a esquerda é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 1){ if ((element->box->x + steps*SQUARE_STEP) + element->box->width/2 <= max_x) element->box->x = element->box->x + steps*SQUARE_STEP;}			//Verifica se a movimentação para a direita é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 2){ if (((element->box->y - steps*SQUARE_STEP) - element->box->height/2 >= min_y) && ((element->box->y - steps*SQUARE_STEP) - element->box->height/2 >= 0)) element->box->y = element->box->y - steps*SQUARE_STEP;}				//Verifica se a movimentação para cima é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 3){ if ((element->box->y + steps*SQUARE_STEP) + element->box->height/2 <= max_y) element->box->y = element->box->y + steps*SQUARE_STEP;}			//Verifica se a movimentação para baixo é desejada e possível; se sim, efetiva a mesma
}

void square_shot(square *element){
	bullet *shot;

	if (!element->face) shot = pistol_shot(element->box->x - element->box->width/2, element->box->y, element->face, element->gun);										//Quadrado atira para a esquerda
	else if (element->face == 1) shot = pistol_shot(element->box->x + element->box->width/2, element->box->y, element->face, element->gun);								//Quadrado atira para a direita
	if (shot) element->gun->shots = shot;
}

void square_destroy(square *element){																													//Implementação da função "square_destroy"
	box_destroy(element->box);
	box_destroy(element->hurt_box);
	pistol_destroy(element->gun);																														//Destrói o armemento do quadrado
	joystick_destroy(element->control);																													//Destrói o controle do quadrado
	al_destroy_bitmap (element->sprites);
	free(element);																																		//Libera a memória do quadrado na heap
}