#include "Menu.h"

#define X_SCREEN 960
#define Y_SCREEN 540
#define FLOOR Y_SCREEN - 10

#define X_MAP 1728


void menu_up (char *opt)
{
	if (*opt == 0) return;
	if (*opt == 1) {*opt = 0; return;}
	if (*opt == 2) {*opt = 1; return;}
}


void menu_down (char *opt)
{
	if (*opt == 0) {*opt = 1; return;}
	if (*opt == 1) {*opt = 2; return;}
	if (*opt == 2) return;
}

unsigned char options (Essentials *essentials)
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(essentials->font, al_map_rgb(255, 0, 0), X_SCREEN/4, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PLAYER 1");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN/4 - 100, Y_SCREEN/2 +20, ALLEGRO_ALIGN_LEFT, "Move left: A");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN/4 - 100, Y_SCREEN/2 +40, ALLEGRO_ALIGN_LEFT, "Move right: D");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN/4 - 100, Y_SCREEN/2 +60, ALLEGRO_ALIGN_LEFT, "Jump: W");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN/4 - 100, Y_SCREEN/2 +80, ALLEGRO_ALIGN_LEFT, "Crouch: S");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN/4 - 100, Y_SCREEN/2 +100, ALLEGRO_ALIGN_LEFT, "Punch or select: J");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN/4 - 100, Y_SCREEN/2 +120, ALLEGRO_ALIGN_LEFT, "Kick: K");


	al_draw_text(essentials->font, al_map_rgb(0, 0, 255), X_SCREEN - X_SCREEN/4, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PLAYER 2");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN - X_SCREEN/4 -100, Y_SCREEN/2 +20, ALLEGRO_ALIGN_LEFT, "Move left: LEFT ARROW");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN - X_SCREEN/4 -100, Y_SCREEN/2 +40, ALLEGRO_ALIGN_LEFT, "Move right: RIGHT ARROW");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN - X_SCREEN/4 -100, Y_SCREEN/2 +60, ALLEGRO_ALIGN_LEFT, "Jump: UP ARROW");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN - X_SCREEN/4 -100, Y_SCREEN/2 +80, ALLEGRO_ALIGN_LEFT, "Crouch: DOWN ARROW");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN - X_SCREEN/4 -100, Y_SCREEN/2 +100, ALLEGRO_ALIGN_LEFT, "Punch or select: NUM PAD 1");
	al_draw_text(essentials->font2, al_map_rgb(255, 255, 255), X_SCREEN - X_SCREEN/4 -100, Y_SCREEN/2 +120, ALLEGRO_ALIGN_LEFT, "Kick: NUM PAD 2");		

	al_draw_text(essentials->font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 +200 , ALLEGRO_ALIGN_CENTRE, "BACK");
	al_flip_display();
 		
	while (1) {
		al_wait_for_event(essentials->queue, &(essentials->event));
		if ((essentials->event.type == 10)){									
			if (essentials->event.keyboard.keycode == ALLEGRO_KEY_ENTER || essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) return 1;
			else if (essentials->event.keyboard.keycode == 3) return 1;
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_1) return 1;
		}																																			
		else if (essentials->event.type == 42) return 0;
	}
	return 0;
}

unsigned char menu (Essentials *essentials)
{
	char opt = 0;
	while (1) {
		al_wait_for_event(essentials->queue, &(essentials->event));	

		al_clear_to_color(al_map_rgb(0, 0, 0));

		al_draw_text(essentials->title, al_map_rgb(255, 255, 255), X_SCREEN/2, 200, ALLEGRO_ALIGN_CENTRE, "LEAK FIGHTERS");
		
		if (opt == 0)
			al_draw_text(essentials->font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "PLAY");
		else		
			al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "PLAY");
		if (opt == 1)
			al_draw_text(essentials->font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 90, ALLEGRO_ALIGN_CENTRE, "CONTROLS");
		else
			al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 90, ALLEGRO_ALIGN_CENTRE, "CONTROLS");
		if (opt == 2)
			al_draw_text(essentials->font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 120, ALLEGRO_ALIGN_CENTRE, "EXIT");
		else
			al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 120, ALLEGRO_ALIGN_CENTRE, "EXIT");
		al_flip_display();
 		
		if ((essentials->event.type == 10)){
			if (essentials->event.keyboard.keycode == 23) menu_up(&opt);
			else if (essentials->event.keyboard.keycode == 19) menu_down(&opt);
			else if (essentials->event.keyboard.keycode == 84) menu_up(&opt);
			else if (essentials->event.keyboard.keycode == 85) menu_down(&opt);
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_ENTER || essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
				if (opt == 0) return 1;
				else if (opt == 1) {if (!options (essentials)) return 0;}
				else if (opt == 2) return 2;
			}
			else if (essentials->event.keyboard.keycode == 3) {
				if (opt == 0) return 1;
				else if (opt == 1) {if (!options (essentials)) return 0;}
				else if (opt == 2) return 2;
			}
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_1) {
				if (opt == 0) return 1;
				else if (opt == 1) {if (!options (essentials)) return 0;}
				else if (opt == 2) return 2;
			}
		}																																			
		else if (essentials->event.type == 42) return 2;
	}
}

