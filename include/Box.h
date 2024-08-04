#ifndef __BOX__
#define __BOX__	

typedef struct box {
	unsigned short width;
	unsigned short height;
	unsigned short x;
	unsigned short y;
} box;

// cria uma caixa com area e cordenadas
box* box_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y);

// desaloca memoria da caixa
void box_destroy(box *element);

#endif