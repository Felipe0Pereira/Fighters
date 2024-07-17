#ifndef __SQUARE__ 																																	//Guardas de inclusão
#define __SQUARE__																																	//Guardas de inclusão																															//Quantidade de pixels que um quadrado se move por passo

#include <allegro5/allegro5.h>																																												//Biblioteca base do Allegro
#include <allegro5/allegro_image.h>																																								//Biblioteca de figuras básicas


#include "Joystick.h"																																//Estrutura e procedimentos relacionados ao controle do quadrado
#include "Pistol.h" 																																//Estrutura e procedimentos relacionados ao controle da arma (pistola) no jogo
#include "Attacks.h"
#include "Box.h"

#define SQUARE_STEP 1																																//Tamanho, em pixels, de um passo do quadrado
#define PROPORTION 5

#define MAX_HP 5

typedef struct Sprites {
	unsigned char quantity;
	box **props;
} Sprites;

typedef struct Actions {
	Sprites *walk;
	Sprites *jump;
	Sprites *crouch;
	Sprites *punch;
	Sprites *air_punch;
	Sprites *crouch_punch;
	Sprites *kick;
	Sprites *air_kick;
	Sprites *crouch_kick;
} Actions;

typedef struct {																																	//Definição da estrutura de um quadrado
	unsigned char side;																																//Tamanmho da lateral de um quadrado
	unsigned char height;
	unsigned char face;																																//A face principal do quadrado, algo como a sua "frente"
	char vertSpeed;
	char movSpeed;
	unsigned char jump;
	unsigned char crouch;
	char hp;
	char stamina;
	unsigned char cooldown;																																//Quantidade de vida do quadrado, em unidades (!)
	box *box;
	joystick *control;																																//Elemento de controle do quadrado no jogo
	attacks *punch;
	attacks *air_punch;
	attacks *crouch_punch;
	attacks *kick;
	attacks *air_kick;
	attacks *crouch_kick;
	pistol *gun;	
	Actions *actions;	
	ALLEGRO_BITMAP *sprites;																														//Elemento para realizar disparos no jogo
} square;																																			//Definição do nome da estrutura

square* square_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);		//Protótipo da função de criação de um quadrado
void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);								//Protótipo da função de movimentação de um quadrado
void square_reset (square *element, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);

void square_shot(square *element);																													//Protótipo da função de disparo de um quadrado
void square_destroy(square *element);																												//Protótipo da função de destruição de um quadrado

#endif																																				//Guardas de inclusão