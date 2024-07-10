//Compilação: gcc AggressiveSquares.c Square.c Joystick.c Attacks.c Bullet.c Pistol.c Box.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>																																								//Biblioteca de figuras básicas
#include <stdio.h>
#include "Square.h"																																															//Inclusão da biblioteca de quadrados

#define X_SCREEN 960																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 540





	const char gravity = 2;

	unsigned long long frame = 0;
	unsigned int counter = 99;

unsigned char collision_x (box *element_first, box *element_second) {
	if (((element_first->x-element_first->width/2 >= element_second->x-element_second->width/2) && (element_second->x+element_second->width/2 >= element_first->x-element_second->width/2)) || 
		((element_second->x-element_second->width/2 >= element_first->x-element_first->width/2) && (element_first->x+element_first->width/2 >= element_second->x-element_second->width/2)))	return 1;
	else return 0; 
}																																															//Definição do tamanho da tela em pixels no eixo y

unsigned char collision_2D(box *element_first, box *element_second){																																	//Implementação da função de verificação de colisão entre dois quadrados

	if ((((element_second->y-element_second->height/2 > element_first->y-element_first->height/2) && (element_first->y+element_first->height/2 > element_second->y-element_second->height/2)) || 	//				//Verifica se o primeiro elemento colidiu com o segundo no eixo X 
		((element_first->y-element_first->height/2 >= element_second->y-element_second->height/2) && (element_second->y+element_second->height/2 > element_first->y-element_first->height/2))) && 	//				//Verifica se o segundo elemento colidiu com o primeiro no eixo X 
		(((element_second->x-element_second->width/2 >= element_first->x-element_first->width/2) && (element_first->x+element_first->width/2 >= element_second->x-element_second->width/2)) || 	//				//Verifica se o primeiro elemento colidiu com o segundo no eixo Y
		((element_first->x-element_first->width/2 >= element_second->x-element_second->width/2) && (element_second->x+element_second->width/2 >= element_first->x-element_first->width/2)))) return 1;			//Verifica se o segundo elemento colidiu com o primeiro no eixo Y
	else return 0;																																															//Se as condições não forem satisfeita, não houve colisão
}

unsigned char check_kill(square *killer, square *victim){																																					//Implementação da função que verifica se um projétil acertou um jogador

	bullet *previous = NULL;
	for (bullet *index = killer->gun->shots; index != NULL; index = (bullet*) index->next){																													//Para todos os projéteis do atirador
		if ((index->x >= victim->box->x - victim->box->width/2) && (index->x <= victim->box->x + victim->box->width/2) && //																										//Verique se houve colisão com a vítima no eixo X
		   (index->y >= victim->box->y - victim->box->height/2) && (index->y <= victim->box->y + victim->box->height/2)){																											//Verifique se houve colisão com a vítima no eixo Y
			victim->hp--;																																													//Reduz o HP da vítima em uma unidade (!)
			if (victim->hp > 0){																																												//Verifica se a vítima ainda tem HP (!)
				if (previous){																																												//Verifica se não é o primeiro elemento da lista de projéteis (!)
					previous->next = index->next;																																							//Se não for, salva o próximo projétil (!)
					bullet_destroy(index);																																									//Chama o destrutor para o projétil atual (!)
					index = (bullet*) previous->next;																																						//Atualiza para o próximo projétil (!)
				}
				else {																																					//Se for o primeiro projétil da lista (!)
					killer->gun->shots = (bullet*) index->next;																																				//Atualiza o projétil no início da lista (!)
					bullet_destroy(index);																																									//Chama o destrutor para o projétil atual (!)
					index = killer->gun->shots;																																								//Atualiza para o próximo projétil (!)
				}
				return 0;																																													//A vítima sofreu dano, mas ainda não morre (!)
			}
			else return 1;																																													//A vítima sofreu dano e morreu (!)
		}
		previous = index;																																													//Atualiza a variável de controle do projétil anterior (!)
	}
	return 0;																																																//Se não houver nenhum projétil que acertou a vítima, retorne falso
}

