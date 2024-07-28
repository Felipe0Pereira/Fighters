//Compilação: gcc AggressiveSquares.c Menu.c Essentials.c Square.c Joystick.c Attacks.c Bullet.c Pistol.c Box.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>																																								//Biblioteca de figuras básicas
#include <stdio.h>
#include "Square.h"	
#include "Menu.h"																																								//Inclusão da biblioteca de quadrados

#define CLOSE_WINDOW 2

#define X_SCREEN 960																																														//Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 540
#define FLOOR Y_SCREEN - 10


	const char gravity = 2;

	unsigned long long frame = 0;
	unsigned int counter = 99;

unsigned char collision_x (box *element_first, box *element_second) {
	if (((element_first->x-element_first->width/2 >= element_second->x-element_second->width/2) && (element_second->x+element_second->width/2 >= element_first->x-element_second->width/2)) || 
		((element_second->x-element_second->width/2 >= element_first->x-element_first->width/2) && (element_first->x+element_first->width/2 >= element_second->x-element_second->width/2)))	return 1;
	else return 0; 
}		

unsigned char collision_y (box *element_first, box *element_second) {
	if (((element_first->y + element_first->height/ 2 >= element_second->y - element_second->height/2) && (element_first->y + element_first->height/ 2 <= element_second->y - element_second->height/2)) ||
		((element_second->y + element_second->height/ 2 >= element_first->y - element_first->height/2) && (element_second->y + element_second->height/ 2 <= element_first->y - element_second->height/2))) return 1;
	else return 0;
}

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
		
		if ((index->x == 0) || (index->x > X_SCREEN)){																																						//Verifica se o projétil saiu das bordas da janela
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

int hit_update (attacks *attack, square *player_2)
{
	al_draw_rectangle(attack->attack_area->x-attack->attack_area->width/2, attack->attack_area->y-attack->attack_area->height/2, attack->attack_area->x+attack->attack_area->width/2, 
		attack->attack_area->y+attack->attack_area->height/2, al_map_rgb(0, 0, 255), 2);					//Insere o quadrado do segundo jogador na tela

	if (collision_2D (attack->attack_area, player_2->hurt_box)) {
		player_2->hp -= attack->attack_damage;
		return 1;
	}
	return 0;
}

void attack_update (square *player_1)
{
	//Update punch
	if (player_1->punch->action_time) 
		player_1->punch->action_time--;
	
	if (player_1->air_punch->action_time)
		player_1->air_punch->action_time--;

	if (player_1->crouch_punch->action_time) {
		player_1->crouch_punch->action_time--;
	}

	if (player_1->air_punch->action_time) {
		if (((FLOOR - player_1->box->y)  < 150) && (player_1->vertSpeed < 0)) {
			player_1->control->punch = 0;
			player_1->air_punch->action_time = 0;
		}
		else
			player_1->air_punch->action_time--;
	}

	if (!player_1->crouch_punch->action_time && !player_1->air_punch->action_time && !player_1->punch->action_time) {
		player_1->crouch = player_1->control->down;
		player_1->control->punch = 0;
	}

	//Update kick
	if (player_1->kick->action_time)
		player_1->kick->action_time--;

	if (player_1->air_kick->action_time)
		player_1->air_kick->action_time--;
	
	if (player_1->crouch_kick->action_time) {
		player_1->crouch_kick->action_time--;
	}

	if (player_1->air_kick->action_time) {
		if (((FLOOR - player_1->box->y)) < 200 && (player_1->vertSpeed < 0)) {
			player_1->control->kick = 0;
			player_1->air_kick->action_time = 0;
		}
		else
			player_1->air_kick->action_time--;
	}

	if (!player_1->crouch_kick->action_time && !player_1->air_kick->action_time && !player_1->kick->action_time) {
		player_1->crouch = player_1->control->down;
		player_1->control->kick = 0;
	
		if (!player_1->crouch_punch->action_time && !player_1->air_punch->action_time && !player_1->punch->action_time && player_1->stamina < 100)
			player_1->stamina++;
	}
}

int attack_move (square *player_1, square *player_2)
{
	if (player_1->stamina <= 0)
		return 0;

	//punch attacks
	if (player_1->control->punch) {
		if (player_1->punch->action_time) {
			if (hit_update (player_1->punch, player_2)) {
				player_1->control->punch = 0;
			}	
		}
		if (player_1->crouch_punch->action_time) {
			player_1->crouch = 1;
			if (hit_update (player_1->crouch_punch, player_2)) {
				player_1->control->punch = 0;
				player_1->crouch = player_1->control->down;;
			}
		}
		if (player_1->air_punch->action_time) {
			if (hit_update (player_1->air_punch, player_2)) {
				player_1->control->punch = 0;
			}
		}
	}

	if (player_1->control->punch && !player_1->cooldown) {
		if (player_1->jump) {
			if (player_1->control->down) {
				player_1->cooldown += player_1->crouch_punch->attack_time;
				player_1->stamina -= player_1->crouch_punch->attack_time;
				player_1->crouch_punch->action_time = player_1->crouch_punch->attack_time;
			}
			else {
				player_1->crouch = 1;
				player_1->cooldown += player_1->punch->attack_time;
				player_1->stamina -= player_1->punch->attack_time;
				player_1->punch->action_time = player_1->punch->attack_time;
			}
		}
		else {
			player_1->cooldown += player_1->air_punch->attack_time;
			player_1->stamina -= player_1->air_punch->attack_time;
			player_1->air_punch->action_time = player_1->air_punch->attack_time;
		}
	}

	//kick attacks
	if (player_1->control->kick) {
		if (player_1->kick->action_time) {
			if (hit_update (player_1->kick, player_2)) {
					player_1->control->kick = 0;
			}	
		}
		if (player_1->crouch_kick->action_time) {
			player_1->crouch = 1;
			if (hit_update (player_1->crouch_kick, player_2))
				player_1->control->kick = 0;
		}
		if (player_1->air_kick->action_time) {
			if (hit_update (player_1->air_kick, player_2)) {
				player_1->control->kick = 0;
			}
		}
	}

	if (player_1->control->kick && !player_1->cooldown) {
		if (player_1->jump) {
			if (player_1->control->down) {
				player_1->cooldown += player_1->crouch_kick->attack_time;
				player_1->stamina -= player_1->crouch_kick->attack_time;
				player_1->crouch_kick->action_time = player_1->crouch_kick->attack_time;
			}
			else {
				player_1->crouch = 1;
				player_1->cooldown += player_1->kick->attack_time;
				player_1->stamina -= player_1->kick->attack_time;
				player_1->kick->action_time = player_1->kick->attack_time;
			}
		}
		else {
			player_1->cooldown += player_1->air_kick->attack_time;
			player_1->stamina -= player_1->air_kick->attack_time;
	 		player_1->air_kick->action_time = player_1->air_kick->attack_time;
		}
	}

	return 1;
}


void box_update (square *player_1, int x1_diff, int y1_diff)
{
	player_1->punch->attack_area->x -= x1_diff;
	player_1->punch->attack_area->y -= y1_diff;
	player_1->air_punch->attack_area->x -= x1_diff;
	player_1->air_punch->attack_area->y -= y1_diff;
	player_1->crouch_punch->attack_area->x -=x1_diff;
	player_1->crouch_punch->attack_area->y -=y1_diff;

	player_1->kick->attack_area->x -= x1_diff;
	player_1->kick->attack_area->y -= y1_diff;
	player_1->air_kick->attack_area->x -= x1_diff;
	player_1->air_kick->attack_area->y -= y1_diff;
	player_1->crouch_kick->attack_area->x -=x1_diff;
	player_1->crouch_kick->attack_area->y -=y1_diff;


	// rotaciona as areas
	if (player_1->face == 0) {
		player_1->punch->attack_area->x = player_1->box->x - abs (player_1->box->x - player_1->punch->attack_area->x);
		player_1->air_punch->attack_area->x = player_1->box->x - abs (player_1->box->x - player_1->air_punch->attack_area->x);
		player_1->crouch_punch->attack_area->x = player_1->box->x - abs (player_1->box->x - player_1->crouch_punch->attack_area->x);

		player_1->kick->attack_area->x = player_1->box->x - abs (player_1->box->x - player_1->kick->attack_area->x);
		player_1->air_kick->attack_area->x = player_1->box->x - abs (player_1->box->x - player_1->air_kick->attack_area->x);
		player_1->crouch_kick->attack_area->x = player_1->box->x - abs (player_1->box->x - player_1->crouch_kick->attack_area->x);

	}
	else {
		player_1->punch->attack_area->x = player_1->box->x + abs (player_1->box->x - player_1->punch->attack_area->x);
		player_1->air_punch->attack_area->x = player_1->box->x + abs (player_1->box->x - player_1->air_punch->attack_area->x);
		player_1->crouch_punch->attack_area->x = player_1->box->x + abs (player_1->box->x - player_1->crouch_punch->attack_area->x);

		player_1->kick->attack_area->x = player_1->box->x + abs (player_1->box->x - player_1->kick->attack_area->x);
		player_1->air_kick->attack_area->x = player_1->box->x + abs (player_1->box->x - player_1->air_kick->attack_area->x);
		player_1->crouch_kick->attack_area->x = player_1->box->x + abs (player_1->box->x - player_1->crouch_kick->attack_area->x);
	}

	player_1->hurt_box->x -= x1_diff;
	player_1->hurt_box->y -= y1_diff;
}

void crouch_check (square *player_1, square * player_2)
{
	if ((!player_1->cooldown && player_1->jump && player_1->control->down ) || player_1->crouch_punch->action_time || player_1->crouch_kick->action_time){
		player_1->crouch = 1;
		player_1->box->height = player_1->box->width * PROPORTION/2;
	}
	else if (!player_1->control->down && !player_1->crouch){
		if (collision_x (player_1->box, player_2->box)) {
			if ((player_1->box->y + player_1->box->height/ 2 >= player_2->box->y - player_2->box->height/2) && (player_1->box->y + player_1->box->height/ 2 <= player_2->box->y + player_2->box->height/2)) { //se player_1 esta acima de player_2
				player_1->box->height = player_1->box->width *PROPORTION;
				player_1->box->y = player_2->box->y - player_1->box->height/2 - player_2->box->height/2;
				player_1->crouch = 0;
			}
			else if ((player_1->box->y - player_1->box->height/2 <= player_2->box->y + player_2->box->height/2) && (player_1->box->y - player_1->box->height/2 >= player_2->box->y - player_2->box->height/2)) {
				player_1->box->height = player_1->box->width * PROPORTION;
				player_1->crouch = 0;
				if (collision_2D(player_1->box, player_2->box)) {
					player_1->box->height = player_1->box->width * PROPORTION/2;
					player_1->crouch = 1;
					//player_1->control->down = 1;
				}
			}
			else {
				player_1->box->height = player_1->box->width * PROPORTION;
				player_1->crouch = 0;
				if (collision_2D(player_1->box, player_2->box)) {
					player_1->box->height = player_1->box->width * PROPORTION/2;
					player_1->crouch = 1;
				}
			}
		}
		else {
			player_1->box->height = player_1->box->width * PROPORTION;
			player_1->crouch = 0;
		}
	}
}

void fall_check (square *player_1, square *player_2)
{
	if (collision_x (player_1->box, player_2->box)) { //desloca player 1 de cima de player 2
		if ((player_1->box->y + player_1->box->height/ 2 >= player_2->box->y - player_2->box->height/2) && (player_1->box->y + player_1->box->height/ 2 <= player_2->box->y + player_2->box->height/2)) {
			if (player_1->box->x < player_2->box->x) {
				square_move(player_1, 10, 0, X_SCREEN, FLOOR);
				square_move(player_2, 10, 1, X_SCREEN, FLOOR);
			}
			else {
				square_move(player_1, 10, 1, X_SCREEN, FLOOR);
				square_move(player_2, 10, 0, X_SCREEN, FLOOR);
			}
		}
	}

	if (collision_x (player_1->box, player_2->box)) { // teste se o player esta proximo do chao ou ira cair sobre outro player
		if ((player_1->box->y + player_1->box->height/ 2  -player_1->vertSpeed >= player_2->box->y - player_2->box->height/2) && (player_1->box->y + player_1->box->height/ 2 <= player_2->box->y + player_2->box->height/2)) {		
			player_1->vertSpeed = 0; 
			player_1->box->y = player_2->box->y - player_1->box->height/2 - player_2->box->height/2;
		}
		else {
			if (!player_1->jump && player_1->box->y + player_1->box->height/ 2 + gravity < FLOOR)
				player_1->vertSpeed -= gravity;
			else 
				player_1->box->y = FLOOR - player_1->box->height/ 2 ;
		}
	}
	else {
		if (!player_1->jump && player_1->box->y + player_1->box->height/ 2 - player_1->vertSpeed + gravity < FLOOR)
			player_1->vertSpeed -= gravity;
		else 
			player_1->box->y = FLOOR - player_1->box->height/ 2 ;
	}

	player_1->jump = (( player_1->box->y + player_1->box->height/ 2) >= FLOOR );
	if (player_1->jump) {
		player_1->vertSpeed = 0;
	}
}

void update_position(square *player_1, square *player_2){																																					//Função de atualização das posições dos quadrados conforme os comandos do controle
	int x1_diff = player_1->box->x;
	int y1_diff = player_1->box->y;

	int x2_diff = player_2->box->x;
	int y2_diff = player_2->box->y;

	//golpes
	attack_move (player_1, player_2);
	attack_update (player_1);

	if (player_1->cooldown)
		player_1->cooldown--;

	// movimentacao lateral
	if (player_1->control->left && player_1->jump && !player_1->cooldown && !player_1->crouch){
		player_1->movSpeed = -5;																																										//Se o botão de movimentação para esquerda do controle do segundo jogador está ativado... (!)
	}
	else if (player_1->control->right && player_1->jump && !player_1->cooldown && !player_1->crouch){ 																																										//Se o botão de movimentação para direita do controle do segundo jogador está ativado... (!)
		player_1->movSpeed = 5;
	}
	else if (player_1->jump) player_1->movSpeed = 0;

	if (player_1->movSpeed > 0) {
		square_move(player_1, player_1->movSpeed, 1, X_SCREEN, FLOOR);																																				//Move o quadrado do segundo jogador para a esquerda (!)
		if (collision_2D(player_1->box, player_2->box)) {square_move(player_1, -player_1->movSpeed, 1, X_SCREEN, FLOOR); square_move(player_2, player_1->movSpeed, 1, X_SCREEN, FLOOR);}
	}
	else if (player_1->movSpeed < 0) {
		square_move(player_1, -player_1->movSpeed, 0, X_SCREEN, FLOOR);																																				//Move o quadrado do segundo jogador para a esquerda (!)
		if (collision_2D(player_1->box, player_2->box)) {square_move(player_1, player_1->movSpeed, 0, X_SCREEN, FLOOR); square_move(player_2, -player_1->movSpeed, 0, X_SCREEN, FLOOR);}
	}
	

	//Crouch
	crouch_check (player_1, player_2);

	//Jump and fall
	if (player_1->control->up && player_1->jump && !player_1->cooldown && !player_1->control->down){
		player_1->vertSpeed = 35;																																											//Se o botão de movimentação para cima do controle do segundo jogador está ativado... (!)
		player_1->jump = 0;
	}
	square_move(player_1, player_1->vertSpeed, 2, X_SCREEN, FLOOR);																																				//Move o quadrado do segundo jogador para a cima (!)
	if (collision_2D(player_1->box, player_2->box)) {square_move(player_1, -player_1->vertSpeed, 2, X_SCREEN, FLOOR); player_1->vertSpeed = 0;}
	fall_check (player_1, player_2);


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

	if (player_1->box->x < player_2->box->x)
		player_1->face = 1;
	else
		player_1->face = 0;

	box_update (player_1, x1_diff, y1_diff);
	box_update (player_2, x2_diff, y2_diff);

	update_bullets(player_1);																																												//Atualiza os disparos do primeiro jogador
}
void draw_status (ALLEGRO_FONT *font, int hp1, int hp2, int stamina1, int stamina2, unsigned char counter) 
{
	al_draw_textf(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, 30, ALLEGRO_ALIGN_CENTER, "%d", counter);
	//hp bars
	al_draw_filled_rectangle(10, 40,  (hp1) *((X_SCREEN /2 - 10)/ MAX_HP), 20, al_map_rgb(255, 0, 0));
	al_draw_rectangle(10, 40, X_SCREEN /2 -10, 20, al_map_rgb (255, 255, 255), 2);
	al_draw_filled_rectangle((X_SCREEN / 2 + 10) + (MAX_HP -hp2) *((X_SCREEN /2 - 10)/ MAX_HP), 40, X_SCREEN -10, 20, al_map_rgb(0, 0, 255));
	al_draw_rectangle(X_SCREEN/2 +10, 40, X_SCREEN -10, 20, al_map_rgb (255, 255, 255), 2);

	//stamina bars
	if (stamina1 >= 0)
		al_draw_filled_rectangle(10, 50, (stamina1) *(X_SCREEN /4) / 100, 60, al_map_rgb(255, 0, 0));
	al_draw_rectangle(10, 50, X_SCREEN /4, 60, al_map_rgb (255, 255, 255), 2);

	if (stamina2 >= 0)
		al_draw_filled_rectangle(((X_SCREEN) - X_SCREEN / 4) + (100 -stamina2) *((X_SCREEN /4)/ 100), 50 , X_SCREEN -10, 60, al_map_rgb(255, 0, 0));
	al_draw_rectangle(X_SCREEN - X_SCREEN/ 4, 50, X_SCREEN -10, 60, al_map_rgb (255, 255, 255), 2);

}
void draw_player (square *player, unsigned long int frame)
{
	  // Definir novas dimensões para a imagem
	int nova_largura = player->box->width;
	int nova_altura = player->box->height;

	if (player->punch->action_time) {
		int i = (player->punch->attack_time - player->punch->action_time) / (player->punch->attack_time / player->actions->punch->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->punch->props[i]->x, player->actions->punch->props[i]->y,  player->actions->punch->props[i]->width, player->actions->punch->props[i]->height, // fonte
	  		player->box->x + (player->box->width - (2*player->face * player->box->width)) *2, player->box->y - player->box->height /2 * player->actions->punch->props[i]->height / 75, -(nova_largura - (2*player->face * nova_largura)) *PROPORTION * player->actions->punch->props[i]->width / 75, nova_altura * player->actions->punch->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->air_punch->action_time) {
		int i = (player->air_punch->attack_time - player->air_punch->action_time) / (player->air_punch->attack_time / player->actions->air_punch->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->air_punch->props[i]->x, player->actions->air_punch->props[i]->y,  player->actions->air_punch->props[i]->width, player->actions->air_punch->props[i]->height, // fonte
	  		player->box->x + (player->box->width - (2*player->face * player->box->width)) *2, player->box->y - player->box->height /2 * player->actions->air_punch->props[i]->height / 75, -(nova_largura - (2*player->face * nova_largura))*PROPORTION * player->actions->air_punch->props[i]->width / 75, nova_altura * player->actions->air_punch->props[i]->height / 75,     // destino
	   		0);

	}
	else if (player->crouch_punch->action_time) {
		int i = (player->crouch_punch->attack_time - player->crouch_punch->action_time) / (player->crouch_punch->attack_time / player->actions->crouch_punch->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->crouch_punch->props[i]->x, player->actions->crouch_punch->props[i]->y,  player->actions->crouch_punch->props[i]->width, player->actions->crouch_punch->props[i]->height, // fonte
	  		player->box->x + (player->box->width - (2*player->face * player->box->width)) *2, player->box->y - (player->box->height + player->box->height/4) * player->actions->crouch_punch->props[i]->height / 75,-(nova_largura - (2*player->face * nova_largura))*PROPORTION * player->actions->crouch_punch->props[i]->width / 75, nova_altura*2 * player->actions->crouch_punch->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->kick->action_time) {
		int i = (player->kick->attack_time - player->kick->action_time) / (player->kick->attack_time / player->actions->kick->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->kick->props[i]->x, player->actions->kick->props[i]->y,  player->actions->kick->props[i]->width, player->actions->kick->props[i]->height, // fonte
	  		(player->box->x + (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - player->box->height /2 * player->actions->kick->props[i]->height / 75 , -(nova_largura - (2*player->face * nova_largura))*PROPORTION * player->actions->kick->props[i]->width / 75,  nova_altura * player->actions->kick->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->air_kick->action_time) {
		int i = (player->air_kick->attack_time - player->air_kick->action_time) / (player->air_kick->attack_time / player->actions->air_kick->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->air_kick->props[i]->x, player->actions->air_kick->props[i]->y,  player->actions->air_kick->props[i]->width, player->actions->air_kick->props[i]->height, // fonte
	  		player->box->x + (player->box->width - (2*player->face * player->box->width)) *2, player->box->y - player->box->height /2 * player->actions->air_kick->props[i]->height / 75, -(nova_largura - (2*player->face * nova_largura))*PROPORTION * player->actions->air_kick->props[i]->width / 75, nova_altura * player->actions->air_kick->props[i]->height / 75,     // destino
	   		0);

	}
	else if (player->crouch_kick->action_time) {
		int i = (player->crouch_kick->attack_time - player->crouch_kick->action_time) / (player->crouch_kick->attack_time / player->actions->crouch_kick->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->crouch_kick->props[i]->x, player->actions->crouch_kick->props[i]->y,  player->actions->crouch_kick->props[i]->width, player->actions->crouch_kick->props[i]->height, // fonte
	  		player->box->x + (player->box->width - (2*player->face * player->box->width)) *2, player->box->y  - (player->box->height + player->box->height/4) * player->actions->crouch_kick->props[i]->height / 75, -(nova_largura - (2*player->face * nova_largura))*PROPORTION * player->actions->crouch_kick->props[i]->width / 75, nova_altura*2 *player->actions->crouch_kick->props[i]->height / 75,     // destino
	   		0);
	}
	else if (!player->jump) {
		int i;
		if (player->vertSpeed > 10)
			i = 0;
		else if (player->vertSpeed < -10)
			i = 2;
		else
			i = 1;
		al_draw_scaled_bitmap(player->sprites,
			player->actions->jump->props[i]->x, player->actions->jump->props[i]->y,  player->actions->jump->props[i]->width, player->actions->jump->props[i]->height, // fonte
	  		player->box->x + (player->box->width - (2*player->face * player->box->width)) *2, player->box->y - player->box->height /2 * player->actions->jump->props[i]->height / 75, -(nova_largura - (2*player->face * nova_largura))*PROPORTION * player->actions->jump->props[i]->width / 75, nova_altura *  player->actions->jump->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->crouch) {
		int i = 0;
		al_draw_scaled_bitmap(player->sprites,
			player->actions->crouch->props[i]->x, player->actions->crouch->props[i]->y,  player->actions->crouch->props[i]->width, player->actions->crouch->props[i]->height, // fonte
	  		(player->box->x + (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - (player->box->height + player->box->height /4) * player->actions->crouch->props[i]->height / 75, -(nova_largura - (2*player->face * nova_largura))*PROPORTION * player->actions->crouch->props[i]->width / 75, nova_altura*2 * player->actions->crouch->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->control->left || player->control->right) {
		int i = frame/2 % player->actions->walk->quantity;
		al_draw_scaled_bitmap(player->sprites,
			player->actions->walk->props[i]->x, player->actions->walk->props[i]->y,  player->actions->walk->props[i]->width, player->actions->walk->props[i]->height, // fonte
	  		player->box->x + (player->box->width - (2*player->face * player->box->width)) *2, player->box->y - player->box->height /2 * player->actions->walk->props[i]->height / 75, -(nova_largura - (2*player->face * nova_largura))*PROPORTION * player->actions->walk->props[i]->width / 75, nova_altura * player->actions->walk->props[i]->height / 75,     // destino
	   		0);
	}
	else {
		int i = frame/6 % player->actions->standing->quantity;
		al_draw_scaled_bitmap(player->sprites,
			player->actions->standing->props[i]->x, player->actions->standing->props[i]->y,  player->actions->standing->props[i]->width, player->actions->standing->props[i]->height, // fonte
	  		player->box->x + (player->box->width - (2*player->face * player->box->width)) *2, player->box->y - player->box->height /2 * player->actions->standing->props[i]->height / 75, -(nova_largura - (2*player->face * nova_largura))*PROPORTION * player->actions->standing->props[i]->width / 75, nova_altura * player->actions->standing->props[i]->height / 75,     // destino
	   		0);
	}
	
	al_draw_rectangle(player->hurt_box->x-player->hurt_box->width/2, player->hurt_box->y-player->hurt_box->height/2, player->hurt_box->x+player->hurt_box->width/2, player->hurt_box->y+player->hurt_box->height/2, al_map_rgb(0, 0, 255), 2);					//Insere o quadrado do segundo jogador na tela

	al_draw_rectangle(player->box->x-player->box->width/2, player->box->y-player->box->height/2, player->box->x+player->box->width/2, player->box->y+player->box->height/2, al_map_rgb(255, 255, 255), 2);	
/*
	if (player->punch->action_time)
		al_draw_filled_rectangle(player->punch->attack_area->x-player->punch->attack_area->width/2, player->punch->attack_area->y-player->punch->attack_area->height/2, player->punch->attack_area->x+player->punch->attack_area->width/2, player->punch->attack_area->y+player->punch->attack_area->height/2, al_map_rgb(255, 255, 255));
	if (player->air_punch->action_time)
		al_draw_filled_rectangle(player->air_punch->attack_area->x-player->air_punch->attack_area->width/2, player->air_punch->attack_area->y-player->air_punch->attack_area->height/2, player->air_punch->attack_area->x+player->air_punch->attack_area->width/2, player->air_punch->attack_area->y+player->air_punch->attack_area->height/2, al_map_rgb(255, 255, 255));
*/
}

void control (ALLEGRO_EVENT event, square *player_1, square *player_2)
{
	if (event.type == 10) {
		if (event.keyboard.keycode == 1) {player_1->face = 0; player_1->control->left = 1;}																															//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda)
		else if (event.keyboard.keycode == 4) {player_1->face = 1; player_1->control->right = 1;}																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)
		else if (event.keyboard.keycode == 23) player_1->control->up = 1;																														//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para cima)
		else if (event.keyboard.keycode == 19) player_1->control->down = 1;																													//Indica o evento correspondente no controle do primeiro jogador (botão de movimentação para baixo)
		else if (event.keyboard.keycode == 82) {player_2->control->left = 1;}																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à esquerda)
		else if (event.keyboard.keycode == 83) {player_2->control->right = 1;}																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação à direita)
		else if (event.keyboard.keycode == 84) player_2->control->up = 1;																														//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para cima)
		else if (event.keyboard.keycode == 85) player_2->control->down = 1;																													//Indica o evento correspondente no controle do segundo jogador (botão de movimentação para baixo)
		else if (event.keyboard.keycode == 3) player_1->control->fire = 1;																														//Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c)					
		else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_3) player_2->control->fire = 1;																									//Indica o evento correspondente no controle do segundo joagdor (botão de disparo - shift dir)
		else if (event.keyboard.keycode == ALLEGRO_KEY_J && !player_1->cooldown) player_1->control->punch = 1;
		else if (event.keyboard.keycode == ALLEGRO_KEY_K && !player_1->cooldown) player_1->control->kick = 1;
		else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_1 && !player_2->cooldown) player_2->control->punch = 1;
		else if (event.keyboard.keycode == ALLEGRO_KEY_PAD_2&& !player_2->cooldown) player_2->control->kick = 1;
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
	}
}

int gameLoop (square *player_1, square *player_2, ALLEGRO_BITMAP *background, unsigned char background_count, Essentials *essentials)
{
	unsigned char p1k = 0, p2k = 0;
	unsigned char p1wins = 0, p2wins = 0; 
	unsigned char round = 0;

	unsigned char character;
	int menu_control;
	float pos;

	while(1){	
		al_wait_for_event(essentials->queue, &(essentials->event));																																									//Função que captura eventos da fila, inserindo os mesmos na variável de eventos
			
		if (p1wins == 2 || p2wins == 2){																																													//Verifica se algum jogador foi morto 																																							//Limpe a tela atual para um fundo preto
			if (p1wins == 2 && p2wins == 2){ if (!endGameMenu(0, essentials)) break;} // draw
			else if (p1wins == 2) {if (!endGameMenu(1, essentials)) break;} // player 1 wins
			else if (!endGameMenu(2, essentials))break; // player 2 wins
			p1wins = p2wins = 0;
			round = 0;																			//Indique o modo de conclusão do programa

			if ((essentials->event.type == 10) && (essentials->event.keyboard.keycode == 75)) break;																																//Espera por um evento de teclado, de clique da tecla de espaço
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_ENTER || essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {}
			else if (essentials->event.type == 42) break; 																																								//Finaliza o jogo
		}
		else if (round > 3) {
			if (p1wins == p2wins ){ if (!endGameMenu(0, essentials)) break;} // draw
			else if (p1wins > p2wins) {if (!endGameMenu(1, essentials)) break;}	// player 1 wins
			else if (!endGameMenu(2, essentials))break; // player 2 wins
			p1wins = p2wins = 0; // wins reset
			round = 0;

			if ((essentials->event.type == 10) && (essentials->event.keyboard.keycode == 75)) break;																																//Espera por um evento de teclado, de clique da tecla de espaço
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_ENTER || essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {}
			else if (essentials->event.type == 42) break;
		}
		else{																																																//Se nenhum quadrado morreu
			if (essentials->event.type == 30){																																											//O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)

				frame++;
				al_clear_to_color(al_map_rgb(0, 0, 0));
				pos = ((float) X_SCREEN/2 - ((float)(player_1->box->x + player_2->box->x))/2);

				al_draw_scaled_bitmap(background,
					((frame / 5 % background_count) * 768) -(pos / (X_SCREEN/2 -20)) * 215, 0, 429, 241, // fonte
	  				0, 0, X_SCREEN, Y_SCREEN,     // destino
	   				0);
			
				//al_draw_bitmap(background, 0, 0, 0);
				draw_player (player_2, frame);
				draw_player (player_1, frame);
				draw_status (essentials->font,player_1->hp, player_2->hp, player_1->stamina, player_2->stamina, counter);

				update_position(player_1, player_2);
				update_position(player_2, player_1);																																						//Atualiza a posição dos jogadores
				p1k = check_kill(player_2, player_1);																																						//Verifica se o primeiro jogador matou o segundo jogador
				p2k = check_kill(player_1, player_2);

				if (player_1->hp <= 0) p1k = 1;																																						//Verifica se o segundo jogador matou o primeiro jogador
				if (player_2->hp <= 0) p2k = 1;

				if (p1k || p2k || !counter) { // win condition
					if (!counter) {
						if (player_1->hp > player_2->hp) p1wins++;
						else if (player_1->hp > player_2->hp) p2wins++;	
					}
					if (p2k) 
						p1wins++;
					if (p1k)
						p2wins++;
					p1k = p2k = 0;
					round++;
					
					character = player_1->character;
					square_destroy(player_1);																																												//Destrutor do quadrado do primeiro jogador
					player_1 = character_load (1, character);

					character = player_2->character;
					square_destroy(player_2);
					player_2 = character_load (2, character);
					
					counter = 99;
				}

				if (frame % 30 == 0)
					counter--;
				

	    		for (bullet *index = player_1->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(255, 0, 0));								//Insere as balas existentes disparadas pelo primeiro jogador na tela
	    		if (player_1->gun->timer) player_1->gun->timer--;																																			//Atualiza o cooldown da arma do primeiro jogador
	    		for (bullet *index = player_2->gun->shots; index != NULL; index = (bullet*) index->next) al_draw_filled_circle(index->x, index->y, 2, al_map_rgb(0, 0, 255));								//Insere as balas existentes disparadas pelo segundo jogador na tela
	    		if (player_2->gun->timer) player_2->gun->timer--;
	    		al_flip_display();																																											//Insere as modificações realizadas nos buffers de tela
			}
			if (essentials->event.type == 10 && essentials->event.keyboard.keycode == ALLEGRO_KEY_P){
				joystick_reset (player_1->control);
				joystick_reset (player_2->control);
				if ( (menu_control = menu_pause (essentials)) == 2) {
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
			else if (essentials->event.type == 42) return 0;
			control (essentials->event, player_1, player_2);																																								//Evento de clique no "X" de fechamento da tela. Encerra o programa graciosamente.
		}
	}

	return 1;


}

int main(){
	Essentials *essentials;
	if (!(essentials = malloc (sizeof (Essentials))))
		return 1;
	start_essentials (&essentials);

	square* player_1;// = square_create(30, 1, 50, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																															//Cria o quadrado do primeiro jogadorrado do primeiro jogador
	square* player_2;// = square_create(30, 0, X_SCREEN - 50, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																													//Cria o quadrado do segundo jogador
    int menu_control = 1;

	ALLEGRO_BITMAP *background;
	unsigned char background_count;


	while(1){	

		if (menu_control == 1) {
			if ((menu_control = menu (essentials) == CLOSE_WINDOW)) {
				end_essentials (essentials);
				return 0;
			}
			if (menuCharacter (&player_1, &player_2, essentials) == CLOSE_WINDOW) {
				end_essentials (essentials);
				return 0;
			}
			if (menuMap (&background, &background_count, essentials) == CLOSE_WINDOW) {
				end_essentials (essentials);
				return 0;
			}
		}
 
		if (gameLoop (player_1, player_2, background, background_count, essentials) == 1)
			menu_control = 1;
		else
			break;
	}
	end_essentials (essentials);
	return 0;
}