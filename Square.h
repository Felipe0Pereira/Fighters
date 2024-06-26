#ifndef __SQUARE__ 																																	//Guardas de inclusão
#define __SQUARE__																																	//Guardas de inclusão																															//Quantidade de pixels que um quadrado se move por passo


#include "Joystick.h"																																//Estrutura e procedimentos relacionados ao controle do quadrado
#include "Pistol.h" 																																//Estrutura e procedimentos relacionados ao controle da arma (pistola) no jogo
#include "Attacks.h"
#include "Box.h"

#define SQUARE_STEP 1																																//Tamanho, em pixels, de um passo do quadrado

typedef struct {																																	//Definição da estrutura de um quadrado
	unsigned char side;																																//Tamanmho da lateral de um quadrado
	unsigned char height;
	unsigned char face;																																//A face principal do quadrado, algo como a sua "frente"
	char vertSpeed;
	char movSpeed;
	unsigned char jump;
	char hp;
	unsigned char cooldown;																																//Quantidade de vida do quadrado, em unidades (!)
	box *box;
	joystick *control;																																//Elemento de controle do quadrado no jogo
	attacks *punch;
	attacks *kick;
	pistol *gun;																																	//Elemento para realizar disparos no jogo
} square;																																			//Definição do nome da estrutura

square* square_create(unsigned char side, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);		//Protótipo da função de criação de um quadrado
void square_move(square *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);								//Protótipo da função de movimentação de um quadrado
void square_shot(square *element);																													//Protótipo da função de disparo de um quadrado
void square_destroy(square *element);																												//Protótipo da função de destruição de um quadrado

#endif																																				//Guardas de inclusão