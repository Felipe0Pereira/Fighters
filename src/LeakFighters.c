//Compilação: gcc AggressiveSquares.c Menu.c Essentials.c Player.c Joystick.c Attacks.c Bullet.c Pistol.c Box.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

#include <stdio.h>
#include "Essentials.h"
#include "Player.h"	
#include "Menu.h"

#define CLOSE_WINDOW 2

#define X_SCREEN 960
#define Y_SCREEN 540
#define FLOOR Y_SCREEN - 10

#define X_MAP 1728
#define RATIO 3.186 // proporcao do mapa

#define ROUND_TIME 99
#define START_ROUND_TIME 60
#define END_TIME 90

#define GRAVITY 2

//retorna maior entre n1 e n2
unsigned short max (int n1, int n2)
{
	if (n1 < n2)
		return n2;
	return n1;
}

//retorna menor entre n1 e n2
unsigned short min (unsigned short n1, unsigned short n2)
{
	if (n1 > n2)
		return n2;
	return n1;
}

// Colisao no eixo x
// Retorna 1 se colidiu, 0 caso contrario
unsigned char collision_x (box *element_first, box *element_second) {
	if (((element_first->x-element_first->width/2 >= element_second->x-element_second->width/2) && (element_second->x+element_second->width/2 >= element_first->x-element_second->width/2)) || 
		((element_second->x-element_second->width/2 >= element_first->x-element_first->width/2) && (element_first->x+element_first->width/2 >= element_second->x-element_second->width/2)))	return 1;
	else return 0; 
}		

// Colisao no eixo y
// Retorna 1 se colidiu, 0 caso contrario
unsigned char collision_y (box *element_first, box *element_second) {
	if (((element_first->y + element_first->height/ 2 >= element_second->y - element_second->height/2) && (element_first->y + element_first->height/ 2 <= element_second->y - element_second->height/2)) ||
		((element_second->y + element_second->height/ 2 >= element_first->y - element_first->height/2) && (element_second->y + element_second->height/ 2 <= element_first->y - element_second->height/2))) return 1;
	else return 0;
}

// Retorna 1 se colidiu, 0 caso contrario
unsigned char collision_2D(box *element_first, box *element_second){																																	//Implementação da função de verificação de colisão entre dois quadrados

	if ((((element_second->y-element_second->height/2 > element_first->y-element_first->height/2) && (element_first->y+element_first->height/2 > element_second->y-element_second->height/2)) || 	//				//Verifica se o primeiro elemento colidiu com o segundo no eixo X 
		((element_first->y-element_first->height/2 >= element_second->y-element_second->height/2) && (element_second->y+element_second->height/2 > element_first->y-element_first->height/2))) && 	//				//Verifica se o segundo elemento colidiu com o primeiro no eixo X 
		(((element_second->x-element_second->width/2 >= element_first->x-element_first->width/2) && (element_first->x+element_first->width/2 >= element_second->x-element_second->width/2)) || 	//				//Verifica se o primeiro elemento colidiu com o segundo no eixo Y
		((element_first->x-element_first->width/2 >= element_second->x-element_second->width/2) && (element_second->x+element_second->width/2 >= element_first->x-element_first->width/2)))) return 1;			//Verifica se o segundo elemento colidiu com o primeiro no eixo Y
	else return 0;																																															//Se as condições não forem satisfeita, não houve colisão
}

// Verifica se player foi atingido por um golpe
int hit_update (attacks *attack, Player *player)
{
	if (attack->action_time > attack->attack_time/2)
		return 0;

	if (collision_2D (attack->attack_area, player->hurt_box)) {
		player->hp -= attack->attack_damage;
		return 1;
	}
	return 0;
}

// Atualiza os status de ataque do player
void attack_update (Player *player)
{
	//Update punch
	if (player->punch->action_time) 
		player->punch->action_time--;
	
	if (player->air_punch->action_time)
		player->air_punch->action_time--;

	if (player->crouch_punch->action_time) {
		player->crouch_punch->action_time--;
	}

	if (player->air_punch->action_time) {
		if (((FLOOR - player->box->y)  < 150) && (player->vertSpeed < 0)) {
			player->control->punch = 0;
			player->air_punch->action_time = 0;
		}
		else
			player->air_punch->action_time--;
	}

	if (!player->crouch_punch->action_time && !player->air_punch->action_time && !player->punch->action_time) {
		player->crouch = player->control->down;
		player->control->punch = 0;
	}

	//Update kick
	if (player->kick->action_time)
		player->kick->action_time--;

	if (player->air_kick->action_time)
		player->air_kick->action_time--;
	
	if (player->crouch_kick->action_time) {
		player->crouch_kick->action_time--;
	}

	if (player->air_kick->action_time) {
		if (((FLOOR - player->box->y)) < 200 && (player->vertSpeed < 0)) {
			player->control->kick = 0;
			player->air_kick->action_time = 0;
		}
		else
			player->air_kick->action_time--;
	}

	if (!player->crouch_kick->action_time && !player->air_kick->action_time && !player->kick->action_time) {
		player->crouch = player->control->down;
		player->control->kick = 0;
	}
}

