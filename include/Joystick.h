#ifndef __JOYSTICK__ 																												//Guardas de inclusão 
#define __JOYSTICK__																												//Guardas de inclusão 

typedef struct {																													//Definição da estrutura de um controle 
	unsigned char right;																											//Botão de movimentação à direta 
	unsigned char left;																												//Botão de movimentação à esquerda 
	unsigned char up;																												//Botão de movimentação para cima 
	unsigned char down;																												//Botão de disparo																											//Botão de movimentação para baixo 
	unsigned char punch;
	unsigned char kick;
} joystick;																															//Definição do nome da estrutura 

joystick* joystick_create();	
void joystick_reset (joystick *element);																									//Protótipo da função de criação de um controle 
void joystick_destroy(joystick *element);																							//Protótipo da função de destruição de um controle 

#endif																																//Guardas de inclusão 