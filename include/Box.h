#ifndef __BOX__
#define __BOX__	

typedef struct box {
	unsigned short width;
	unsigned short height;
	unsigned short x;
	unsigned short y;
} box;

box* box_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y);
void box_destroy(box *element);

#endif