void update_bullets(square *player){																																										//Implementação da função que atualiza o posicionamento de projéteis conforme o movimento dos mesmos
	
	bullet *previous = NULL;																																												//Variável auxiliar para salvar a posição imediatamente anterior na fila
	for (bullet *index = player->gun->shots; index != NULL;){																																				//Para cada projétil presente na lista de projéteis disparados
		if (!index->trajectory) index->x -= BULLET_MOVE;																																					//Se a trajetória for para a esquerda, atualiza a posição para a esquerda
		else if (index->trajectory == 1) index->x += BULLET_MOVE;																																			//Se a trajetória for para a direita, atualiza a posição para a esquerda
		
		if ((index->x < 0) || (index->x > X_SCREEN)){																																						//Verifica se o projétil saiu das bordas da janela
			if (previous){																																													//Verifica se não é o primeiro elemento da lista de projéteis
				previous->next = index->next;																																								//Se não for, salva o próximo projétil
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual
				index = (bullet*) previous->next;																																							//Atualiza para o próximo projétil
			}
			else {																																															//Se for o primeiro projétil da lista
				player->gun->shots = (bullet*) index->next;																																					//Atualiza o projétil no início da lista
				bullet_destroy(index);																																										//Chama o destrutor para o projétil atual
				index = player->gun->shots;																																									//Atualiza para o próximo projétil
			}
		}
		else{																																																//Se não saiu da tela
			previous = index;																																												//Atualiza o projétil anterior (para a próxima iteração)
			index = (bullet*) index->next;																																									//Atualiza para o próximo projétil
		}
	}
}

int attack_move (attacks *attack, square *player_2)
{
	if (collision_2D (attack->attack_area, player_2->box)) {
		player_2->hp -= attack->attack_damage;
		return 1;
	}
	return 0;
}

