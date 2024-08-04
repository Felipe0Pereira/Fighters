#include <stdlib.h>
#include "Joystick.h"

joystick* joystick_create(){														//Implementação da função "joystick_create"
	joystick *element = (joystick*) malloc (sizeof(joystick));						//Aloca memória na heap para um novo controle
	if (!element) return NULL;														//Se a alocação não deu certo, retorna erro
	element->right = 0;																//Insere o estado de desligado para o botão de movimentação à esquerda
	element->left = 0;																//Insere o estado de desligado para o botão de movimentação à direita
	element->up = 0;																//Insere o estado de desligado para o botão de movimentação para cima
	element->down = 0;																//Insere o estado de desligado para o botão de movimentação para baixo
	element->punch = 0;
	element->kick = 0;																//Insere o estado de desligado para o botão de disparo
	return element;																	//Retorna o novo controle
}

void joystick_destroy(joystick *element){ free(element);}							//Implementação da função "joystick_destroy"; libera a memória do elemento na heap

void joystick_reset (joystick *element) {											
	element->right = 0;																//Insere o estado de desligado para o botão de movimentação à esquerda
	element->left = 0;																//Insere o estado de desligado para o botão de movimentação à direita
	element->up = 0;																//Insere o estado de desligado para o botão de movimentação para cima
	element->down = 0;																//Insere o estado de desligado para o botão de movimentação para baixo
	element->punch = 0;																//Insere o estado de desligado para o botão de soco
	element->kick = 0;																//Insere o estado de desligado para o botão de chute
}