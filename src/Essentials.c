#include "Essentials.h"

unsigned char start_essentials (Essentials **essentials)
{
	al_init();
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();
    al_init_font_addon();  // Inicializa o addon de fontes
    al_init_ttf_addon();   // Inicializa o addon para fontes TrueType


	(*essentials)->disp = al_create_display(X_SCREEN, Y_SCREEN);
	if (!(*essentials)->disp)
		return 0;
	(*essentials)->title = al_load_ttf_font("fonts/Pixelmania.ttf", 42, 0);
	if (!(*essentials)->title)
		return 0;
	(*essentials)->font = al_load_ttf_font("fonts/Kemco_Pixel_Bold.ttf", 20, 0);
	if (!(*essentials)->font)
		return 0;
	(*essentials)->font2 = al_load_ttf_font("fonts/Kemco_Pixel_Bold.ttf", 14, 0);
	if (!(*essentials)->font2)
		return 0;
	(*essentials)->queue = al_create_event_queue();
	if (!(*essentials)->queue)
		return 0;
	(*essentials)->timer = al_create_timer(1.0 / 30.0);
	if (!(*essentials)->timer)
		return 0;

	al_start_timer((*essentials)->timer);

	al_register_event_source((*essentials)->queue, al_get_display_event_source((*essentials)->disp));
	al_register_event_source((*essentials)->queue, al_get_keyboard_event_source());
	al_register_event_source((*essentials)->queue, al_get_timer_event_source((*essentials)->timer));	

	return 1;
}

void end_essentials (Essentials *essentials)
{
	al_destroy_font(essentials->title);	
	al_destroy_font(essentials->font);	
	al_destroy_font(essentials->font2);
	al_destroy_display(essentials->disp);
	al_destroy_timer(essentials->timer);
	al_destroy_event_queue(essentials->queue);

	free (essentials);
}