int spritesLoad (Player *player, char character)
{
	if (character == 1) {		
		player->sprites = al_load_bitmap("characters/Donatello.png");
		if (!player->sprites)
			return 0;
		
		player->actions->standing->quantity = 6;
		player->actions->walk->quantity = 6;
		player->actions->jump->quantity = 3;
		player->actions->crouch->quantity = 1;
		player->actions->punch->quantity = 3;
		player->actions->air_punch->quantity = 2;
		player->actions->crouch_punch->quantity = 2;
		player->actions->kick->quantity = 3;
		player->actions->air_kick->quantity = 2;
		player->actions->crouch_kick->quantity = 3;
		player->actions->stuned->quantity = 2;
		player->actions->death->quantity = 3;

		if (!(player->actions->standing->props = malloc (sizeof (box *) * 6)))
			return 0;
		if (!(player->actions->walk->props = malloc (sizeof (box *) * 6)))
			return 0;
		if (!(player->actions->jump->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->crouch->props = malloc (sizeof (box *))))
			return 0;
		if (!(player->actions->punch->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->air_punch->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->crouch_punch->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->kick->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->air_kick->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->crouch_kick->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->stuned->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->death->props = malloc (sizeof (box *) * 3)))
			return 0;


		player->actions->standing->props[0] = box_create(75, 75, 70, 20);
		player->actions->standing->props[1] = box_create(75, 75, 145, 18);
		player->actions->standing->props[2] = box_create(75, 75, 215, 16);
		player->actions->standing->props[3] = box_create(75, 75, 215, 16);
		player->actions->standing->props[4] = box_create(75, 75, 145, 18);
		player->actions->standing->props[5] = box_create(75, 75, 70, 20);

		player->actions->walk->props[0] = box_create(75, 75, 0, 115);
		player->actions->walk->props[1] = box_create(75, 75, 70, 115);
		player->actions->walk->props[2] = box_create(75, 75, 140, 115);
		player->actions->walk->props[3] = box_create(75, 75, 140, 115);
		player->actions->walk->props[4] = box_create(75, 75, 70, 115);
		player->actions->walk->props[5] = box_create(75, 75, 0, 115);
		
		player->actions->jump->props[0] = box_create(90, 80, 0, 749);
		player->actions->jump->props[1] = box_create(90, 80, 80, 749);
		player->actions->jump->props[2] = box_create(90, 80, 170, 745);

		player->actions->crouch->props[0] = box_create(75, 75, 90, 495);

		player->actions->punch->props[0] = box_create(75, 75, 0, 200);
		player->actions->punch->props[1] = box_create(75, 75, 85, 200);
		player->actions->punch->props[2] = box_create(110, 75, 160, 200);

		player->actions->air_punch->props[0] = box_create(75, 75, 0, 855);
		player->actions->air_punch->props[1] = box_create(100, 75, 75, 853);

		player->actions->crouch_punch->props[0] = box_create(75, 75, 4, 565);
		player->actions->crouch_punch->props[1] = box_create(110, 75, 92, 565);

		player->actions->kick->props[0] = box_create(80, 80, 183, 390);
		player->actions->kick->props[1] = box_create(80, 80, 276, 387);
		player->actions->kick->props[2] = box_create(110, 80, 368, 387);

		player->actions->air_kick->props[0] = box_create(80, 80, 6, 938);
		player->actions->air_kick->props[1] = box_create(110, 80, 90, 938);


		player->actions->crouch_kick->props[0] = box_create(75, 75, 220, 565);
		player->actions->crouch_kick->props[1] = box_create(100, 75, 305, 565);
		player->actions->crouch_kick->props[2] = box_create(90, 75, 402, 565);

		player->actions->stuned->props[0] = box_create(75, 75, 173, 1689);
		player->actions->stuned->props[1] = box_create(75, 75, 266, 1692);

		player->actions->death->props[0] = box_create(100, 75, 8, 1600);
		player->actions->death->props[1] = box_create(100, 75, 103, 1590);
		player->actions->death->props[2] = box_create(100, 75, 203, 1585);
		

	}
	else if (character == 2) {
		player->sprites = al_load_bitmap("characters/Leonardo.png");
		if (!player->sprites)
			return 0;

		player->actions->standing->quantity = 6;
		player->actions->walk->quantity = 6;
		player->actions->jump->quantity = 3;
		player->actions->crouch->quantity = 1;
		player->actions->punch->quantity = 2;
		player->actions->air_punch->quantity = 2;
		player->actions->crouch_punch->quantity = 2;
		player->actions->kick->quantity = 2;
		player->actions->air_kick->quantity = 2;
		player->actions->crouch_kick->quantity = 2;
		player->actions->stuned->quantity = 2;
		player->actions->death->quantity = 3;

		if (!(player->actions->standing->props = malloc (sizeof (box *) * 6)))
			return 0;
		if (!(player->actions->walk->props = malloc (sizeof (box *) * 6)))
			return 0;
		if (!(player->actions->jump->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->crouch->props = malloc (sizeof (box *))))
			return 0;
		if (!(player->actions->punch->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->air_punch->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->crouch_punch->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->kick->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->air_kick->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->crouch_kick->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->stuned->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->death->props = malloc (sizeof (box *) * 3)))
			return 0;

		player->actions->standing->props[0] = box_create(75, 105, 57, 0);
		player->actions->standing->props[1] = box_create(75, 105, 130, 0);
		player->actions->standing->props[2] = box_create(70, 105, 200, 0);
		player->actions->standing->props[3] = box_create(70, 105, 200, 0);
		player->actions->standing->props[4] = box_create(75, 105, 130, 0);
		player->actions->standing->props[5] = box_create(75, 105, 57, 0);

		player->actions->walk->props[0] = box_create(75, 100, 0, 105);
		player->actions->walk->props[1] = box_create(75, 100, 70, 105);
		player->actions->walk->props[2] = box_create(75, 100, 140, 105);
		player->actions->walk->props[3] = box_create(75, 100, 140, 105);
		player->actions->walk->props[4] = box_create(75, 100, 70, 105);
		player->actions->walk->props[5] = box_create(75, 100, 0, 105);

		player->actions->jump->props[0] = box_create(110, 100, 6, 705);
		player->actions->jump->props[1] = box_create(75, 100, 222, 700);
		player->actions->jump->props[2] = box_create(70, 100, 301, 701);
		
		player->actions->crouch->props[0] = box_create(75, 75, 100, 425);

		player->actions->punch->props[0] = box_create(75, 75, 180, 220);
		player->actions->punch->props[1] = box_create(100, 75, 255, 215);

		player->actions->air_punch->props[0] = box_create(75, 80, 4, 808);
		player->actions->air_punch->props[1] = box_create(90, 90, 88, 795);

		player->actions->crouch_punch->props[0] = box_create(75, 75, 6, 516);
		player->actions->crouch_punch->props[1] = box_create(80, 75, 100, 516);

		player->actions->kick->props[0] = box_create(80, 100, 0, 312);
		player->actions->kick->props[1] = box_create(100, 110, 87, 312);
		

		player->actions->air_kick->props[0] = box_create(75, 80, 4, 808);
		player->actions->air_kick->props[1] = box_create(100, 75, 362, 803);

		player->actions->crouch_kick->props[0] = box_create(75, 75, 4, 620);
		player->actions->crouch_kick->props[1] = box_create(110, 75, 86, 620);

		player->actions->stuned->props[0] = box_create(75, 75, 448, 1215);
		player->actions->stuned->props[1] = box_create(75, 75, 330, 1303);
		
		player->actions->death->props[0] = box_create(90, 75, 96, 1203);
		player->actions->death->props[1] = box_create(90, 75, 186, 1210);
		player->actions->death->props[2] = box_create(95, 75, 0, 1200);
		

	}
	else if (character == 3) {
		player->sprites = al_load_bitmap("characters/Raphael.png");
		if (!player->sprites)
			return 0;
		
		player->actions->standing->quantity = 6;
		player->actions->walk->quantity = 6;
		player->actions->jump->quantity = 3;
		player->actions->crouch->quantity = 1;
		player->actions->punch->quantity = 3;
		player->actions->air_punch->quantity = 2;
		player->actions->crouch_punch->quantity = 3;
		player->actions->kick->quantity = 2;
		player->actions->air_kick->quantity = 2;
		player->actions->crouch_kick->quantity = 2;
		player->actions->stuned->quantity = 2;
		player->actions->death->quantity = 3;

		if (!(player->actions->standing->props = malloc (sizeof (box *) * 6)))
			return 0;
		if (!(player->actions->walk->props = malloc (sizeof (box *) * 6)))
			return 0;
		if (!(player->actions->jump->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->crouch->props = malloc (sizeof (box *))))
			return 0;
		if (!(player->actions->punch->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->air_punch->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->crouch_punch->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->kick->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->air_kick->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->crouch_kick->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->stuned->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->death->props = malloc (sizeof (box *) * 3)))
			return 0;


		player->actions->standing->props[0] = box_create(75, 75, 65, 22);
		player->actions->standing->props[1] = box_create(75, 75, 135, 20);
		player->actions->standing->props[2] = box_create(75, 75, 205, 18);
		player->actions->standing->props[3] = box_create(75, 75, 205, 18);
		player->actions->standing->props[4] = box_create(75, 75, 135, 20);
		player->actions->standing->props[5] = box_create(75, 75, 65, 22);
		
		player->actions->walk->props[0] = box_create(75, 75, 0, 120);
		player->actions->walk->props[1] = box_create(75, 75, 67, 120);
		player->actions->walk->props[2] = box_create(75, 75, 138, 120);
		player->actions->walk->props[3] = box_create(75, 75, 138, 120);
		player->actions->walk->props[4] = box_create(75, 75, 67, 120);
		player->actions->walk->props[5] = box_create(75, 75, 0, 120);

		player->actions->jump->props[0] = box_create(75, 100, 7, 736);
		player->actions->jump->props[1] = box_create(70, 90, 162, 732);
		player->actions->jump->props[2] = box_create(75, 90, 225, 732);
		
		player->actions->crouch->props[0] = box_create(75, 75, 72, 483);

		player->actions->punch->props[0] = box_create(75, 75, 5, 292);
		player->actions->punch->props[1] = box_create(100, 80, 82, 282);
		player->actions->punch->props[2] = box_create(100, 90, 178, 280);

		player->actions->air_punch->props[0] = box_create(75, 75, 6, 836);
		player->actions->air_punch->props[1] = box_create(90, 75, 81, 836);

		player->actions->crouch_punch->props[0] = box_create(75, 75, 1, 558);
		player->actions->crouch_punch->props[1] = box_create(75, 75, 84, 558);
		player->actions->crouch_punch->props[2] = box_create(100, 75, 160, 558);

		player->actions->kick->props[0] = box_create(75, 75, 4, 384);
		player->actions->kick->props[1] = box_create(90, 75, 81, 382);

		player->actions->air_kick->props[0] = box_create(75, 75, 4, 835);
		player->actions->air_kick->props[1] = box_create(100, 75, 4, 650);

		player->actions->crouch_kick->props[0] = box_create(75, 75, 1, 558);
		player->actions->crouch_kick->props[1] = box_create(100, 75, 107, 643);

		player->actions->stuned->props[0] = box_create(75, 75, 152, 1245);
		player->actions->stuned->props[1] = box_create(75, 75, 338, 1325);

		player->actions->death->props[0] = box_create(100, 75, 223, 1242);
		player->actions->death->props[1] = box_create(90, 75, 322, 1247);
		player->actions->death->props[2] = box_create(100, 75, 426, 1233);
		
	}
	else if (character == 4) {
		player->sprites = al_load_bitmap("characters/Michelangelo.png");
		if (!player->sprites)
			return 0;
		player->actions->standing->quantity = 6;
		player->actions->walk->quantity = 6;
		player->actions->jump->quantity = 3;
		player->actions->crouch->quantity = 1;
		player->actions->punch->quantity = 3;
		player->actions->air_punch->quantity = 3;
		player->actions->crouch_punch->quantity = 3;
		player->actions->kick->quantity = 2;
		player->actions->air_kick->quantity = 2;
		player->actions->crouch_kick->quantity = 1;
		player->actions->stuned->quantity = 2;
		player->actions->death->quantity = 3;

		if (!(player->actions->standing->props = malloc (sizeof (box *) * 6)))
			return 0;
		if (!(player->actions->walk->props = malloc (sizeof (box *) * 6)))
			return 0;
		if (!(player->actions->jump->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->crouch->props = malloc (sizeof (box *))))
			return 0;
		if (!(player->actions->punch->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->air_punch->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->crouch_punch->props = malloc (sizeof (box *) * 3)))
			return 0;
		if (!(player->actions->kick->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->air_kick->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->crouch_kick->props = malloc (sizeof (box *) * 1)))
			return 0;
		if (!(player->actions->stuned->props = malloc (sizeof (box *) * 2)))
			return 0;
		if (!(player->actions->death->props = malloc (sizeof (box *) * 3)))
			return 0;

		player->actions->standing->props[0] = box_create(75, 75, 60, 8);
		player->actions->standing->props[1] = box_create(75, 75, 135, 6);
		player->actions->standing->props[2] = box_create(75, 75, 210, 4);
		player->actions->standing->props[3] = box_create(75, 75, 210, 4);
		player->actions->standing->props[4] = box_create(75, 75, 135, 6);
		player->actions->standing->props[5] = box_create(75, 75, 60, 8);

		player->actions->walk->props[0] = box_create(75, 75, 0, 106);
		player->actions->walk->props[1] = box_create(75, 75, 73, 106);
		player->actions->walk->props[2] = box_create(75, 75, 145, 103);
		player->actions->walk->props[3] = box_create(75, 75, 145, 103);
		player->actions->walk->props[4] = box_create(75, 75, 73, 106);
		player->actions->walk->props[5] = box_create(75, 75, 0, 106);

		player->actions->jump->props[0] = box_create(75, 100, 5, 611);
		player->actions->jump->props[1] = box_create(70, 75, 162, 613);
		player->actions->jump->props[2] = box_create(75, 90, 230, 610);
		
		player->actions->crouch->props[0] = box_create(75, 75, 353, 378);

		player->actions->punch->props[0] = box_create(75, 75, 5, 190);
		player->actions->punch->props[1] = box_create(75, 75, 89, 190);
		player->actions->punch->props[2] = box_create(110, 75, 177, 190);

		player->actions->air_punch->props[0] = box_create(75, 100, 78, 711);
		player->actions->air_punch->props[1] = box_create(70, 100, 144, 705);
		player->actions->air_punch->props[2] = box_create(90, 75, 220, 720);

		player->actions->crouch_punch->props[0] = box_create(75, 75, 2, 468);
		player->actions->crouch_punch->props[1] = box_create(100, 75, 87, 466);
		player->actions->crouch_punch->props[2] = box_create(110, 75, 190, 466);

		player->actions->kick->props[0] = box_create(65, 75, 144, 285);
		player->actions->kick->props[1] = box_create(100, 90, 210, 277);

		player->actions->air_kick->props[0] = box_create(75, 75, 418, 716);
		player->actions->air_kick->props[1] = box_create(100, 75, 305, 716);

		player->actions->crouch_kick->props[0] = box_create(90, 75, 11, 538);

		player->actions->stuned->props[0] = box_create(70, 75, 138, 1246);
		player->actions->stuned->props[1] = box_create(70, 75, 3, 1365);
		
		player->actions->death->props[0] = box_create(100, 75, 205, 1255);
		player->actions->death->props[1] = box_create(90, 75, 300, 1256);
		player->actions->death->props[2] = box_create(100, 75, 393, 1243);
		

	}

	return 1;
}

