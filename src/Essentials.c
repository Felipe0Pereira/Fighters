#include <stdio.h>

#include "Essentials.h"
#define X_SCREEN 960																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 540

void start_essentials (Essentials **essentials)
{
	al_init();																																												//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																																												//Faz a inicialização dos addons das imagens básicas
	al_init_image_addon();
	al_install_keyboard();

	(*essentials)->disp = al_create_display(X_SCREEN, Y_SCREEN);
	(*essentials)->font = al_create_builtin_font();																																								//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	(*essentials)->queue = al_create_event_queue();
	(*essentials)->timer = al_create_timer(1.0 / 30.0);																																						//Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
	al_start_timer((*essentials)->timer);

	al_register_event_source((*essentials)->queue, al_get_display_event_source((*essentials)->disp));																																							//Variável que guarda um evento capturado, sua estrutura é definida em: https:		//www.allegro.cc/manual/5/ALLEGRO_EVENT
	al_register_event_source((*essentials)->queue, al_get_keyboard_event_source());
	al_register_event_source((*essentials)->queue, al_get_timer_event_source((*essentials)->timer));	
}

void end_essentials (Essentials *essentials)
{
	al_destroy_font(essentials->font);																																													//Destrutor da fonte padrão
	al_destroy_display(essentials->disp);																																												//Destrutor da tela
	al_destroy_timer(essentials->timer);																																												//Destrutor do relógio
	al_destroy_event_queue(essentials->queue);

	free (essentials);
}