void update_position(square *player_1, square *player_2){																																					//Função de atualização das posições dos quadrados conforme os comandos do controle
	int x1_diff = player_1->box->x;
	int y1_diff = player_1->box->y;

	int x2_diff = player_2->box->x;
	int y2_diff = player_2->box->y;

	//golpes

	if (player_1->control->punch) {
		if (!player_1->cooldown) {
			if (player_1->jump) {
			player_1->cooldown += player_1->punch->attack_time;
			player_1->punch->action_time = player_1->punch->attack_time;
		}
		else {
			player_1->cooldown += player_1->air_punch->attack_time;
			player_1->air_punch->action_time = player_1->air_punch->attack_time;
			}
		}

		if (player_1->punch->action_time) {
			if (attack_move (player_1->punch, player_2)) {
				player_1->control->punch = 0;
			}	
		}
		if (player_1->air_punch->action_time) {
			if (attack_move (player_1->air_punch, player_2)) {
				player_1->control->punch = 0;
			}
		}

	}

	if (player_1->punch->action_time)
		player_1->punch->action_time--;
	
	if (player_1->air_punch->action_time)
		player_1->air_punch->action_time--;


	if (!player_1->air_punch->action_time && !player_1->punch->action_time)
		player_1->control->punch = 0;

	if (player_1->cooldown)
		player_1->cooldown--;


	// movimentacao
	if (player_1->control->left && player_1->jump && !player_1->cooldown){
		player_1->movSpeed = -5;																																										//Se o botão de movimentação para esquerda do controle do segundo jogador está ativado... (!)
	}
	else if (player_1->control->right && player_1->jump && !player_1->cooldown){ 																																										//Se o botão de movimentação para direita do controle do segundo jogador está ativado... (!)
		player_1->movSpeed = 5;
	}
	else if (player_1->jump) player_1->movSpeed = 0;

	if (player_1->movSpeed > 0) {
		square_move(player_1, player_1->movSpeed, 1, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a esquerda (!)
		if (collision_2D(player_1->box, player_2->box)) {square_move(player_1, -player_1->movSpeed, 1, X_SCREEN, Y_SCREEN); square_move(player_2, player_1->movSpeed, 1, X_SCREEN, Y_SCREEN);}
	}
	else if (player_1->movSpeed < 0) {
		square_move(player_1, -player_1->movSpeed, 0, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a esquerda (!)
		if (collision_2D(player_1->box, player_2->box)) {square_move(player_1, player_1->movSpeed, 0, X_SCREEN, Y_SCREEN); square_move(player_2, -player_1->movSpeed, 0, X_SCREEN, Y_SCREEN);}
	}


	//teste
	if (collision_x (player_1->box, player_2->box)) {
		if ((player_1->box->y + player_1->box->height/ 2 >= player_2->box->y - player_2->box->height/2) && (player_1->box->y + player_1->box->height/ 2 <= player_2->box->y - player_2->box->height/2)) {
			if (player_1->box->x < player_2->box->x) {
				square_move(player_1, 10, 0, X_SCREEN, Y_SCREEN);
				square_move(player_2, 10, 1, X_SCREEN, Y_SCREEN);
				player_2->movSpeed = 15;
			}
			else {
				square_move(player_1, 5, 1, X_SCREEN, Y_SCREEN);
				square_move(player_2, 10, 0, X_SCREEN, Y_SCREEN);
				player_2->movSpeed = -15;
			}
		}
	}


	if (player_1->control->down){
		player_1->box->height = player_1->box->width * PROPORTION/2;
	}
	else {
		if (collision_x (player_1->box, player_2->box)) {
			if ((player_1->box->y + player_1->box->height/ 2 >= player_2->box->y - player_2->box->height/2) && (player_1->box->y + player_1->box->height/ 2 <= player_2->box->y - player_2->box->height/2)) {
				player_1->box->height = player_1->box->width *PROPORTION;
				player_1->box->y = player_2->box->y - player_1->box->height/2 - player_2->box->height/2;
				
			}
			else if ((player_1->box->y - player_1->box->height*2 > player_2->box->y + player_2->box->height/2) || (player_1->box->y - player_1->box->height*2 < player_2->box->y - player_2->box->height/2)) {
					player_1->box->height = player_1->box->width * PROPORTION;
					if (collision_2D(player_1->box, player_2->box)) player_1->box->height = player_1->box->width;
				}
		}
		else
			player_1->box->height = player_1->box->width * PROPORTION;
	}

	if (player_1->control->up && player_1->jump && !player_1->cooldown){
		player_1->vertSpeed = 30;																																											//Se o botão de movimentação para cima do controle do segundo jogador está ativado... (!)
		player_1->jump = 0;
	}



	square_move(player_1, player_1->vertSpeed, 2, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a cima (!)
	if (collision_2D(player_1->box, player_2->box)) {square_move(player_1, -player_1->vertSpeed, 2, X_SCREEN, Y_SCREEN); player_1->vertSpeed = 0;}

	if (collision_x (player_1->box, player_2->box)) {
			if ((player_1->box->y + player_1->box->height/ 2  -player_1->vertSpeed >= player_2->box->y - player_2->box->height/2) && (player_1->box->y + player_1->box->height/ 2 <= player_2->box->y - player_2->box->height/2)) {
				player_1->vertSpeed = 0; 
				player_1->box->y = player_2->box->y - player_1->box->height/2 - player_2->box->height/2;
			}
			else {
				if (!player_1->jump && player_1->box->y + player_1->box->height/ 2 - player_1->vertSpeed + gravity < Y_SCREEN)
					player_1->vertSpeed -= gravity;
				else 
					player_1->box->y = Y_SCREEN - player_1->box->height/ 2 ;
			}
	}
	else {
		if (!player_1->jump && player_1->box->y + player_1->box->height/ 2 - player_1->vertSpeed + gravity < Y_SCREEN)
			player_1->vertSpeed -= gravity;
		else 
			player_1->box->y = Y_SCREEN - player_1->box->height/ 2 ;
	}

	player_1->jump = (( player_1->box->y + player_1->box->height/ 2) >= Y_SCREEN );
	if (player_1->jump) {
		player_1->vertSpeed = 0;
	}

	if (player_1->control->fire){																																											//Verifica se o primeiro jogador está atirando
		if (!player_1->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown
			square_shot(player_1); 																																											//Se não estiver, faz um disparo
			player_1->gun->timer = PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma
		} 
	}

	//diferenca apos movimentacao
	x1_diff = x1_diff - player_1->box->x;
	y1_diff = y1_diff - player_1->box->y;

	x2_diff = x2_diff - player_2->box->x;
	y2_diff = y2_diff - player_2->box->y;


	//move area dos golpes pela diferenca de movimentacao
	player_1->punch->attack_area->x -= x1_diff;
	player_1->punch->attack_area->y -= y1_diff;
	player_1->air_punch->attack_area->x -= x1_diff;
	player_1->air_punch->attack_area->y -= y1_diff;

	player_2->punch->attack_area->x -= x2_diff;
	player_2->punch->attack_area->y -= y2_diff;
	player_2->air_punch->attack_area->x -= x2_diff;
	player_2->air_punch->attack_area->y -= y2_diff;

	if (player_1->box->x < player_2->box->x)
		player_1->face = 1;
	else
		player_1->face = 0;
	//rotaciona area do golpe de acordo com face
	if (player_1->face == 0) {
		player_1->punch->attack_area->x = player_1->box->x - abs (player_1->box->x - player_1->punch->attack_area->x);
		player_1->air_punch->attack_area->x = player_1->box->x - abs (player_1->box->x - player_1->air_punch->attack_area->x);
	}
	else {
		player_1->punch->attack_area->x = player_1->box->x + abs (player_1->box->x - player_1->punch->attack_area->x);
		player_1->air_punch->attack_area->x = player_1->box->x + abs (player_1->box->x - player_1->air_punch->attack_area->x);
	}

	player_1->kick->attack_area->x -= x1_diff;
	player_1->kick->attack_area->y -= y1_diff;


	update_bullets(player_1);																																												//Atualiza os disparos do primeiro jogador
}

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

int menuCharacter (square **player_1, square **player_2,ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp)
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
		
		if (select1 && select2)
			break;

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
	ALLEGRO_BITMAP *image = al_load_bitmap("mario.png");
	if (!image)
		return 1;
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
		}																																			
		else if (event.type == 42) return 2;
			//al_clear_to_color(al_map_rgb(0, 0, 0));
			if (opt == 0) {
				al_draw_text(font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
				al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
			}
			else {		
				al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2, Y_SCREEN/2 + 30, ALLEGRO_ALIGN_CENTRE, "RESUME");
				al_draw_text(font, al_map_rgb(255, 150, 0), X_SCREEN/2, Y_SCREEN/2 + 60, ALLEGRO_ALIGN_CENTRE, "BACK TO MENU");
			}
			al_flip_display();																																			//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)

	}
}

