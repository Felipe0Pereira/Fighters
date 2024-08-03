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

joystick* joystick_create();	
void joystick_reset (joystick *element);
void joystick_destroy(joystick *element);

#endif