// Verifica se o player 1 ira conseguir executar o ataque do comando dado
int attack_move (Player *player_1, Player *player_2)
{
	if (player_1->stamina <= 0)
		return 0;

	unsigned char hit = 0;
	//punch attacks
	if (player_1->control->punch) {
		if (player_1->punch->action_time) {
			if (hit_update (player_1->punch, player_2)) {
				player_1->control->punch = 0;
				hit = player_1->punch->attack_time;
			}	
		}
		if (player_1->crouch_punch->action_time) {
			player_1->crouch = 1;
			if (hit_update (player_1->crouch_punch, player_2)) {
				player_1->control->punch = 0;
				player_1->crouch = player_1->control->down;;
				hit = player_1->crouch_punch->attack_time;
			}
		}
		if (player_1->air_punch->action_time) {
			if (hit_update (player_1->air_punch, player_2)) {
				player_1->control->punch = 0;
				hit = player_1->air_punch->attack_time;
			}
		}
	}

	if (player_1->control->punch && !player_1->cooldown) {
		if (player_1->jump) {
			if (player_1->control->down) {
				player_1->cooldown += player_1->crouch_punch->attack_time;
				player_1->stamina -= player_1->crouch_punch->attack_time*2;
				player_1->crouch_punch->action_time = player_1->crouch_punch->attack_time;
			}
			else {
				player_1->crouch = 1;
				player_1->cooldown += player_1->punch->attack_time;
				player_1->stamina -= player_1->punch->attack_time*2;
				player_1->punch->action_time = player_1->punch->attack_time;
			}
		}
		else if ((FLOOR - player_1->box->y) > 150 || player_1->vertSpeed >= 0){
			player_1->cooldown += player_1->air_punch->attack_time;
			player_1->stamina -= player_1->air_punch->attack_time*2;
			player_1->air_punch->action_time = player_1->air_punch->attack_time;
		}
	}

	//kick attacks
	if (player_1->control->kick) {
		if (player_1->kick->action_time) {
			if (hit_update (player_1->kick, player_2)) {
				player_1->control->kick = 0;
				hit = player_1->kick->attack_time;
			}	
		}
		if (player_1->crouch_kick->action_time) {
			player_1->crouch = 1;
			if (hit_update (player_1->crouch_kick, player_2)) {
				player_1->control->kick = 0;
				hit = player_1->crouch_kick->attack_time;
			}
		}
		if (player_1->air_kick->action_time) {
			if (hit_update (player_1->air_kick, player_2)) {
				player_1->control->kick = 0;
				hit = player_1->air_kick->attack_time;
			}
		}
	}

	if (player_1->control->kick && !player_1->cooldown) {
		if (player_1->jump) {
			if (player_1->control->down) {
				player_1->cooldown += player_1->crouch_kick->attack_time;
				player_1->stamina -= player_1->crouch_kick->attack_time*2;
				player_1->crouch_kick->action_time = player_1->crouch_kick->attack_time;
			}
			else {
				player_1->crouch = 1;
				player_1->cooldown += player_1->kick->attack_time;
				player_1->stamina -= player_1->kick->attack_time*2;
				player_1->kick->action_time = player_1->kick->attack_time;
			}
		}
		else if ((FLOOR - player_1->box->y) > 200 || player_1->vertSpeed >= 0){
			player_1->cooldown += player_1->air_kick->attack_time;
			player_1->stamina -= player_1->air_kick->attack_time*2;
	 		player_1->air_kick->action_time = player_1->air_kick->attack_time;
		}
	}

	return hit;
}

// Atualiza a posicao das estruturas de hit box, hurt box e areas de ataque
void box_update (Player *player, int x1_diff, int y1_diff)
{
	player->punch->attack_area->x -= x1_diff;
	player->punch->attack_area->y -= y1_diff;
	player->air_punch->attack_area->x -= x1_diff;
	player->air_punch->attack_area->y -= y1_diff;
	player->crouch_punch->attack_area->x -=x1_diff;
	player->crouch_punch->attack_area->y -=y1_diff;

	player->kick->attack_area->x -= x1_diff;
	player->kick->attack_area->y -= y1_diff;
	player->air_kick->attack_area->x -= x1_diff;
	player->air_kick->attack_area->y -= y1_diff;
	player->crouch_kick->attack_area->x -=x1_diff;
	player->crouch_kick->attack_area->y -=y1_diff;


	// rotaciona as areas
	if (player->face == 0) {
		player->punch->attack_area->x = player->box->x - abs (player->box->x - player->punch->attack_area->x);
		player->air_punch->attack_area->x = player->box->x - abs (player->box->x - player->air_punch->attack_area->x);
		player->crouch_punch->attack_area->x = player->box->x - abs (player->box->x - player->crouch_punch->attack_area->x);

		player->kick->attack_area->x = player->box->x - abs (player->box->x - player->kick->attack_area->x);
		player->air_kick->attack_area->x = player->box->x - abs (player->box->x - player->air_kick->attack_area->x);
		player->crouch_kick->attack_area->x = player->box->x - abs (player->box->x - player->crouch_kick->attack_area->x);

	}
	else {
		player->punch->attack_area->x = player->box->x + abs (player->box->x - player->punch->attack_area->x);
		player->air_punch->attack_area->x = player->box->x + abs (player->box->x - player->air_punch->attack_area->x);
		player->crouch_punch->attack_area->x = player->box->x + abs (player->box->x - player->crouch_punch->attack_area->x);

		player->kick->attack_area->x = player->box->x + abs (player->box->x - player->kick->attack_area->x);
		player->air_kick->attack_area->x = player->box->x + abs (player->box->x - player->air_kick->attack_area->x);
		player->crouch_kick->attack_area->x = player->box->x + abs (player->box->x - player->crouch_kick->attack_area->x);
	}

	player->hurt_box->x -= x1_diff;
	player->hurt_box->y -= y1_diff;
}

