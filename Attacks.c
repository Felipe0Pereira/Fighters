#include <stdlib.h>
#include "Attacks.h"

attacks* attacks_create(){

	attacks *element = (attacks*) malloc (sizeof(attacks));						//Aloca memória na heap para um novo controle
	if (!element) return NULL;

	box *punch = (box*) malloc (sizeof(box));						//Aloca memória na heap para um novo controle
	if (!element) return NULL;

	box *kick = (box*) malloc (sizeof(box));						//Aloca memória na heap para um novo controle
	if (!element) return NULL;		

	punch->x = 10;
	punch->y = 10;

	kick->x = 15;
	kick->x = 15;

	element->punch = punch;																
	element->kick = kick;																
	return element;																	
}

void attacks_destroy(attacks *element)
{ 
	free (element->punch); 
	free (element->kick);
	free (element);
}