void draw_player (square *player, ALLEGRO_BITMAP *image , ALLEGRO_COLOR color)
{
	int largura_original = al_get_bitmap_width(image);
	int altura_original = al_get_bitmap_height(image);

	  // Definir novas dimensões para a imagem
	int nova_largura = player->box->width;
	int nova_altura = player->box->height;

	al_draw_filled_rectangle(player->box->x-player->box->width/2, player->box->y-player->box->height/2, player->box->x+player->box->width/2, player->box->y+player->box->height/2, color/*al_map_rgb(0, 0, 255)*/);					//Insere o quadrado do segundo jogador na tela
    if (player->face == 1) {
    	al_draw_scaled_bitmap(image,
			0, 0, largura_original, altura_original, // fonte
            player->box->x - player->box->width / 2 *PROPORTION, player->box->y - player->box->height /2, nova_largura*PROPORTION, nova_altura,     // destino
            0);
    }
    else {
		al_draw_scaled_bitmap(image,
			0, 0, largura_original, altura_original, // fonte
            player->box->x + player->box->width / 2 *PROPORTION, player->box->y - player->box->height /2, -nova_largura*PROPORTION, nova_altura,     // destino
            0);
    }

	printf ("%d\n", player->air_punch->action_time);
	if (player->punch->action_time)
		al_draw_filled_rectangle(player->punch->attack_area->x-player->punch->attack_area->width/2, player->punch->attack_area->y-player->punch->attack_area->height/2, player->punch->attack_area->x+player->punch->attack_area->width/2, player->punch->attack_area->y+player->punch->attack_area->height/2, al_map_rgb(255, 255, 255));
	if (player->air_punch->action_time)
		al_draw_filled_rectangle(player->air_punch->attack_area->x-player->air_punch->attack_area->width/2, player->air_punch->attack_area->y-player->air_punch->attack_area->height/2, player->air_punch->attack_area->x+player->air_punch->attack_area->width/2, player->air_punch->attack_area->y+player->air_punch->attack_area->height/2, al_map_rgb(255, 255, 255));
}

