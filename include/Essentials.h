#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>	

typedef struct Essentials {
	ALLEGRO_DISPLAY *disp;
	ALLEGRO_FONT *font;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_EVENT event;
	ALLEGRO_TIMER *timer;
} Essentials;

void start_essentials (Essentials **essentials);

void end_essentials (Essentials *essentials);
