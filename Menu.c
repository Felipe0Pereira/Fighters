#include "Menu.h"

#define X_SCREEN 960																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 540

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

unsigned char options (ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp )
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/4 -50, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PLAYER 1");
	al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/4 - 100, Y_SCREEN/2 +10, ALLEGRO_ALIGN_LEFT, "MOVE LEFT: A");
	al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/4 - 100, Y_SCREEN/2 +20, ALLEGRO_ALIGN_LEFT, "MOVE RIGHT: D");
	al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/4 - 100, Y_SCREEN/2 +30, ALLEGRO_ALIGN_LEFT, "JUMP: W");
	al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/4 - 100, Y_SCREEN/2 +40, ALLEGRO_ALIGN_LEFT, "ATTACK: C");


	al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN - X_SCREEN/4 +50, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PLAYER 2");
	al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN - X_SCREEN/4 , Y_SCREEN/2 +10, ALLEGRO_ALIGN_LEFT, "MOVE LEFT: LEFT ARROW");
	al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN - X_SCREEN/4 , Y_SCREEN/2 +20, ALLEGRO_ALIGN_LEFT, "MOVE RIGHT: RIGHT ARROW");
	al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN - X_SCREEN/4 , Y_SCREEN/2 +30, ALLEGRO_ALIGN_LEFT, "JUMP: UP ARROW");
	al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN - X_SCREEN/4 , Y_SCREEN/2 +40, ALLEGRO_ALIGN_LEFT, "ATTACK: NUM PAD 1");		

	al_draw_text(font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 +200 , ALLEGRO_ALIGN_CENTRE, "BACK");																									//Indique o modo de conclusão do programa
	al_flip_display();
 		
	while (1) {
		al_wait_for_event(queue, &event);																																												//Atualiza a tela
		if ((event.type == 10)){									
			if (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) return 1;
			else if (event.keyboard.keycode == 3) return 1;																								//Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c)					
			else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1) return 1;																				//Indica o evento correspondente no controle do segundo joagdor (botão de disparo - shift dir)
		}																																			
		else if (event.type == 42) return 0;
	}
	return 0;
}

unsigned char menu (ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp )
{
	char opt = 0;
	while (1) {
		al_wait_for_event(queue, &event);	

		al_clear_to_color(al_map_rgb(0, 0, 0));
		if (opt == 0)
			al_draw_text(font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PLAY");
		else		
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PLAY");
		if (opt == 1)
			al_draw_text(font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "CONTROLS");
		else
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "CONTROLS");
		if (opt == 2)
			al_draw_text(font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "EXIT");
		else
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "EXIT");																											//Indique o modo de conclusão do programa
		al_flip_display();
 		
		if ((event.type == 10)){																																						//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)
			if (event.keyboard.keycode == 23) menu_up(&opt);																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima)
			else if (event.keyboard.keycode == 19) menu_down(&opt);																														//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à direita)
			else if (event.keyboard.keycode == 84) menu_up(&opt);																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para cima)
			else if (event.keyboard.keycode == 85) menu_down(&opt);
			else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
				if (opt == 0) return 1;
				else if (opt == 1) {if (!options (event, timer, queue, font, disp)) return 0;}
				else if (opt == 2) return 2;																												//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para baixo)
			}
			else if (event.keyboard.keycode == 3) {
				if (opt == 0) return 1;
				else if (opt == 1) {if (!options (event, timer, queue, font, disp)) return 0;}
				else if (opt == 2) return 2;																											//Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c)					
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1) {
				if (opt == 0) return 1;
				else if (opt == 1) {if (!options (event, timer, queue, font, disp)) return 0;}
				else if (opt == 2) return 2;
			}
		}																																			
		else if (event.type == 42) return 0;
	}
}

