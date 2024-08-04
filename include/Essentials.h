#ifndef __ESSENTIALS__
#define __ESSENTIALS__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>	

#define X_SCREEN 960 // largura da tela 
#define Y_SCREEN 540 // altura da tela

#define X_MAP 1728 // largura do mapa
#define FLOOR Y_SCREEN - 10 // altura do chao do mapa
#define RATIO 3.186 // proporcao do mapa

#define GRAVITY 2 // aceleracao da gravidade

typedef struct Essentials {
	ALLEGRO_DISPLAY *disp;
	ALLEGRO_FONT *title;
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *font2;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_EVENT event;
	ALLEGRO_TIMER *timer;
} Essentials;

// inicia os itens allegro essenciais para o jogo
unsigned char start_essentials (Essentials **essentials);

// desaloca memoria dos itens essencias
void end_essentials (Essentials *essentials);

#endif