int attack_load (Player *player, char character)
{
	if (character == 1) { //Donatello
		if (player->face == 0) {
			player->punch = attacks_create(5, 15, player->box->width *5, player->box->width,player->box->x - 100, player->box->y-20);
			player->air_punch = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x - 80, player->box->y-10);
			player->crouch_punch = attacks_create(5, 14, player->box->width *5, player->box->width,player->box->x - 100, player->box->y-40);
			player->kick = attacks_create(15, 18, player->box->width *5, player->box->width,player->box->x - 100, player->box->y - 40);
			player->air_kick = attacks_create(15, 18, player->box->width *5, player->box->width * 2,player->box->x - 90, player->box->y + 50);
			player->crouch_kick = attacks_create(8, 18, player->box->width *5, 120,player->box->x - 80, player->box->y);
		}
		else {
			player->punch = attacks_create(5, 15, player->box->width *5, player->box->width,player->box->x + 100, player->box->y-20);
			player->air_punch = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x +80, player->box->y-10);
			player->crouch_punch = attacks_create(5, 14, player->box->width *5, player->box->width,player->box->x + 100, player->box->y-40);
			player->kick = attacks_create(15, 18, player->box->width *5, player->box->width,player->box->x + 100, player->box->y - 40);
			player->air_kick = attacks_create(15, 18, player->box->width *5, player->box->width * 2,player->box->x - 90, player->box->y + 50);
			player->crouch_kick = attacks_create(8, 18, player->box->width *5, 120,player->box->x - 80, player->box->y);
		}
	}
	else if (character == 2) {
		if (player->face == 0) {
			player->punch = attacks_create(10, 12, player->box->width *5, 150,player->box->x - 80, player->box->y+10);
			player->air_punch = attacks_create(5, 18, player->box->width *3, 120,player->box->x - 60, player->box->y+60);
			player->crouch_punch = attacks_create(5, 10, player->box->width *4, player->box->width,player->box->x - 50, player->box->y-60);
			player->kick = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x - 80, player->box->y - 50);
			player->air_kick = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x - 80, player->box->y +40);
			player->crouch_kick = attacks_create(5, 12, player->box->width *5, player->box->width,player->box->x - 80, player->box->y+30);
		}
		else {
			player->punch = attacks_create(10, 18, player->box->width *5, 150,player->box->x + 80, player->box->y+10);
			player->air_punch = attacks_create(5, 18, player->box->width *3, 120,player->box->x + 60, player->box->y+60);
			player->crouch_punch = attacks_create(5, 10, player->box->width *4, player->box->width,player->box->x + 50, player->box->y-60);
			player->kick = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x + 80, player->box->y - 50);
			player->air_kick = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x - 80, player->box->y +40);
			player->crouch_kick = attacks_create(5, 12, player->box->width *5, player->box->width,player->box->x - 80, player->box->y+30);
		}
	}
	else if (character == 3) {
		if (player->face == 0) {
			player->punch = attacks_create(10, 18, player->box->width *4, 100, player->box->x - 100, player->box->y-50);
			player->air_punch = attacks_create(5, 18, player->box->width *4, player->box->width,player->box->x - 70, player->box->y+10);
			player->crouch_punch = attacks_create(5, 12, player->box->width *4, player->box->width,player->box->x - 100, player->box->y-60);
			player->kick = attacks_create(8, 14, player->box->width *4, 70,player->box->x - 70, player->box->y+20);
			player->air_kick = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x - 80, player->box->y +50);
			player->crouch_kick = attacks_create(5, 12, player->box->width *4, player->box->width,player->box->x - 90, player->box->y +30);
		}
		else {
			player->punch = attacks_create(10, 18, player->box->width *4, 100, player->box->x + 100, player->box->y-50);
			player->air_punch = attacks_create(5, 18, player->box->width *4, player->box->width,player->box->x + 70, player->box->y+10);
			player->crouch_punch = attacks_create(5, 12, player->box->width *4, player->box->width,player->box->x + 100, player->box->y-60);
			player->kick = attacks_create(8, 14, player->box->width *4, 70,player->box->x + 70, player->box->y+20);
			player->air_kick = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x - 80, player->box->y+50);
			player->crouch_kick = attacks_create(5, 12, player->box->width *4, player->box->width,player->box->x - 90, player->box->y+30);
		}
	}
	else if (character == 4) {
		if (player->face == 0) {
			player->punch = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x - 120, player->box->y-60);
			player->air_punch = attacks_create(5, 18, player->box->width *3, 150,player->box->x - 70, player->box->y-10);
			player->crouch_punch = attacks_create(10, 18, player->box->width *3, 120,player->box->x - 150, player->box->y-60);
			player->kick = attacks_create(8, 18, player->box->width *4, player->box->width,player->box->x - 100, player->box->y - 50);
			player->air_kick = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x - 100, player->box->y+30);
			player->crouch_kick = attacks_create(5, 10, player->box->width *4, player->box->width,player->box->x - 80, player->box->y +30);
		}
		else {
			player->punch = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x + 120, player->box->y-60);
			player->air_punch = attacks_create(5, 18, player->box->width *3, 150,player->box->x + 70, player->box->y-10);
			player->crouch_punch = attacks_create(1, 18, player->box->width *3, 120,player->box->x + 150, player->box->y-60);
			player->kick = attacks_create(10, 18, player->box->width *4, player->box->width,player->box->x + 100, player->box->y -50);
			player->air_kick = attacks_create(5, 18, player->box->width *5, player->box->width,player->box->x - 100, player->box->y+30);
			player->crouch_kick = attacks_create(5, 10, player->box->width *4, player->box->width,player->box->x - 80, player->box->y +30);
		}
	}
	else return 0;

	return 1;
}

