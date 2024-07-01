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
	new_square->hp = 5;																																	//Insere o total de pontos de vida de um quadrado (!)
	new_square->cooldown = 0;
	new_square->box = box_create(side, side * PROPORTION, x, y);
	new_square->control = joystick_create();
	if (face == 0) {																											//Insere o elemento de controle do quadrado
		new_square->punch = attacks_create(1, 120, 15,x - (side/2 + 120/2), y);
		new_square->air_punch = attacks_create(1, 120, 15,x - (side/2 + 120/2), y);
		new_square->kick = attacks_create(1, 120, 20,x - (side/2 + 120/2), y + 10);
	}
	else {
		new_square->punch = attacks_create(1, 120, 15,x + (side/2 + 120/2), y);
		new_square->air_punch = attacks_create(1, 120, 15,x - (side/2 + 120/2), y);
		new_square->kick = attacks_create(1, 120, 20,x + (side/2 + 120/2), y + 10);
	}
	new_square->gun = pistol_create();																													//Insere o elemento de disparos do quadrado
	return new_square;																																	//Retorna o novo quadrado
}

void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y){									//Implementação da função "square_move"

	if (!trajectory){ if ((element->box->x - steps*SQUARE_STEP) - element->box->width/2 >= 0) element->box->x = element->box->x - steps*SQUARE_STEP;} 						//Verifica se a movimentação para a esquerda é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 1){ if ((element->box->x + steps*SQUARE_STEP) + element->box->width/2 <= max_x) element->box->x = element->box->x + steps*SQUARE_STEP;}			//Verifica se a movimentação para a direita é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 2){ if ((element->box->y - steps*SQUARE_STEP) - element->box->height/2 >= 0) element->box->y = element->box->y - steps*SQUARE_STEP;}				//Verifica se a movimentação para cima é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 3){ if ((element->box->y + steps*SQUARE_STEP) + element->box->height/2 <= max_y) element->box->y = element->box->y + steps*SQUARE_STEP;}			//Verifica se a movimentação para baixo é desejada e possível; se sim, efetiva a mesma
}

void square_shot(square *element){																														//Implementação da função "square_shot"
	bullet *shot;

	if (!element->face) shot = pistol_shot(element->box->x - element->box->width/2, element->box->y, element->face, element->gun);										//Quadrado atira para a esquerda
	else if (element->face == 1) shot = pistol_shot(element->box->x + element->box->width/2, element->box->y, element->face, element->gun);								//Quadrado atira para a direita
	if (shot) element->gun->shots = shot;
}

void square_destroy(square *element){																													//Implementação da função "square_destroy"
	box_destroy(element->box);
	pistol_destroy(element->gun);																														//Destrói o armemento do quadrado
	joystick_destroy(element->control);																													//Destrói o controle do quadrado
	free(element);																																		//Libera a memória do quadrado na heap
}