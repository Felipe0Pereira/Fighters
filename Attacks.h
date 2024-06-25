#ifndef __ATTACKS__ 																												//Guardas de inclusão 
#define __ATTACKS__																												//Guardas de inclusão 

typedef struct {
	unsigned char x;
	unsigned char y;
} box;

typedef struct {																													//Definição da estrutura de um controle 
	box *punch ;																											//Botão de movimentação à direta 
	box *kick;																												//Botão de movimentação à esquerda 
} attacks;

attacks* attacks_create();	
void attacks_destroy(attacks *element);

#endif	