Player *character_load (int player, int character)
{
	Player *element; 

	if (player == 1)
		element = player_create(40, 1, X_MAP/2 - X_SCREEN/2 +100, FLOOR - PLAYER_HEIGHT/2, X_MAP/2 + X_SCREEN/2, Y_SCREEN);
	else if (player == 2)
		element = player_create(40, 0, X_MAP/2 + X_SCREEN/2 - 100, FLOOR - PLAYER_HEIGHT/2, X_MAP/2 + X_SCREEN/2, Y_SCREEN);
	else return NULL;
	element->character = character;

	if (!spritesLoad (element, character) || !attack_load (element, character)) return NULL;
	return element;
}
int menuCharacter (Player **player_1, Player **player_2, Essentials *essentials)
{
	ALLEGRO_BITMAP *portraits = al_load_bitmap("characters/characters.png");
	if (!portraits)
		return 2;
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(X_SCREEN/2 -125, Y_SCREEN/2 -75, X_SCREEN /2 -5, Y_SCREEN / 2 + 55, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(X_SCREEN/2 +125, Y_SCREEN/2 -75, X_SCREEN /2 +5, Y_SCREEN / 2 + 55, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(X_SCREEN/2 -125, Y_SCREEN/2 +65, X_SCREEN /2 -5, Y_SCREEN / 2 + 195, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(X_SCREEN/2 +125, Y_SCREEN/2 +65, X_SCREEN /2 +5, Y_SCREEN / 2 + 195, al_map_rgb(255, 255, 255));

	al_draw_scaled_bitmap(portraits,
		175, 395,  95, 95, // fonte
		X_SCREEN/2 -115 , Y_SCREEN/2 -55, 120, 120,     // destino
		0);

	al_draw_scaled_bitmap(portraits,
		92, 570,  95, 95, // fonte
		X_SCREEN/2 +5, Y_SCREEN/2 -50, 120, 120,     // destino
		0);

	al_draw_scaled_bitmap(portraits,
		390, 314,  95, 95, // fonte
		X_SCREEN/2 -125, Y_SCREEN/2 +93, 120, 120,     // destino
		0);

	al_draw_scaled_bitmap(portraits,
		80, 481,  92, 92, // fonte
		X_SCREEN/2 +10, Y_SCREEN/2 +88, 115, 115,     // destino
		0);

	al_flip_display();
	char opt1 = 1;
	char opt2 = 2;
	char select1;
	char select2;
	select1 = select2 = 0;

	while (1) {
		if (select1 && select2)
			return 1;
		al_wait_for_event(essentials->queue, &(essentials->event));	

		if ((essentials->event.type == 10)){			
			if (!select1) {
				if (essentials->event.keyboard.keycode == ALLEGRO_KEY_A && (opt1-1) > 0 && (opt1-1) != opt2) opt1 -= 1;
				else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_D && (opt1+1) < 5 && (opt1+1) != opt2) opt1 += 1;
				else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_W && (opt1-2) > 0 && (opt1-2) != opt2) opt1-= 2;
				else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_S && (opt1+2) < 5 && (opt1+2) != opt2) opt1 += 2;
				else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_J) {
					select1 = opt1;
					if (!(*player_1 = character_load (1, opt1))) return 2;
				}
			}
			if (!select2) {
				if (essentials->event.keyboard.keycode == ALLEGRO_KEY_LEFT && (opt2-1) > 0 && (opt2-1) != opt1) opt2 -= 1;
				else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_RIGHT && (opt2+1) < 5 && (opt2+1) != opt1) opt2 += 1;
				else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_UP && (opt2-2) > 0 && (opt2-2) != opt1) opt2 -= 2;
				else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_DOWN && (opt2+2) < 5 && (opt2+2) != opt1) opt2 += 2;
				else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_1) {
					select2 = opt2;
					if (!(*player_2 = character_load (2, opt2))) return 2;
				}
			}
			
			
		}																																			
		else if (essentials->event.type == 42) return 2;


		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, 100, ALLEGRO_ALIGN_CENTRE, "SELECT YOUR CHARACTER");

		al_draw_filled_rectangle(X_SCREEN/2 -125, Y_SCREEN/2 -75, X_SCREEN /2 -5, Y_SCREEN / 2 + 55, al_map_rgb(255, 255, 255));
		al_draw_filled_rectangle(X_SCREEN/2 +125, Y_SCREEN/2 -75, X_SCREEN /2 +5, Y_SCREEN / 2 + 55, al_map_rgb(255, 255, 255));
		al_draw_filled_rectangle(X_SCREEN/2 -125, Y_SCREEN/2 +65, X_SCREEN /2 -5, Y_SCREEN / 2 + 195, al_map_rgb(255, 255, 255));
		al_draw_filled_rectangle(X_SCREEN/2 +125, Y_SCREEN/2 +65, X_SCREEN /2 +5, Y_SCREEN / 2 + 195, al_map_rgb(255, 255, 255));
		
		al_draw_scaled_bitmap(portraits,
			175, 395,  95, 95, // fonte
			X_SCREEN/2 -115 , Y_SCREEN/2 -55, 120, 120,     // destino
			0);

		al_draw_scaled_bitmap(portraits,
			92, 570,  95, 95, // fonte
			X_SCREEN/2 +5, Y_SCREEN/2 -50, 120, 120,     // destino
			0);

		al_draw_scaled_bitmap(portraits,
			390, 314,  95, 95, // fonte
			X_SCREEN/2 -125, Y_SCREEN/2 +93, 120, 120,     // destino
			0);

		al_draw_scaled_bitmap(portraits,
			80, 481,  92, 92, // fonte
			X_SCREEN/2 +10, Y_SCREEN/2 +88, 115, 115,     // destino
			0);
		
		if (opt1 == 1) {
			al_draw_scaled_bitmap(portraits,
				0, 160,  210, 75, // fonte
				X_SCREEN/2 -470 , Y_SCREEN/2, 336, 120,     // destino
				0);
			al_draw_rectangle(X_SCREEN/2 -125, Y_SCREEN/2 -75, X_SCREEN /2 -5, Y_SCREEN / 2 + 55, al_map_rgb(255, 0, 0), 2);
		}
		else if (opt1 == 2) {
			al_draw_scaled_bitmap(portraits,
				213, 160,  210, 75, // fonte
				X_SCREEN/2 -470 , Y_SCREEN/2, 336, 120,     // destino
				0);
			al_draw_rectangle(X_SCREEN/2 +125, Y_SCREEN/2 -75, X_SCREEN /2 +5, Y_SCREEN / 2 + 55, al_map_rgb(255, 0, 0), 2);
		}
		else if (opt1 == 3) {
			al_draw_scaled_bitmap(portraits,
				213, 237,  210, 75, // fonte
				X_SCREEN/2 -470 , Y_SCREEN/2, 336, 120,     // destino
				0);
			al_draw_rectangle(X_SCREEN/2 -125, Y_SCREEN/2 +65, X_SCREEN /2 -5, Y_SCREEN / 2 + 195, al_map_rgb(255, 0, 0), 2);
		}
		else if (opt1 == 4) {
			al_draw_scaled_bitmap(portraits,
				0, 237,  210, 75, // fonte
				X_SCREEN/2 -470 , Y_SCREEN/2, 336, 120,     // destino
				0);
			al_draw_rectangle(X_SCREEN/2 +125, Y_SCREEN/2 +65, X_SCREEN /2 +5, Y_SCREEN / 2 + 195, al_map_rgb(255, 0, 0), 2);
		}

		if (opt2 == 1) {
			al_draw_scaled_bitmap(portraits,
				0, 160,  210, 75, // fonte
				X_SCREEN/2 +135 , Y_SCREEN/2, 336, 120,     // destino
				0);
			al_draw_rectangle(X_SCREEN/2 -125, Y_SCREEN/2 -75, X_SCREEN /2 -5, Y_SCREEN / 2 + 55, al_map_rgb(0, 0, 255), 2);
		}
		else if (opt2 == 2) {
			al_draw_scaled_bitmap(portraits,
				213, 160,  210, 75, // fonte
				X_SCREEN/2 +135 , Y_SCREEN/2, 336, 120,     // destino
				0);
			al_draw_rectangle(X_SCREEN/2 +125, Y_SCREEN/2 -75, X_SCREEN /2 +5, Y_SCREEN / 2 + 55, al_map_rgb(0, 0, 255), 2);
		}
		else if (opt2 == 3) {
			al_draw_scaled_bitmap(portraits,
				213, 237,  210, 75, // fonte
				X_SCREEN/2 +135 , Y_SCREEN/2, 336, 120,     // destino
				0);
			al_draw_rectangle(X_SCREEN/2 -125, Y_SCREEN/2 +65, X_SCREEN /2 -5, Y_SCREEN / 2 + 195, al_map_rgb(0, 0, 255), 2);
		}
		else if (opt2 == 4) {
			al_draw_scaled_bitmap(portraits,
				0, 237,  210, 75, // fonte
				X_SCREEN/2 +135 , Y_SCREEN/2, 336, 120,     // destino
				0);
			al_draw_rectangle(X_SCREEN/2 +125, Y_SCREEN/2 +65, X_SCREEN /2 +5, Y_SCREEN / 2 + 195, al_map_rgb(0, 0, 255), 2);
		}
		if (select1)
			al_draw_rectangle(X_SCREEN/2 -470, Y_SCREEN/2, X_SCREEN /2 -134, Y_SCREEN / 2 + 120, al_map_rgb(255, 255, 255), 2);
		if (select2)
			al_draw_rectangle(X_SCREEN/2 +135, Y_SCREEN/2, X_SCREEN /2 +471, Y_SCREEN / 2 + 120, al_map_rgb(255, 255, 255), 2);
		
		al_flip_display();

	}
	return 1;
}