int menuCharacter (square **player_1, square **player_2, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp)
{
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(X_SCREEN/2 -190, Y_SCREEN/2 -60, X_SCREEN /2 -100, Y_SCREEN / 2 + 60, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(X_SCREEN/2 +190, Y_SCREEN/2 -60, X_SCREEN /2 +100, Y_SCREEN / 2 + 60, al_map_rgb(0, 0, 255));
	al_flip_display();
	//al_draw_text(font, al_map_rgb(255, 255, 255),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PAUSE");
	char opt1 = 1;
	char opt2 = 2;
	char select1;
	char select2;
	select1 = select2 = 0;

	while (1) {
		al_wait_for_event(queue, &event);	
		
		if (select1 && select2) {
			(*player_1)->sprites[0] = al_load_bitmap("walk-monk.png");
			(*player_1)->sprites[1] = (*player_1)->sprites[0];
			(*player_1)->sprites[2] = al_load_bitmap("punch-monk.png");
			(*player_2)->sprites[0] = al_load_bitmap("walk-monk2.png");
			(*player_2)->sprites[1] = (*player_2)->sprites[0];
			(*player_2)->sprites[2] = al_load_bitmap("punch-monk2.png");
			
			break;
		}

		if ((event.type == 10)){			
			if (event.keyboard.keycode == ALLEGRO_KEY_A && !select1) opt1 = 1;
			else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT && !select2) opt2 = 1;
			else if (event.keyboard.keycode == ALLEGRO_KEY_D && !select1) opt1 = 2;																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para cima)
			else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && !select2) opt2 = 2;
			else if (event.keyboard.keycode == ALLEGRO_KEY_J && opt1 != select2) {
				select1 = opt1;
				if (opt1 == 1) *player_1 = square_create(20, 1, 100, Y_SCREEN/2, X_SCREEN, Y_SCREEN);
				else if (opt1 == 2) *player_1 = square_create(20, 1, 100, Y_SCREEN/2, X_SCREEN, Y_SCREEN);
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1 && opt2 != select1) {
				select2 = opt2;
				if (opt2 == 1) *player_2 = square_create(20, 0, X_SCREEN - 100, Y_SCREEN/2, X_SCREEN, Y_SCREEN);	
				else if (opt2 == 2) *player_2 = square_create(20, 0, X_SCREEN - 100, Y_SCREEN/2, X_SCREEN, Y_SCREEN);	
			}
		}																																			
		else if (event.type == 42) return 2;


		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_filled_rectangle(X_SCREEN/2 -190, Y_SCREEN/2 -60, X_SCREEN /2 -100, Y_SCREEN / 2 + 60, al_map_rgb(255, 0, 0));
		al_draw_filled_rectangle(X_SCREEN/2 +190, Y_SCREEN/2 -60, X_SCREEN /2 +100, Y_SCREEN / 2 + 60, al_map_rgb(0, 0, 255));
		//al_draw_filled_rectangle(X_SCREEN /2 + 100, Y_SCREEN /2, 50, 100, al_map_rgb(0, 0, 255));
		
		if (opt1 == 1)
			al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 145, Y_SCREEN/2 -80, ALLEGRO_ALIGN_CENTRE, "PLAYER 1");
		else
			al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 + 145, Y_SCREEN/2 - 80, ALLEGRO_ALIGN_CENTRE, "PLAYER 1");
		if (opt2 == 1)		
			al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN/2 - 145, Y_SCREEN/2 + 80, ALLEGRO_ALIGN_CENTRE, "PLAYER 2");
		else
			al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN/2 + 145, Y_SCREEN/2 + 80, ALLEGRO_ALIGN_CENTRE, "PLAYER 2");
	
		al_flip_display();																																			//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)

	}
	return 1;
}

unsigned char mapSelect (char opt, ALLEGRO_BITMAP **background)
{
	if (opt > 2)
		return 0;
	if (opt == 0) {*background = al_load_bitmap("map1.jpg"); if (!background) return 0;}
	else if (opt == 1) {*background = al_load_bitmap("map1.jpg"); if (!background) return 0;}
	else if (opt == 2) {*background = al_load_bitmap("map1.jpg"); if (!background) return 0;}
	return 1;
}