void control (ALLEGRO_EVENT event, square *player_1, square *player_2)
{
	if (event.type == 10) {
		if (event.keyboard.keycode == 1) {player_1->face = 0; player_1->control->left = 1;}																															//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda)
		else if (event.keyboard.keycode == 4) {player_1->face = 1; player_1->control->right = 1;}																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)
		else if (event.keyboard.keycode == 23) player_1->control->up = 1;																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima)
		else if (event.keyboard.keycode == 19) player_1->control->down = 1;																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para baixo)
		else if (event.keyboard.keycode == 82) {/*player_2->face = 0;*/ player_2->control->left = 1;}																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à esquerda)
		else if (event.keyboard.keycode == 83) {/*player_2->face = 1;*/ player_2->control->right = 1;}																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à direita)
		else if (event.keyboard.keycode == 84) player_2->control->up = 1;																														//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para cima)
		else if (event.keyboard.keycode == 85) player_2->control->down = 1;																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para baixo)
		else if (event.keyboard.keycode == 3) player_1->control->fire = 1;																														//Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c)					
		else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_3) player_2->control->fire = 1;																									//Indica o evento correspondente no controle do segundo joagdor (botão de disparo - shift dir)
		else if (event.keyboard.keycode == ALLEGRO_KEY_J) player_1->control->punch = 1;
		else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2) player_2->control->punch = 1;
	}
	else if (event.type == 12) {
		if (event.keyboard.keycode == 1) {player_1->control->left = 0;}																															//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda)
		else if (event.keyboard.keycode == 4) {player_1->control->right = 0;}																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)
		else if (event.keyboard.keycode == 23) player_1->control->up = 0;																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima)
		else if (event.keyboard.keycode == 19) player_1->control->down = 0;																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para baixo)
		else if (event.keyboard.keycode == 82) {player_2->control->left = 0;}																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à esquerda)
		else if (event.keyboard.keycode == 83) {player_2->control->right = 0;}																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à direita)
		else if (event.keyboard.keycode == 84) player_2->control->up = 0;																														//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para cima)
		else if (event.keyboard.keycode == 85) player_2->control->down = 0;																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para baixo)
		else if (event.keyboard.keycode == 3) player_1->control->fire = 0;																														//Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c)					
		else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_3) player_2->control->fire = 0;																									//Indica o evento correspondente no controle do segundo joagdor (botão de disparo - shift dir)
		//else if (event.keyboard.keycode == ALLEGRO_KEY_J) player_1->control->punch = 0;
		//else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2) player_2->control->punch = 0;
	}
}