unsigned char mapSelect (char opt, ALLEGRO_BITMAP **background, unsigned char *background_count)
{
	if (opt > 2)
		return 0;
	if (opt == 0) {
		*background = al_load_bitmap("backgrounds/map1.png");
		*background_count = 8;
		if (!background) return 0;
	}
	else if (opt == 1) {
		*background = al_load_bitmap("backgrounds/map2.png"); 
		*background_count = 8;
		if (!background) return 0;}
	else if (opt == 2) {
		*background = al_load_bitmap("backgrounds/map3.png"); 
		*background_count = 8;
		if (!background) return 0;}
	return 1;
}

int menuMap (ALLEGRO_BITMAP **background, unsigned char *background_count, Essentials *essentials)
{
	ALLEGRO_BITMAP *thumb1 = al_load_bitmap("backgrounds/map1_thumb.png"); 
	if (!thumb1)
		return 2;
	ALLEGRO_BITMAP *thumb2 = al_load_bitmap("backgrounds/map2_thumb.png");
	if (!thumb2)
		return 2;
	ALLEGRO_BITMAP *thumb3 = al_load_bitmap("backgrounds/map3_thumb.png");
	if (!thumb3)
		return 2;


	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_scaled_bitmap(thumb1,
		0, 0, al_get_bitmap_width(thumb1), al_get_bitmap_height(thumb1), // fonte
		X_SCREEN/2 -256/2 -20 -256, Y_SCREEN/2 -90/2, 256, 90,     // destino
		0);

	al_draw_scaled_bitmap(thumb1,
		0, 0, al_get_bitmap_width(thumb2), al_get_bitmap_height(thumb2), // fonte
		X_SCREEN/2 -256/2, Y_SCREEN/2 -90/2, 256, 90,     // destino
		0);

	al_draw_scaled_bitmap(thumb1,
		0, 0, al_get_bitmap_width(thumb3), al_get_bitmap_height(thumb3), // fonte
		X_SCREEN/2 +256/2 +20, Y_SCREEN/2 -90/2, 256, 90,     // destino
		0);		
	al_flip_display();

	char opt = 0;

	while (1) {
		al_wait_for_event(essentials->queue, &(essentials->event));	
		
		if ((essentials->event.type == 10)){
			if (essentials->event.keyboard.keycode == ALLEGRO_KEY_A) menu_up(&opt);
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_D) menu_down(&opt);
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_LEFT) menu_up(&opt);
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_RIGHT) menu_down(&opt);
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_ENTER || essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
				al_destroy_bitmap (thumb1);
				al_destroy_bitmap (thumb2);
				al_destroy_bitmap (thumb3);
				if (!mapSelect (opt, background, background_count)) return 2;
				return 1;
			}
			else if (essentials->event.keyboard.keycode == 3) {
				al_destroy_bitmap (thumb1);
				al_destroy_bitmap (thumb2);
				al_destroy_bitmap (thumb3);
				if (!mapSelect (opt, background, background_count)) return 2;
				return 1;
			}
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_1) {
				al_destroy_bitmap (thumb1);
				al_destroy_bitmap (thumb2);
				al_destroy_bitmap (thumb3);
				if (!mapSelect (opt, background, background_count)) return 2;
				return 1;
			}
		}																																			
		else if (essentials->event.type == 42) {
			al_destroy_bitmap (thumb1);
			al_destroy_bitmap (thumb2);
			al_destroy_bitmap (thumb3);
			return 2;
		}

		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, 100, ALLEGRO_ALIGN_CENTRE, "SELECT MAP");

		if (opt == 0)
			al_draw_rectangle(X_SCREEN/2 -256 -256/2 -20, Y_SCREEN/2 -90/2, X_SCREEN /2 -256/2 -20, Y_SCREEN / 2 + 90/2, al_map_rgb(255, 150, 0), 4);
		if (opt == 1)		
			al_draw_rectangle(X_SCREEN/2 -256/2, Y_SCREEN/2 -90/2, X_SCREEN /2 +256/2, Y_SCREEN / 2 + +90 / 2, al_map_rgb(255, 150, 0), 4);
		if (opt == 2)
			al_draw_rectangle(X_SCREEN/2 + 256/2 +20, Y_SCREEN/2 -90/2, X_SCREEN /2 +256 +256/2 +20, Y_SCREEN / 2 + 90/2, al_map_rgb(255, 150, 0), 4);

		al_draw_scaled_bitmap(thumb1,
			0, 0, al_get_bitmap_width(thumb1), al_get_bitmap_height(thumb1), // fonte
			X_SCREEN/2 -256/2 -20 -256, Y_SCREEN/2 -90/2, 256, 90,     // destino
			0);

		al_draw_scaled_bitmap(thumb2,
			0, 0, al_get_bitmap_width(thumb2), al_get_bitmap_height(thumb2), // fonte
			X_SCREEN/2 -256/2, Y_SCREEN/2 -90/2, 256, 90,     // destino
			0);

		al_draw_scaled_bitmap(thumb3,
			0, 0, al_get_bitmap_width(thumb3), al_get_bitmap_height(thumb3), // fonte
			X_SCREEN/2 +256/2 +20, Y_SCREEN/2 -90/2, 256, 90,     // destino
			0);		
		
		al_flip_display();
	} 
	al_destroy_bitmap (thumb1);
	al_destroy_bitmap (thumb2);
	al_destroy_bitmap (thumb3);
	
	return 1;
}

