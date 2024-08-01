#ifndef __ESSENTIALS__
#define __ESSENTIALS__

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>																																										//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>	

typedef struct Essentials {
	ALLEGRO_DISPLAY *disp;
	ALLEGRO_FONT *title;
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *font2;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_EVENT event;
	ALLEGRO_TIMER *timer;
} Essentials;

void start_essentials (Essentials **essentials);

void end_essentials (Essentials *essentials);

#endif