// Verifica se o player 1 pode agachar ou levantar
void crouch_check (Player *player_1, Player * player_2)
{
	if ((!player_1->cooldown && player_1->jump && player_1->control->down ) || player_1->crouch_punch->action_time || player_1->crouch_kick->action_time){
		player_1->crouch = 1;
		player_1->box->height = player_1->box->width * PROPORTION/2;
	}
	else if (!player_1->control->down && !player_1->crouch){
		if (collision_x (player_1->box, player_2->box)) {
			if ((player_1->box->y + player_1->box->height/ 2 >= player_2->box->y - player_2->box->height/2) && 
				(player_1->box->y + player_1->box->height/ 2 <= player_2->box->y + player_2->box->height/2)) { //se player_1 esta acima de player_2
				player_1->box->height = player_1->box->width *PROPORTION;
				player_1->box->y = player_2->box->y - player_1->box->height/2 - player_2->box->height/2;
				player_1->crouch = 0;
			}
			else if ((player_1->box->y - player_1->box->height/2 <= player_2->box->y + player_2->box->height/2) && 
				(player_1->box->y - player_1->box->height/2 >= player_2->box->y - player_2->box->height/2)) {
				player_1->box->height = player_1->box->width * PROPORTION;
				player_1->crouch = 0;
				if (collision_2D(player_1->box, player_2->box)) {
					player_1->box->height = player_1->box->width * PROPORTION/2;
					player_1->crouch = 1;
					
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
	else if (player_1->control->down && player_1->jump) {
		player_1->box->height = player_1->box->width * PROPORTION/2;
		player_1->crouch = 1;
	}
	else {
		player_1->box->height = player_1->box->width * PROPORTION;
		player_1->crouch = 0;
	}
}

// Verifica se a queda do player 1 ira colidir com player 2
void fall_check (Player *player_1, Player *player_2)
{
	if (collision_x (player_1->box, player_2->box)) { //desloca player 1 de cima de player 2
		if ((player_1->box->y + player_1->box->height/ 2 >= player_2->box->y - player_2->box->height/2) && (player_1->box->y + player_1->box->height/ 2 <= player_2->box->y + player_2->box->height/2)) {
			if (player_1->box->x < player_2->box->x) {
				player_move(player_1, 10, 0, max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0,min ((player_1->box->x + player_2->box->x)/2+ X_SCREEN/2, X_MAP) , FLOOR);
				player_move(player_2, 10, 1, max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0,min ((player_1->box->x + player_2->box->x)/2+ X_SCREEN/2, X_MAP) , FLOOR);
			}
			else {
				player_move(player_1, 10, 1, max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0,min ((player_1->box->x + player_2->box->x)/2+ X_SCREEN/2, X_MAP) , FLOOR);
				player_move(player_2, 10, 0, max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0, min ((player_1->box->x + player_2->box->x)/2+ X_SCREEN/2, X_MAP) , FLOOR);
			}
		}
	}

	if (collision_x (player_1->box, player_2->box)) { // teste se o player esta proximo do chao ou ira cair sobre outro player
		if ((player_1->box->y + player_1->box->height/ 2  -player_1->vertSpeed >= player_2->box->y - player_2->box->height/2) && 
			(player_1->box->y + player_1->box->height/ 2 <= player_2->box->y + player_2->box->height/2)) {		
			player_1->vertSpeed = 0; 
			player_1->box->y = player_2->box->y - player_1->box->height/2 - player_2->box->height/2;
		}
		else {
			if (!player_1->jump && player_1->box->y + player_1->box->height/ 2 + GRAVITY < FLOOR)
				player_1->vertSpeed -= GRAVITY;
			else 
				player_1->box->y = FLOOR - player_1->box->height/ 2 ;
		}
	}
	else {
		if (!player_1->jump && player_1->box->y + player_1->box->height/ 2 - player_1->vertSpeed + GRAVITY < FLOOR)
			player_1->vertSpeed -= GRAVITY;
		else 
			player_1->box->y = FLOOR - player_1->box->height/ 2 ;
	}

	player_1->jump = (( player_1->box->y + player_1->box->height/ 2) >= FLOOR );
	if (player_1->jump) {
		player_1->vertSpeed = 0;
	}
}

// Atualiza posicionamentos, ataques e status dos players
void update (unsigned long int frame, Player *player_1, Player *player_2){
	int x1_diff = player_1->box->x;
	int y1_diff = player_1->box->y;

	int x2_diff = player_2->box->x;
	int y2_diff = player_2->box->y;
	
	unsigned char stun;

	if (player_1->stamina < MAX_STAMINA && !(frame % 2))
		player_1->stamina++;
	if (player_1->cooldown)
		player_1->cooldown--;
	if (player_1->stuned)
		player_1->stuned--;

	//golpes
	if ((stun = attack_move (player_1, player_2))) {
		player_2->stuned = stun /3;
		player_2->cooldown = stun;
		
		if (player_2->face)
			player_2->movSpeed = -15;
		else
			player_2->movSpeed = 15;
	}
	attack_update (player_1);


	// movimentacao lateral
	if (player_1->control->left && player_1->jump && !player_1->cooldown && !player_1->crouch){
		player_1->movSpeed = -10;
	}
	else if (player_1->control->right && player_1->jump && !player_1->cooldown && !player_1->crouch){
		player_1->movSpeed = 10;
	}
	else if (!player_1->stuned && player_1->jump) player_1->movSpeed = 0;

	if (player_1->movSpeed > 0) {
		player_move(player_1, player_1->movSpeed, 1, max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0,min ((player_1->box->x + player_2->box->x)/2+ X_SCREEN/2, X_MAP) , FLOOR);
		if (collision_2D(player_1->box, player_2->box)) {
			player_move(player_1, -player_1->movSpeed, 1, max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0,min ((player_1->box->x + player_2->box->x)/2+ X_SCREEN/2, X_MAP) , FLOOR); 
			player_move(player_2, player_1->movSpeed, 1, max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0,min ((player_1->box->x + player_2->box->x)/2+ X_SCREEN/2, X_MAP) , FLOOR);
		}
	}
	else if (player_1->movSpeed < 0) {
		player_move(player_1, -player_1->movSpeed, 0,  max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0,min ((player_1->box->x + player_2->box->x)/2+ X_SCREEN/2, X_MAP) , FLOOR);
		if (collision_2D(player_1->box, player_2->box)) {
			player_move(player_1, player_1->movSpeed, 0,  max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0,min ((player_1->box->x + player_2->box->x)/2+ X_SCREEN/2, X_MAP) , FLOOR);
			player_move(player_2, -player_1->movSpeed, 0,max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0,min ((player_1->box->x + player_2->box->x)/2+ X_SCREEN/2, X_MAP) , FLOOR);
		}
	}
	

	//Crouch
	crouch_check (player_1, player_2);

	//Jump and fall
	if (player_1->control->up && player_1->jump && !player_1->cooldown && !player_1->control->down){
		player_1->vertSpeed = 33;
		player_1->jump = 0;
	}
	player_move(player_1, player_1->vertSpeed, 2, max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0) ,0, min ((player_1->box->x + player_2->box->x)/2 + X_SCREEN/2, X_MAP) , FLOOR);
	if (collision_2D(player_1->box, player_2->box)) {
		player_move(player_1, -player_1->vertSpeed, 2, max ((player_1->box->x + player_2->box->x)/2 - X_SCREEN/2, 0),0,min ((player_1->box->x + player_2->box->x)/2 + X_SCREEN/2, X_MAP) , FLOOR);
		player_1->vertSpeed = 0;
	}
	fall_check (player_1, player_2);

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

}

// Controle de movimentacao dos players
void control (ALLEGRO_EVENT event, Player *player_1, Player *player_2)
{
	if (event.type == 10) {
		switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_A:
				player_1->control->left = 1;
			break;
			case ALLEGRO_KEY_D:
				player_1->control->right = 1;
			break;
			case ALLEGRO_KEY_W:
				player_1->control->up = 1;
			break;
			case ALLEGRO_KEY_S:
				player_1->control->down = 1;
			break;
			case ALLEGRO_KEY_LEFT:
				player_2->control->left = 1;
			break;
			case ALLEGRO_KEY_RIGHT:
				player_2->control->right = 1;
			break;
			case ALLEGRO_KEY_UP:
				player_2->control->up = 1;
			break;
			case ALLEGRO_KEY_DOWN:
				player_2->control->down = 1;
			break;
			case ALLEGRO_KEY_J:
				if (!player_1->cooldown)
					player_1->control->punch = 1;
			break;
			case ALLEGRO_KEY_K:
				if (!player_1->cooldown)
					player_1->control->kick = 1;
			break;
			case ALLEGRO_KEY_PAD_1:
				if (!player_2->cooldown)
					player_2->control->punch = 1;
			break;
			case ALLEGRO_KEY_PAD_2:
				if (!player_2->cooldown)
					player_2->control->kick = 1;
			break;
		}
	}
	else if (event.type == 12) {
		switch (event.keyboard.keycode) {
			case ALLEGRO_KEY_A:
				player_1->control->left = 0;
			break;
			case ALLEGRO_KEY_D:
				player_1->control->right = 0;
			break;
			case ALLEGRO_KEY_W:
				player_1->control->up = 0;
			break;
			case ALLEGRO_KEY_S:
				player_1->control->down = 0;
			break;
			case ALLEGRO_KEY_LEFT:
				player_2->control->left = 0;
			break;
			case ALLEGRO_KEY_RIGHT:
				player_2->control->right = 0;
			break;
			case ALLEGRO_KEY_UP:
				player_2->control->up = 0;
			break;
			case ALLEGRO_KEY_DOWN:
				player_2->control->down = 0;
			break;
		}
	}
}

// Desenha os sprites do player
void draw_player (unsigned int center, Player *player, unsigned long int frame, unsigned char end_game_timer)
{
	// Definir novas dimensões para a imagem
	int new_width = player->box->width;
	int new_height = player->box->height;
	
	//shadow
	al_draw_filled_ellipse((int) player->box->x - (center - X_SCREEN/2), FLOOR, player->box->width * 2 *  player->box->y/FLOOR , 10,  al_map_rgba(0, 0, 0, 180 *  player->box->y/FLOOR));

	if (player->hp <= 0) {
		int i;
		if (end_game_timer > END_TIME - 5)
			i = 0;
		else if (end_game_timer > END_TIME - 15)
			i = 1;
		else 
			i = 2;
		al_draw_scaled_bitmap(player->sprites,
			player->actions->death->props[i]->x, player->actions->death->props[i]->y,  player->actions->death->props[i]->width, player->actions->death->props[i]->height, // fonte
	  		(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - player->box->height /2 * player->actions->death->props[i]->height / 75,
	  		-(new_width - (2*player->face * new_width))*PROPORTION * player->actions->death->props[i]->width / 75, new_height * player->actions->death->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->stuned) {
		if (player->crouch) {
			al_draw_scaled_bitmap(player->sprites,
				player->actions->stuned->props[1]->x, player->actions->stuned->props[1]->y,  player->actions->stuned->props[1]->width, player->actions->stuned->props[1]->height, // fonte
	  			(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - (player->box->height + player->box->height /4) * player->actions->stuned->props[1]->height / 75,
	  			-(new_width - (2*player->face * new_width)) *PROPORTION * player->actions->stuned->props[1]->width / 75, new_height*2 * player->actions->stuned->props[1]->height / 75,     // destino
	   			0);
		}
		else
			al_draw_scaled_bitmap(player->sprites,
				player->actions->stuned->props[0]->x, player->actions->stuned->props[0]->y,  player->actions->stuned->props[0]->width, player->actions->stuned->props[0]->height, // fonte
	  			(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - player->box->height /2 * player->actions->stuned->props[0]->height / 75,
	  			-(new_width - (2*player->face * new_width)) *PROPORTION * player->actions->stuned->props[0]->width / 75, new_height * player->actions->stuned->props[0]->height / 75,     // destino
	   			0);
	}
	else if (player->punch->action_time) {
		int i = (player->punch->attack_time - player->punch->action_time) / (player->punch->attack_time / player->actions->punch->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->punch->props[i]->x, player->actions->punch->props[i]->y,  player->actions->punch->props[i]->width, player->actions->punch->props[i]->height, // fonte
	  		(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - player->box->height /2 * player->actions->punch->props[i]->height / 75,
	  		-(new_width - (2*player->face * new_width)) *PROPORTION * player->actions->punch->props[i]->width / 75, new_height * player->actions->punch->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->air_punch->action_time) {
		int i = (player->air_punch->attack_time - player->air_punch->action_time) / (player->air_punch->attack_time / player->actions->air_punch->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->air_punch->props[i]->x, player->actions->air_punch->props[i]->y,  player->actions->air_punch->props[i]->width, player->actions->air_punch->props[i]->height, // fonte
	  		(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - player->box->height /2 * player->actions->air_punch->props[i]->height / 75,
	  		-(new_width - (2*player->face * new_width))*PROPORTION * player->actions->air_punch->props[i]->width / 75, new_height * player->actions->air_punch->props[i]->height / 75,     // destino
	   		0);

	}
	else if (player->crouch_punch->action_time) {
		int i = (player->crouch_punch->attack_time - player->crouch_punch->action_time) / (player->crouch_punch->attack_time / player->actions->crouch_punch->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->crouch_punch->props[i]->x, player->actions->crouch_punch->props[i]->y,  player->actions->crouch_punch->props[i]->width, player->actions->crouch_punch->props[i]->height, // fonte
	  		(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - (player->box->height + player->box->height/4) * player->actions->crouch_punch->props[i]->height / 75,
	  		-(new_width - (2*player->face * new_width))*PROPORTION * player->actions->crouch_punch->props[i]->width / 75, new_height*2 * player->actions->crouch_punch->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->kick->action_time) {
		int i = (player->kick->attack_time - player->kick->action_time) / (player->kick->attack_time / player->actions->kick->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->kick->props[i]->x, player->actions->kick->props[i]->y,  player->actions->kick->props[i]->width, player->actions->kick->props[i]->height, // fonte
	  		(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - player->box->height /2 * player->actions->kick->props[i]->height / 75 ,
	  		-(new_width - (2*player->face * new_width))*PROPORTION * player->actions->kick->props[i]->width / 75,  new_height * player->actions->kick->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->air_kick->action_time) {
		int i = (player->air_kick->attack_time - player->air_kick->action_time) / (player->air_kick->attack_time / player->actions->air_kick->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->air_kick->props[i]->x, player->actions->air_kick->props[i]->y,  player->actions->air_kick->props[i]->width, player->actions->air_kick->props[i]->height, // fonte
	  		(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - player->box->height /2 * player->actions->air_kick->props[i]->height / 75,
	  		-(new_width - (2*player->face * new_width))*PROPORTION * player->actions->air_kick->props[i]->width / 75, new_height * player->actions->air_kick->props[i]->height / 75,     // destino
	   		0);

	}
	else if (player->crouch_kick->action_time) {
		int i = (player->crouch_kick->attack_time - player->crouch_kick->action_time) / (player->crouch_kick->attack_time / player->actions->crouch_kick->quantity);
		al_draw_scaled_bitmap(player->sprites,
			player->actions->crouch_kick->props[i]->x, player->actions->crouch_kick->props[i]->y,  player->actions->crouch_kick->props[i]->width, player->actions->crouch_kick->props[i]->height, // fonte
	  		(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y  - (player->box->height + player->box->height/4) * player->actions->crouch_kick->props[i]->height / 75,
	  		-(new_width - (2*player->face * new_width))*PROPORTION * player->actions->crouch_kick->props[i]->width / 75, new_height*2 *player->actions->crouch_kick->props[i]->height / 75,     // destino
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
	  		(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - player->box->height /2 * player->actions->jump->props[i]->height / 75,
	  		-(new_width - (2*player->face * new_width))*PROPORTION * player->actions->jump->props[i]->width / 75, new_height *  player->actions->jump->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->crouch) {
		int i = 0;
		al_draw_scaled_bitmap(player->sprites,
			player->actions->crouch->props[i]->x, player->actions->crouch->props[i]->y,  player->actions->crouch->props[i]->width, player->actions->crouch->props[i]->height, // fonte
	  		(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - (player->box->height + player->box->height /4) * player->actions->crouch->props[i]->height / 75,
	  		-(new_width - (2*player->face * new_width))*PROPORTION * player->actions->crouch->props[i]->width / 75, new_height*2 * player->actions->crouch->props[i]->height / 75,     // destino
	   		0);
	}
	else if (player->movSpeed) {
		int i = frame/2 % player->actions->walk->quantity;
		al_draw_scaled_bitmap(player->sprites,
			player->actions->walk->props[i]->x, player->actions->walk->props[i]->y,  player->actions->walk->props[i]->width, player->actions->walk->props[i]->height, // fonte
	  		(int) (player->box->x - (center - X_SCREEN/2)+ (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - player->box->height /2 * player->actions->walk->props[i]->height / 75,
	  		-(new_width - (2*player->face * new_width))*PROPORTION * player->actions->walk->props[i]->width / 75, new_height * player->actions->walk->props[i]->height / 75,     // destino
	   		0);
	}
	else {
		int i = frame/6 % player->actions->standing->quantity;
		al_draw_scaled_bitmap(player->sprites,
			player->actions->standing->props[i]->x, player->actions->standing->props[i]->y,  player->actions->standing->props[i]->width, player->actions->standing->props[i]->height, // fonte
	  		(int) (player->box->x - (center - X_SCREEN/2) + (player->box->width - (2*player->face * player->box->width)) *2), player->box->y - player->box->height /2 * player->actions->standing->props[i]->height / 75,
	  		-(new_width - (2*player->face * new_width))*PROPORTION * player->actions->standing->props[i]->width / 75, new_height * player->actions->standing->props[i]->height / 75,     // destino
	   		0);
	}
}

// Desenha o mapa de fundo
void draw_background (unsigned long frame, unsigned int center, ALLEGRO_BITMAP *background, unsigned char background_count)
{
	unsigned short background_width = al_get_bitmap_width(background) / background_count;
	unsigned short	background_height = al_get_bitmap_height(background);

	al_draw_scaled_bitmap(background,
		((frame / 5 % background_count) * background_width) + (center - X_SCREEN /2)/ RATIO, 0, 429, background_height, // fonte
	  	0, 0, X_SCREEN, Y_SCREEN,     // destino
	   	0);
}


void start_round (ALLEGRO_FONT *font, unsigned char start_game_timer, unsigned char round)
{
	if (start_game_timer % 3) {
		if (start_game_timer > START_ROUND_TIME/2)
			al_draw_textf(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, 200, ALLEGRO_ALIGN_CENTER, "ROUND %d", round);
		else
			al_draw_textf(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, 200, ALLEGRO_ALIGN_CENTER, "FIGHT");
	}
}

// Menssagem de fim de round
void end_game (ALLEGRO_FONT *font, unsigned char end_game_timer, unsigned char victory)
{
	if (end_game_timer < 50 && end_game_timer%3) {
		if (!victory)
			al_draw_textf(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, 200, ALLEGRO_ALIGN_CENTER, "ROUND DRAW");
		else
			al_draw_textf(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, 200, ALLEGRO_ALIGN_CENTER, "PLAYER %d WINS", victory);
	}
}

// Desenha os status dos players
void draw_status (ALLEGRO_FONT *font, Player *player_1, Player *player_2, unsigned char counter,  unsigned char p1wins, unsigned char p2wins) 
{
	//draw timer
	al_draw_textf(font, al_map_rgb(255, 255, 255), X_SCREEN / 2, 50, ALLEGRO_ALIGN_CENTER, "%d", counter);

	int max_hp_bar = X_SCREEN / 2 -20;
	int max_stamina_bar = X_SCREEN / 4 -10;


	if (p1wins > 0)
		al_draw_filled_rectangle(20, 70, 30, 80, al_map_rgb(255, 0, 0));
	if (p1wins > 1)
		al_draw_filled_rectangle(35, 70, 45, 80, al_map_rgb(255, 0, 0));
	al_draw_rectangle(20, 70, 30, 80, al_map_rgb(255, 255, 255), 2);
	al_draw_rectangle(35, 70, 45, 80, al_map_rgb(255, 255, 255), 2);
	
	if (p2wins > 0)
		al_draw_filled_rectangle(X_SCREEN - 20, 70, X_SCREEN -30, 80, al_map_rgb(0, 0, 255));
	if (p2wins > 1)
		al_draw_filled_rectangle(X_SCREEN - 35, 70, X_SCREEN - 45, 80, al_map_rgb(0, 0, 255));
	al_draw_rectangle(X_SCREEN - 20, 70, X_SCREEN -30, 80, al_map_rgb(255, 255, 255), 2);
	al_draw_rectangle(X_SCREEN - 35, 70, X_SCREEN - 45, 80, al_map_rgb(255, 255, 255), 2);

	//hp bars
	if (player_1->hp >= 0)
        al_draw_filled_rectangle(10, 20, 10 + (player_1->hp * max_hp_bar / MAX_HP), 40, player_1->color);
    al_draw_rectangle(10, 40, X_SCREEN /2 -10, 20, al_map_rgb (255, 255, 255), 2);
	if (player_2->hp >= 0)
        al_draw_filled_rectangle(X_SCREEN - 10 - (player_2->hp * max_hp_bar / MAX_HP), 20, X_SCREEN - 10, 40, player_2->color);
    al_draw_rectangle(X_SCREEN/2 +10, 40, X_SCREEN -10, 20, al_map_rgb (255, 255, 255), 2);

	//stamina bars
	if (player_1->stamina >= 0)
		al_draw_filled_rectangle(10, 50, 10 + (player_1->stamina * max_stamina_bar / MAX_STAMINA), 60, al_map_rgb(255, 255, 100));
	al_draw_rectangle(10, 50, X_SCREEN /4, 60, al_map_rgb (255, 255, 255), 2);

	if (player_2->stamina >= 0)
		al_draw_filled_rectangle(X_SCREEN -10 - (player_2->stamina* max_stamina_bar / MAX_STAMINA), 50 , X_SCREEN -10, 60, al_map_rgb(255, 255, 100));
	al_draw_rectangle(X_SCREEN - X_SCREEN/ 4, 50, X_SCREEN -10, 60, al_map_rgb (255, 255, 255), 2);

}

// Loop de execucao do jogo
int gameLoop (Player *player_1, Player *player_2, ALLEGRO_BITMAP *background, unsigned char background_count, Essentials *essentials)
{
	unsigned long long frame = 0;
	unsigned int counter = ROUND_TIME; // duracao do round
	unsigned char start_game_timer = START_ROUND_TIME; // tempo do inicio de round
	unsigned char end_game_timer = END_TIME; // tempo de fim de round

	unsigned char p1k = 0, p2k = 0;
	unsigned char p1wins = 0, p2wins = 0; 
	unsigned char round = 1;
	
	unsigned char character; // variavel auxiliar para destruir personagens
	int menu_control; // flag para saida de menus

	int center; 

	while(1){	
		al_wait_for_event(essentials->queue, &(essentials->event));
			
		if (p1wins == 2 || p2wins == 2){
			if (p1wins == 2 && p2wins == 2){ // draw
				menu_control = endGameMenu(0, essentials);
				if (menu_control == 2)
					return 0;
				if (menu_control == 0)
					break;
			} 
			else if (p1wins == 2) { // player 2 wins
				menu_control = endGameMenu(1, essentials);
				if (menu_control == 2)
					return 0;
				if (menu_control == 1)
					return 1;
			} 
			else { // player 2 wins
				menu_control = endGameMenu(2, essentials);
				if (menu_control == 2)
					return 0;
				if (menu_control == 1)
					return 1;
			}
			p1wins = p2wins = 0;
			round = 1;																			//Indique o modo de conclusão do programa

			if ((essentials->event.type == 10) && (essentials->event.keyboard.keycode == 75)) break;
			else if (essentials->event.keyboard.keycode == ALLEGRO_KEY_ENTER || essentials->event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {}
			else if (essentials->event.type == 42) break;
		}
		else if (round > 3) {// draw
			if (p1wins == p2wins ){ 
				menu_control = endGameMenu(0, essentials);
				if (menu_control == 2)
					return 0;
				if (menu_control == 0)
					break;	
			}
			else if (p1wins > p2wins) {// player 1 wins
				menu_control = endGameMenu(1, essentials);
				if (menu_control == 2)
					return 0;
				if (menu_control == 0)
					break;
			}	
			else { // wins reset
				menu_control = endGameMenu(2, essentials);
				if (menu_control == 2)
					return 0;
				if (menu_control == 0)
					break;
			}
			p1wins = p2wins = 0; // wins reset
			round = 1;
			if (essentials->event.type == 42) break;
		}
		else{
			if (essentials->event.type == 30){

				frame++;

				//Encontra o centro da batalha no eixo X
				if ( ((player_1->box->x + player_2->box->x)/2) < (X_SCREEN/2))
					center = X_SCREEN/2;
				else if ( ((player_1->box->x + player_2->box->x)/2) > (X_MAP - X_SCREEN/2 ))
					center = X_MAP - X_SCREEN/2;
				else
					center = (player_1->box->x + player_2->box->x)/2;

				//Desenha os elementos do jogo na tela
				al_clear_to_color(al_map_rgb(0, 0, 0));
				draw_background (frame, center, background, background_count);
				draw_player (center, player_2, frame, end_game_timer);
				draw_player (center, player_1, frame, end_game_timer);
				draw_status (essentials->font, player_1, player_2, counter, p1wins, p2wins);

				// se nao eh inicio de round atualiza posicoes, ataques e status
				if (start_game_timer) {
					start_round (essentials->font, start_game_timer, round);
					start_game_timer--;
				}
				else {					
					update (frame, player_1, player_2);
					update (frame, player_2, player_1);
					if (player_1->hp <= 0) p1k = 1;
					if (player_2->hp <= 0) p2k = 1;
				}

				if (!counter) {
					if (player_1->hp > player_2->hp) {
						end_game (essentials->font, end_game_timer, 1);
					}
					else if (player_2->hp > player_1->hp)
						end_game (essentials->font, end_game_timer, 2);
					else
						end_game (essentials->font, end_game_timer, 0);
				}
				else {
					if (p1k)
						end_game (essentials->font, end_game_timer, 2);
					if (p2k)
						end_game (essentials->font, end_game_timer, 1);
				}

				if (p1k || p2k || !counter) { // win condition
					joystick_reset (player_1->control);
					joystick_reset (player_2->control);

					if (!end_game_timer) {
						if (!counter) {
							if (player_1->hp > player_2->hp)
								p1wins++;
							else if (player_1->hp > player_2->hp)
								p2wins++;
						}
						else {
							if (p2k)
								p1wins++;
							if (p1k) 
								p2wins++;
						}
						p1k = p2k = 0;
						round++;
						
						character = player_1->character;
						player_destroy(player_1);
						player_1 = character_load (1, character);

						character = player_2->character;
						player_destroy(player_2);
						player_2 = character_load (2, character);
						
						counter = ROUND_TIME;
						start_game_timer = START_ROUND_TIME;
						end_game_timer = END_TIME;
					}
					else
						end_game_timer--;
				}

				if (frame % 30 == 0 && counter && !start_game_timer)
					counter--;
	    		al_flip_display();
			}
			if (essentials->event.type == 10 && essentials->event.keyboard.keycode == ALLEGRO_KEY_P){
				joystick_reset (player_1->control);
				joystick_reset (player_2->control);
				if ( (menu_control = menu_pause (essentials)) == 2) {
					player_destroy(player_1);
					player_destroy(player_2);
					return 0;
				}
				if (menu_control == 1) {
					player_destroy(player_1);																																												
					player_destroy(player_2);
					return 1;
				}
			}																		
			else if (essentials->event.type == 42) return 0;
			if (!p1k && !p2k && counter)
				control (essentials->event, player_1, player_2);
		}
	}

	return 1;
}

int main(){
	Essentials *essentials;
	if (!(essentials = malloc (sizeof (Essentials))))
		return 1;
	start_essentials (&essentials);

	Player* player_1;
	Player* player_2;
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