#ifndef __JOYSTICK__
#define __JOYSTICK__

typedef struct {
	unsigned char right;
	unsigned char left;
	unsigned char up;
	unsigned char down;
	unsigned char punch;
	unsigned char kick;
} joystick;

// cria controles
joystick* joystick_create();	

// reseta os botoes para 0
void joystick_reset (joystick *element);

// destroi a estrutura joystick
void joystick_destroy(joystick *element);

#endif