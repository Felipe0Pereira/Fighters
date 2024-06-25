//Compilação: gcc AggressiveSquares.c Square.c Joystick.c Attacks.c Bullet.c Pistol.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>																																									//Biblioteca de figuras básicas
#include <stdio.h>
#include "Square.h"																																															//Inclusão da biblioteca de quadrados

#define X_SCREEN 960																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 540

	char p1movSpeed = 5, p1vertSpeed = 0;
	char p2movSpeed = 5, p2vertSpeed = 0;



	const char gravity = 2;
	char p1jump = 0, p2jump = 0;
	char counter = 0;
																																															//Definição do tamanho da tela em pixels no eixo y

unsigned char collision_2D(square *element_first, square *element_second){																																	//Implementação da função de verificação de colisão entre dois quadrados

	if ((((element_second->y-element_second->height/2 > element_first->y-element_first->height/2) && (element_first->y+element_first->height/2 > element_second->y-element_second->height/2)) || 	//				//Verifica se o primeiro elemento colidiu com o segundo no eixo X 
		((element_first->y-element_first->height/2 >= element_second->y-element_second->height/2) && (element_second->y+element_second->height/2 > element_first->y-element_first->height/2))) && 	//				//Verifica se o segundo elemento colidiu com o primeiro no eixo X 
		(((element_second->x-element_second->side/2 >= element_first->x-element_first->side/2) && (element_first->x+element_first->side/2 >= element_second->x-element_second->side/2)) || 	//				//Verifica se o primeiro elemento colidiu com o segundo no eixo Y
		((element_first->x-element_first->side/2 >= element_second->x-element_second->side/2) && (element_second->x+element_second->side/2 >= element_first->x-element_first->side/2)))) return 1;			//Verifica se o segundo elemento colidiu com o primeiro no eixo Y
	else return 0;																																															//Se as condições não forem satisfeita, não houve colisão
}

