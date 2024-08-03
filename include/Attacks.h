#ifndef __ATTACKS__
#define __ATTACKS__

#include "Box.h"

typedef struct {
	box *attack_area;
	unsigned char attack_damage;
	unsigned char attack_time;
	unsigned char action_time;
} attacks;

attacks* attacks_create(unsigned short damage, unsigned char attack_time, unsigned short width, unsigned short height, unsigned short x, unsigned short y);
void attacks_destroy(attacks *element);

#endif	