int gameLoop (square *player_1, square *player_2, ALLEGRO_BITMAP *background, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp)
{
	//al_init_image_addon();
	ALLEGRO_BITMAP *image = al_load_bitmap("mario.png");
	if (!image)
		return 1;
	al_convert_mask_to_alpha (image, al_map_rgb(216,40,0));

	unsigned char p1k = 0, p2k = 0;
	unsigned char p1deaths = 0, p2deaths = 0; 
																																											//Verificação de erro na criação do quadrado do segundo jogador
	
	int menu_control;
	while(1){	
		al_wait_for_event(queue, &event);																																									//Função que captura eventos da fila, inserindo os mesmos na variável de eventos
			
		if (p1deaths == 2 || p2deaths == 2){																																													//Verifica se algum jogador foi morto 																																							//Limpe a tela atual para um fundo preto
			if (p1deaths == 2 && p2deaths == 2){ if (!endGameMenu(0, event, timer, queue, font, disp)) break;}																		//Se ambos foram mortos, declare um empate
			else if (p2deaths == 2) {if (!endGameMenu(1, event, timer, queue, font, disp)) break;}																				//Se o segundo jogador morreu, declare o primeiro jogador vencedor
			else if (!endGameMenu(2, event, timer, queue, font, disp))break;
			p1deaths = p2deaths = 0;																			//Indique o modo de conclusão do programa
			al_flip_display();																																												//Atualiza a tela

			if ((event.type == 10) && (event.keyboard.keycode == 75)) break;																																//Espera por um evento de teclado, de clique da tecla de espaço
			else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {}
			else if (event.type == 42) break; 																																								//Finaliza o jogo
		}
		else if (!counter) {
			if (player_1->hp == player_2->hp){player_2->hp = player_1->hp = 0;}
			else if (player_1->hp > player_2->hp) player_2->hp = 0;																				//Se o segundo jogador morreu, declare o primeiro jogador vencedor
			else player_1->hp = 0;
			counter = 10;	
		}
		else{																																																//Se nenhum quadrado morreu
			if (event.type == 30){																																											//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
				frame++;
				update_position(player_1, player_2);
				update_position(player_2, player_1);																																						//Atualiza a posição dos jogadores
				p1k = check_kill(player_2, player_1);																																						//Verifica se o primeiro jogador matou o segundo jogador
				p2k = check_kill(player_1, player_2);
				if (player_1->hp <= 0) p1k = 1;																																						//Verifica se o segundo jogador matou o primeiro jogador
				if (player_2->hp <= 0) p2k = 1;

				if (p1k || p2k) {
					if (p1k) 
						p1deaths++;
					if (p2k)
						p2deaths++;
					p1k = 0;
					p2k = 0;

					square *aux_replace_player = player_1;
					aux_replace_player = square_create(player_1->box->width, 1, player_1->box->width, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																															//Cria o quadrado do primeiro jogador
					square_destroy(player_1);
					player_1 = aux_replace_player;
					if (!player_1) return 1;

					aux_replace_player = player_2;
					aux_replace_player = square_create(player_2->box->width, 0, X_SCREEN - player_2->box->width, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																															//Cria o quadrado do primeiro jogador
					square_destroy(player_2);
					player_2 = aux_replace_player;
					if (!player_2) return 1;		

				}

				al_clear_to_color(al_map_rgb(0, 0, 0));
				
				al_draw_bitmap(background, 0, 0, 0);

				if (frame % 30 == 0)
					counter--;
				else
					al_draw_textf(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, 30, ALLEGRO_ALIGN_CENTER, "%d", counter);
				
				al_draw_filled_rectangle(10, 40, X_SCREEN / 2 - 10 - (X_SCREEN - 20)/ 10 * (5 - player_1->hp), 20, al_map_rgb(255, 0, 0));
				al_draw_rectangle(10, 40, X_SCREEN /2 -10, 20, al_map_rgb (255, 255, 255), 2);
				al_draw_filled_rectangle(X_SCREEN / 2 + 10 + (X_SCREEN - 20) / 10 *(5 - player_2->hp), 40, X_SCREEN -10, 20, al_map_rgb(0, 0, 255));
				al_draw_rectangle(X_SCREEN/2 +10, 40, X_SCREEN -10, 20, al_map_rgb (255, 255, 255), 2);

				draw_player (player_2, image, al_map_rgb(0, 0, 255));
				draw_player (player_1, image, al_map_rgb(255, 0, 0));

			    //al_draw_scaled_bitmap(image,
                //    0, 0, largura_original, altura_original, // fonte
                //    player_1->box->x - player_1->box->width / 2 *PROPORTION, player_1->box->y - player_1->box->height /2, nova_largura*PROPORTION, nova_altura,     // destino
                //    0);
				//draw_player (player_1, al_map_rgb(255, 0, 0));


	    		for (bullet *index = player_1->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));								//Insere as balas existentes disparadas pelo primeiro jogador na tela
	    		if (player_1->gun->timer) player_1->gun->timer--;																																			//Atualiza o cooldown da arma do primeiro jogador
	    		for (bullet *index = player_2->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(0, 0, 255));								//Insere as balas existentes disparadas pelo segundo jogador na tela
	    		if (player_2->gun->timer) player_2->gun->timer--;
	    		al_flip_display();																																											//Insere as modificações realizadas nos buffers de tela
			}
			if (event.type == 10 && event.keyboard.keycode == ALLEGRO_KEY_P){
				if ( (menu_control = menu_pause (event, timer, queue, font, disp)) == 2) {
					square_destroy(player_1);																																												//Destrutor do quadrado do primeiro jogador
					square_destroy(player_2);
					return 0;
				}
				if (menu_control == 1) {
					square_destroy(player_1);																																												//Destrutor do quadrado do primeiro jogador
					square_destroy(player_2);
					return 1;
				}
			}																		
			else if (event.type == 42) return 0;
			control (event, player_1, player_2);																																								//Evento de clique no "X" de fechamento da tela. Encerra o programa graciosamente.
		}
	}

	return 1;


}

int main(){
	al_init();																																												//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																																												//Faz a inicialização dos addons das imagens básicas
	al_init_image_addon();
	al_install_keyboard();																																													//Habilita a entrada via teclado (eventos de teclado), no programa

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);																																						//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																					//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	ALLEGRO_FONT* font = al_create_builtin_font();																																							//Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);
	al_register_event_source(queue, al_get_keyboard_event_source());																																		//Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));																																		//Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));																																		//Indica que eventos de relógio serão inseridos na nossa fila de eventos
	square* player_1;// = square_create(30, 1, 50, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																															//Cria o quadrado do primeiro jogadorrado do primeiro jogador
	square* player_2;// = square_create(30, 0, X_SCREEN - 50, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																													//Cria o quadrado do segundo jogador
	ALLEGRO_EVENT event;																																													//Variável que guarda um evento capturado, sua estrutura é definida em: https:		//www.allegro.cc/manual/5/ALLEGRO_EVENT
	al_start_timer(timer);																																													//Função que inicializa o relógio do programa
    int menu_control = 1;

	ALLEGRO_BITMAP *background;

	while(1){	
		al_wait_for_event(queue, &event);																																									//Função que captura eventos da fila, inserindo os mesmos na variável de eventos
		
		if (menu_control == 1) {

			if ((menu_control = menu (event, timer, queue, font, disp) == 2)) {
				al_destroy_font(font);																																													//Destrutor da fonte padrão
				al_destroy_display(disp);																																												//Destrutor da tela
				al_destroy_timer(timer);																																												//Destrutor do relógio
				al_destroy_event_queue(queue);																																											//Destrutor da fila
				return 0;
			}
			menuCharacter (&player_1, &player_2, event, timer, queue, font, disp);
			if (menuMap (&background, event, queue, font, disp) == 2) {
				al_destroy_font(font);																																													//Destrutor da fonte padrão
				al_destroy_display(disp);																																												//Destrutor da tela
				al_destroy_timer(timer);																																												//Destrutor do relógio
				al_destroy_event_queue(queue);																																											//Destrutor da fila
				return 0;
			}
		}
 
		if (gameLoop (player_1, player_2, background, event, timer, queue, font, disp) == 1)
			menu_control = 1;
		else
			break;
	}

	al_destroy_font(font);																																													//Destrutor da fonte padrão
	al_destroy_display(disp);																																												//Destrutor da tela
	al_destroy_timer(timer);																																												//Destrutor do relógio
	al_destroy_event_queue(queue);																																											//Destrutor da fila

	return 0;
}