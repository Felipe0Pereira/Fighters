#include <stdlib.h>
#include "Attacks.h"

attacks* attacks_create(unsigned short damage, unsigned short width, unsigned short height, unsigned short x, unsigned short y)
{
	attacks *element = (attacks*) malloc (sizeof(attacks));						//Aloca memória na heap para um novo controle
	if (!element) return NULL;

	element->attack_area = box_create (width, height, x, y);																
	element->attack_damage = 1;		
	element->attack_time = 18;	
	element->action_time = 0;														
	return element;														
}

void attacks_destroy(attacks *element)
{ 
	box_destroy (element->attack_area);
	free (element);
}
