#include <stdlib.h>
#include "Box.h"

box* box_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y)
{
	box *element = (box*) malloc (sizeof(box));
	if (!element) return NULL;

	element->width = width;
	element->height = height;
	element->x = x;
	element->y = y;

	return element;																	
}

void box_destroy(box *element)
{
	free (element);
}