int menuMap (ALLEGRO_BITMAP **background, ALLEGRO_EVENT event, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp)
{
	//==================================================================================================================================

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(X_SCREEN/2 -300, Y_SCREEN/2 -60, X_SCREEN /2 -200, Y_SCREEN / 2 + 60, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(X_SCREEN/2 -50, Y_SCREEN/2 -60, X_SCREEN /2 +50, Y_SCREEN / 2 + 60, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(X_SCREEN/2 +300, Y_SCREEN/2 -60, X_SCREEN /2 +200, Y_SCREEN / 2 + 60, al_map_rgb(0, 0, 255));
	al_flip_display();

	char opt = 0;

	while (1) {
		al_wait_for_event(queue, &event);	
		
		if ((event.type == 10)){
			if (event.keyboard.keycode == ALLEGRO_KEY_A) menu_up(&opt);																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima)
			else if (event.keyboard.keycode == ALLEGRO_KEY_D) menu_down(&opt);																														//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à direita)
			else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) menu_up(&opt);																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para cima)
			else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) menu_down(&opt);
			else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
				if (!mapSelect (opt, background)) return 2;
				return 1;
			}
			else if (event.keyboard.keycode == 3) {
				if (!mapSelect (opt, background)) return 2;
				return 1;
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1) {
				if (!mapSelect (opt, background)) return 2;
				return 1;
			}
		}																																			
		else if (event.type == 42) return 2;

		al_clear_to_color(al_map_rgb(0, 0, 0));

		if (opt == 0)
			al_draw_filled_rectangle(X_SCREEN/2 -310, Y_SCREEN/2 -65, X_SCREEN /2 -190, Y_SCREEN / 2 + 65, al_map_rgb(255, 150, 0));
		if (opt == 1)		
			al_draw_filled_rectangle(X_SCREEN/2 -55, Y_SCREEN/2 -65, X_SCREEN /2 +55, Y_SCREEN / 2 + 65, al_map_rgb(255, 150, 0));
		if (opt == 2)
			al_draw_filled_rectangle(X_SCREEN/2 +310, Y_SCREEN/2 -65, X_SCREEN /2 +190, Y_SCREEN / 2 + 65, al_map_rgb(255, 150, 0));
		
		al_draw_filled_rectangle(X_SCREEN/2 -300, Y_SCREEN/2 -60, X_SCREEN /2 -200, Y_SCREEN / 2 + 60, al_map_rgb(255, 0, 0));
		al_draw_filled_rectangle(X_SCREEN/2 -50, Y_SCREEN/2 -60, X_SCREEN /2 +50, Y_SCREEN / 2 + 60, al_map_rgb(255, 255, 255));
		al_draw_filled_rectangle(X_SCREEN/2 +300, Y_SCREEN/2 -60, X_SCREEN /2 +200, Y_SCREEN / 2 + 60, al_map_rgb(0, 0, 255));

	
		al_flip_display();																																			//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)

	} 
	return 1;
}

unsigned char endGameMenu (unsigned char winner, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp) {
	al_clear_to_color(al_map_rgb(0, 0, 0));		
	if (winner == 0) al_draw_text(font, al_map_rgb(255, 255, 255),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "EMPATE!");																					//Se ambos foram mortos, declare um empate
	else if (winner == 1) al_draw_text(font, al_map_rgb(255, 0, 0),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "JOGADOR 1 GANHOU!");																				//Se o segundo jogador morreu, declare o primeiro jogador vencedor
	else al_draw_text(font, al_map_rgb(0, 0, 255),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "JOGADOR 2 GANHOU!");
	al_draw_text(font, al_map_rgb(255, 255, 255),  X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "PRESSIONE ENTER PARA JOGAR NOVAMENTE");																				//Se o primeiro jogador morreu, declare o segundo jogador vencedor
	al_draw_text(font, al_map_rgb(255, 255, 255),  X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "PRESSIONE ESPAÇO PARA SAIR");
	al_flip_display();
	while (1) {
		al_wait_for_event(queue, &event);

		if ((event.type == 10) && (event.keyboard.keycode == 75)) return 0;																																//Espera por um evento de teclado, de clique da tecla de espaço
		else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) return 1;
		else if (event.type == 42) return 0;
	}
}

int menu_pause (ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp)
{
	al_draw_text(font, al_map_rgb(255, 255, 255),  X_SCREEN/2, Y_SCREEN/2, ALLEGRO_ALIGN_CENTRE, "PAUSE");
	al_draw_text(font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
	al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
	al_flip_display();

	char opt = 0;
	while (1) {
		al_wait_for_event(queue, &event);	
		
		if ((event.type == 10)){			
			if (event.keyboard.keycode == 23) opt = 0;																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima)
			else if (event.keyboard.keycode == 19 ) opt = 1;																														//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à direita)
			else if (event.keyboard.keycode == 84) opt = 0;																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para cima)
			else if (event.keyboard.keycode == 85) opt = 1;
			else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
				return opt;				
			}
			else if (event.keyboard.keycode == 3) {
				return opt;
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1) {
				return opt;
			}

			if (opt == 0) {
				al_draw_text(font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
				al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
			}
			else {		
				al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
				al_draw_text(font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
			}
			al_flip_display();				
		}																																			
		else if (event.type == 42) return 2;
		
																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)
	}
}