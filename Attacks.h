#ifndef __ATTACKS__ 																												//Guardas de inclusão 
#define __ATTACKS__																												//Guardas de inclusão 

#include "Box.h"

typedef struct {																													//Definição da estrutura de um controle 
	box *attack_area;																											//Botão de movimentação à direta 
	unsigned char attack_damage;																					//Botão de movimentação à esquerda 
	unsigned char attack_time;
} attacks;

attacks* attacks_create(unsigned short damage, unsigned short width, unsigned short height, unsigned short x, unsigned short y);
void attacks_destroy(attacks *element);

#endif	