unsigned char endGameMenu (unsigned char winner, Essentials *essentials) {	
	char opt = 0;
	
	al_clear_to_color(al_map_rgb(0, 0, 0));		
	if (winner == 0) al_draw_text(essentials->font, al_map_rgb(255, 255, 255),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "DRAW!");
	else if (winner == 1) al_draw_text(essentials->font, al_map_rgb(255, 0, 0),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PLAYER 1 WINS!");
	else al_draw_text(essentials->font, al_map_rgb(0, 0, 255),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PLAYER 2 WINS!");
	al_draw_text(essentials->font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
	al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
	
	al_flip_display();	

	while (1) {
		al_wait_for_event(essentials->queue, &(essentials->event));	
		
		if ((essentials->event.type == 10)){			
			if (essentials->event.keyboard.keycode == 23) opt = 0;
			else if (essentials->event.keyboard.keycode == 19 ) opt = 1;
			else if (essentials->event.keyboard.keycode == 84) opt = 0;
			else if (essentials->event.keyboard.keycode == 85) opt = 1;
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_ENTER || essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
				return opt;				
			}
			else if (essentials->event.keyboard.keycode == 3) {
				return opt;
			}
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_1) {
				return opt;
			}

			al_clear_to_color(al_map_rgb(0, 0, 0));		
			if (winner == 0) al_draw_text(essentials->font, al_map_rgb(255, 255, 255),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "DRAW!");
			else if (winner == 1) al_draw_text(essentials->font, al_map_rgb(255, 0, 0),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PLAYER 1 WINS!");
			else al_draw_text(essentials->font, al_map_rgb(0, 0, 255),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PLAYER 2 WINS!");
			
			if (opt == 0) {
				al_draw_text(essentials->font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
				al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
			}
			else {		
				al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
				al_draw_text(essentials->font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
			}
			al_flip_display();				
		}																																			
		else if (essentials->event.type == 42) return 2;
	}
}


int menu_pause (Essentials *essentials)
{
	al_clear_to_color (al_map_rgb (0, 0 ,0));
	al_draw_text(essentials->font, al_map_rgb(255, 255, 255),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PAUSE");
	al_draw_text(essentials->font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
	al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
	al_flip_display();

	char opt = 0;
	while (1) {
		al_wait_for_event(essentials->queue, &(essentials->event));	
		
		if ((essentials->event.type == 10)){			
			if (essentials->event.keyboard.keycode == 23) opt = 0;
			else if (essentials->event.keyboard.keycode == 19 ) opt = 1;
			else if (essentials->event.keyboard.keycode == 84) opt = 0;																													
			else if (essentials->event.keyboard.keycode == 85) opt = 1;
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_ENTER || essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
				return opt;				
			}
			else if (essentials->event.keyboard.keycode == 3) {
				return opt;
			}
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_1) {
				return opt;
			}

			al_clear_to_color (al_map_rgb (0, 0 ,0));
			al_draw_text(essentials->font, al_map_rgb(255, 255, 255),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PAUSE");

			if (opt == 0) {
				al_draw_text(essentials->font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
				al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
			}
			else {		
				al_draw_text(essentials->font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
				al_draw_text(essentials->font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
			}
			al_flip_display();				
		}																																			
		else if (essentials->event.type == 42) return 2;
	}
}