unsigned char check_kill(square *killer, square *victim){																																					//Implementação da função que verifica se um projétil acertou um jogador

	bullet *previous = NULL;
	for (bullet *index = killer->gun->shots; index != NULL; index = (bullet*) index->next){																													//Para todos os projéteis do atirador
		if ((index->x >= victim->x - victim->side/2) && (index->x <= victim->x + victim->side/2) && //																										//Verique se houve colisão com a vítima no eixo X
		   (index->y >= victim->y - victim->height/2) && (index->y <= victim->y + victim->height/2)){																											//Verifique se houve colisão com a vítima no eixo Y
			victim->hp--;																																													//Reduz o HP da vítima em uma unidade (!)
			if (victim->hp){																																												//Verifica se a vítima ainda tem HP (!)
				if (previous){																																												//Verifica se não é o primeiro elemento da lista de projéteis (!)
					previous->next = index->next;																																							//Se não for, salva o próximo projétil (!)
					bullet_destroy(index);																																									//Chama o destrutor para o projétil atual (!)
					index = (bullet*) previous->next;																																						//Atualiza para o próximo projétil (!)
				}
				else {																																														//Se for o primeiro projétil da lista (!)
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

void update_position(square *player_1, square *player_2){																																					//Função de atualização das posições dos quadrados conforme os comandos do controle

	if (player_1->control->left){																																										//Se o botão de movimentação para esquerda do controle do segundo jogador está ativado... (!)
		square_move(player_1, p1movSpeed, 0, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a esquerda (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -p1movSpeed, 0, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	
	if (player_1->control->right){ 																																										//Se o botão de movimentação para direita do controle do segundo jogador está ativado... (!)
		square_move(player_1, p1movSpeed, 1, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a direita (!)
		if (collision_2D(player_1, player_2)) square_move(player_1, -p1movSpeed, 1, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	

	if (player_1->control->down){
		player_1->height = player_1->side;
	//	if (collision_2D(player_1, player_2)) player_2->height = player_2->side *2;
	}
	else {
		if (((player_1->x-player_1->side/2 >= player_2->x-player_2->side/2) && (player_2->x+player_2->side/2 >= player_1->x-player_2->side/2)) || 
			((player_2->x-player_2->side/2 >= player_1->x-player_1->side/2) && (player_1->x+player_1->side/2 >= player_2->x-player_2->side/2))) {
			if ((player_1->y + player_1->height/ 2 >= player_2->y - player_2->height/2) && (player_1->y + player_1->height/ 2 <= player_2->y - player_2->height/2)) {
				player_1->height = player_1->side *2;
				player_1->y = player_2->y - player_1->height/2 - player_2->height/2;
				
			}
			else if ((player_1->y - player_1->height*2 > player_2->y + player_2->height/2) || (player_1->y - player_1->height*2 < player_2->y - player_2->height/2)) {
					player_1->height = player_1->side *2;
					if (collision_2D(player_1, player_2)) player_1->height = player_1->side;
				}
		}
		else
			player_1->height = player_1->side *2;
	}

	if (player_1->control->up && p1jump){
		p1vertSpeed = 20;																																											//Se o botão de movimentação para cima do controle do segundo jogador está ativado... (!)
		p1jump = 0;
	}

	square_move(player_1, p1vertSpeed, 2, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a cima (!)
	if (collision_2D(player_1, player_2)) {square_move(player_1, -p1vertSpeed, 2, X_SCREEN, Y_SCREEN); p1vertSpeed = 0;}

	if (((player_1->x-player_1->side/2 >= player_2->x-player_2->side/2) && (player_2->x+player_2->side/2 >= player_1->x-player_2->side/2)) || 
		((player_2->x-player_2->side/2 >= player_1->x-player_1->side/2) && (player_1->x+player_1->side/2 >= player_2->x-player_2->side/2))) {
			if ((player_1->y + player_1->height/ 2  -p1vertSpeed >= player_2->y - player_2->height/2) && (player_1->y + player_1->height/ 2 <= player_2->y - player_2->height/2)) {
				p1vertSpeed = 0; 
				player_1->y = player_2->y - player_1->height/2 - player_2->height/2;
			}
			else {
				if (!p1jump && player_1->y + player_1->height/ 2 - p1vertSpeed + gravity < Y_SCREEN)
					p1vertSpeed -= gravity;
				else 
					player_1->y = Y_SCREEN - player_1->height/ 2 ;
			}
	}
	else {
		if (!p1jump && player_1->y + player_1->height/ 2 - p1vertSpeed + gravity < Y_SCREEN)
			p1vertSpeed -= gravity;
		else 
			player_1->y = Y_SCREEN - player_1->height/ 2 ;
	}




	if (player_2->control->left){																																										//Se o botão de movimentação para esquerda do controle do segundo jogador está ativado... (!)
		square_move(player_2, p2movSpeed, 0, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a esquerda (!)
		if (collision_2D(player_2, player_1)) square_move(player_2, -p2movSpeed, 0, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}
	
	if (player_2->control->right){ 																																										//Se o botão de movimentação para direita do controle do segundo jogador está ativado... (!)
		square_move(player_2, p2movSpeed, 1, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a direita (!)
		if (collision_2D(player_2, player_1)) square_move(player_2, -p2movSpeed, 1, X_SCREEN, Y_SCREEN);																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)
	}

	if (player_2->control->down){
		player_2->height = player_2->side;
	//	if (collision_2D(player_1, player_2)) player_2->height = player_2->side *2;
	}
	else {
		if (((player_1->x-player_1->side/2 >= player_2->x-player_2->side/2) && (player_2->x+player_2->side/2 >= player_1->x-player_2->side/2)) || 
			((player_2->x-player_2->side/2 >= player_1->x-player_1->side/2) && (player_1->x+player_1->side/2 >= player_2->x-player_2->side/2))) {
			if ((player_2->y + player_2->height/ 2 >= player_1->y - player_1->height/2) && (player_2->y + player_2->height/ 2 <= player_1->y - player_1->height/2)) {
				player_2->height = player_2->side *2;
				player_2->y = player_1->y - player_2->height/2 - player_1->height/2;
				
			}
			else if ((player_2->y - player_2->height*2 > player_1->y + player_1->height/2) || (player_2->y - player_2->height*2 < player_1->y - player_1->height/2)) {
				player_2->height = player_2->side*2;
				if (collision_2D(player_1, player_2)) player_2->height = player_2->side;
			}
		}
		else 
			player_2->height = player_2->side *2;
		
	}

	if (player_2->control->up && p2jump ){								
		p2vertSpeed = 20;																																			//Se o botão de movimentação para cima do controle do segundo jogador está ativado... (!)																										//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)			
		p2jump = 0;
	}
    square_move(player_2, p2vertSpeed, 2, X_SCREEN, Y_SCREEN);																																				//Move o quadrado do segundo jogador para a cima (!)
	if (collision_2D(player_2, player_1)) {square_move(player_2, -p2vertSpeed, 2, X_SCREEN, Y_SCREEN); p2vertSpeed = 0;}																											//Se o movimento causou uma colisão entre quadrados, desfaça o mesmo (!)			

	p1jump = (( player_1->y + player_1->height/ 2) >= Y_SCREEN );
	if (p1jump) {
		p1vertSpeed = 0;
	}

	if (player_1->control->fire){																																											//Verifica se o primeiro jogador está atirando
		if (!player_1->gun->timer){																																											//Verifica se a arma do primeiro jogador não está em cooldown
			square_shot(player_1); 																																											//Se não estiver, faz um disparo
			player_1->gun->timer = PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma
		} 
	}

	

	if (((player_1->x-player_1->side/2 >= player_2->x-player_2->side/2) && (player_2->x+player_2->side/2 >= player_1->x-player_2->side/2)) || 
	((player_2->x-player_2->side/2 >= player_1->x-player_1->side/2) && (player_1->x+player_1->side/2 >= player_2->x-player_2->side/2))) {


		if ((player_2->y + player_2->height/ 2  -p2vertSpeed >= player_1->y - player_1->height/2) && (player_2->y + player_2->height/ 2 <= player_1->y - player_1->height/2)) {
			p2vertSpeed = 0; 
			player_2->y = player_1->y - player_2->height/2 - player_1->height/2;
		}
		else {
			if (!p2jump && player_2->y + player_2->height/ 2 - p2vertSpeed + gravity < Y_SCREEN)
				p2vertSpeed -= gravity;
			else 
				player_2->y = Y_SCREEN - player_2->height/2 ;
		}
	}
	else {
		if (player_2->control->down)
			player_2->height = player_2->side;
		else
			player_2->height = player_2->side*2;
		if (!p2jump && player_2->y + player_2->height/ 2 - p2vertSpeed + gravity < Y_SCREEN)
			p2vertSpeed -= gravity;
		else 
			player_2->y = Y_SCREEN - player_2->height/ 2 ;
	}

	p2jump = (( player_2->y + player_2->height/ 2) >= Y_SCREEN );
	if (p2jump) {
		p2vertSpeed = 0;
	}

	if (player_2->control->fire){																																											//Verifica se o segundo jogador está atirando
		if (!player_2->gun->timer){																																											//Verifica se a arma do segundo jogador não está em cooldown
			square_shot(player_2);																																											//Se não estiver, faz um disparo
			player_2->gun->timer = PISTOL_COOLDOWN;																																							//Inicia o cooldown da arma
		}
	}
	update_bullets(player_1);																																												//Atualiza os disparos do primeiro jogador
	update_bullets(player_2);																																												//Atualiza os disparos do segundo jogador
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
				else if (opt == 2) return 0;																												//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para baixo)
			}
			else if (event.keyboard.keycode == 3) {
				if (opt == 0) return 1;
				else if (opt == 1) {if (!options (event, timer, queue, font, disp)) return 0;}
				else if (opt == 2) return 0;																											//Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c)					
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1) {
				if (opt == 0) return 1;
				else if (opt == 1) {if (!options (event, timer, queue, font, disp)) return 0;}
				else if (opt == 2) return 0;
			}
		}																																			
		else if (event.type == 42) return 0;
	}
}

unsigned char endGameMenu (unsigned char winner, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_DISPLAY* disp) {
	al_clear_to_color(al_map_rgb(0, 0, 0));		
	printf ("DEU O CARAI\n");																																					//Limpe a tela atual para um fundo preto
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

int main(){

	al_init();																																																//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																																												//Faz a inicialização dos addons das imagens básicas
	
	al_install_keyboard();																																													//Habilita a entrada via teclado (eventos de teclado), no programa

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);																																						//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																																					//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	ALLEGRO_FONT* font = al_create_builtin_font();																																							//Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);																																			//Cria uma janela para o programa, define a largura (x) e a altura (y) da tela em píxeis (320x320, neste caso)

	al_register_event_source(queue, al_get_keyboard_event_source());																																		//Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));																																		//Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));																																		//Indica que eventos de relógio serão inseridos na nossa fila de eventos

	square* player_1 = square_create(30, 1, 50, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																															//Cria o quadrado do primeiro jogador
	if (!player_1) return 1;																																												//Verificação de erro na criação do quadrado do primeiro jogador
	square* player_2 = square_create(30, 0, X_SCREEN-50, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																													//Cria o quadrado do segundo jogador
	if (!player_2) return 2;																																												//Verificação de erro na criação do quadrado do segundo jogador

	ALLEGRO_EVENT event;																																													//Variável que guarda um evento capturado, sua estrutura é definida em: https:		//www.allegro.cc/manual/5/ALLEGRO_EVENT
	al_start_timer(timer);																																													//Função que inicializa o relógio do programa
	unsigned char p1k = 0, p2k = 0;
	if (!menu (event, timer, queue, font, disp)) {
		al_destroy_font(font);																																													//Destrutor da fonte padrão
		al_destroy_display(disp);																																												//Destrutor da tela
		al_destroy_timer(timer);																																												//Destrutor do relógio
		al_destroy_event_queue(queue);																																											//Destrutor da fila
		square_destroy(player_1);																																												//Destrutor do quadrado do primeiro jogador
		square_destroy(player_2);	
		return 0;																																											//Variáveis de controle de vida dos quadrados (jogadores)
	}

	unsigned char p1deaths = 0, p2deaths = 0; 
	while(1){																																																//Laço servidor do jogo
		al_wait_for_event(queue, &event);																																									//Função que captura eventos da fila, inserindo os mesmos na variável de eventos
		
		printf ("%d\n", p1deaths);
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
		else{																																																//Se nenhum quadrado morreu
			if (event.type == 30){																																											//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
				update_position(player_1, player_2);																																						//Atualiza a posição dos jogadores
				p1k = check_kill(player_2, player_1);																																						//Verifica se o primeiro jogador matou o segundo jogador
				p2k = check_kill(player_1, player_2);																																						//Verifica se o segundo jogador matou o primeiro jogador
				if (p1k || p2k) {
					if (p1k) 
						p1deaths++;
					else
						p2deaths++;
					p1k = 0;
					p2k = 0;
					
					pistol_destroy(player_1->gun);
					pistol_destroy(player_2->gun);	

					player_1->gun = pistol_create();	
					player_2->gun = pistol_create();						
					player_1->hp = 5;
					player_2->hp = 5;

					player_1->x = 50;
					player_2->x = X_SCREEN -50;


					player_1->y = Y_SCREEN-200;
					player_2->y = Y_SCREEN-200;
					//square_destroy(player_1);																																												//Destrutor do quadrado do primeiro jogador
					//square_destroy(player_2);		

					//player_1 = square_create(30, 1, 50, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																															//Cria o quadrado do primeiro jogador
					//if (!player_1) return 1;																																												//Verificação de erro na criação do quadrado do primeiro jogador
					//player_2 = square_create(30, 0, X_SCREEN-50, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																													//Cria o quadrado do segundo jogador
					//if (!player_2) return 2;

				}

				al_clear_to_color(al_map_rgb(0, 0, 0));
				//pos x,y | tam x,y
				al_draw_filled_rectangle(10, 40, X_SCREEN / 2 - 10 - (X_SCREEN - 20)/ 10 * (5 - player_1->hp), 20, al_map_rgb(255, 0, 0));
				al_draw_filled_rectangle(X_SCREEN / 2 + 10 + (X_SCREEN - 20) / 10 *(5 - player_2->hp), 40, X_SCREEN -10, 20, al_map_rgb(0, 0, 255));																																						//Substitui tudo que estava desenhado na tela por um fundo preto
				al_draw_filled_rectangle(player_1->x-player_1->side/2, player_1->y-player_1->height/2, player_1->x+player_1->side/2, player_1->y+player_1->height/2, al_map_rgb(255, 0, 0));					//Insere o quadrado do primeiro jogador na tela
				al_draw_filled_rectangle(player_2->x-player_2->side/2, player_2->y-player_2->height/2, player_2->x+player_2->side/2, player_2->y+player_2->height/2, al_map_rgb(0, 0, 255));					//Insere o quadrado do segundo jogador na tela
	    		for (bullet *index = player_1->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));								//Insere as balas existentes disparadas pelo primeiro jogador na tela
	    		if (player_1->gun->timer) player_1->gun->timer--;																																			//Atualiza o cooldown da arma do primeiro jogador
	    		for (bullet *index = player_2->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(0, 0, 255));								//Insere as balas existentes disparadas pelo segundo jogador na tela
	    		if (player_2->gun->timer) player_2->gun->timer--; 																																			//Atualiza o cooldown da arma do segundo jogador
	    		al_flip_display();																																											//Insere as modificações realizadas nos buffers de tela
			}
			else if ((event.type == 10) || (event.type == 12)){																																				//Verifica se o evento é de botão do teclado abaixado ou levantado
				if (event.keyboard.keycode == 1) {if (event.type == 10)player_1->face = 0; joystick_left(player_1->control);}																															//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda)
				else if (event.keyboard.keycode == 4) {if (event.type == 10)player_1->face = 1; joystick_right(player_1->control);}																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)
				else if (event.keyboard.keycode == 23) joystick_up(player_1->control);																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima)
				else if (event.keyboard.keycode == 19) joystick_down(player_1->control);																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para baixo)
				else if (event.keyboard.keycode == 82) {if (event.type == 10)player_2->face = 0; joystick_left(player_2->control);}																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à esquerda)
				else if (event.keyboard.keycode == 83) {if (event.type == 10)player_2->face = 1; joystick_right(player_2->control);}																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à direita)
				else if (event.keyboard.keycode == 84) joystick_up(player_2->control);																														//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para cima)
				else if (event.keyboard.keycode == 85) joystick_down(player_2->control);																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para baixo)
				else if (event.keyboard.keycode == 3) joystick_fire(player_1->control);																														//Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c)					
				else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1) joystick_fire(player_2->control);																													//Indica o evento correspondente no controle do segundo joagdor (botão de disparo - shift dir)
			}																																			
			else if (event.type == 42) break;																																								//Evento de clique no "X" de fechamento da tela. Encerra o programa graciosamente.
		}
	}

	al_destroy_font(font);																																													//Destrutor da fonte padrão
	al_destroy_display(disp);																																												//Destrutor da tela
	al_destroy_timer(timer);																																												//Destrutor do relógio
	al_destroy_event_queue(queue);																																											//Destrutor da fila
	square_destroy(player_1);																																												//Destrutor do quadrado do primeiro jogador
	square_destroy(player_2);																																												//Destrutor do quadrado do segundo jogador

	return 0;
}