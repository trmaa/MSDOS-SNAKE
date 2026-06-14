#ifndef SNAKE_H
#define SNAKE_H

enum direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct coords {
	int x, y;
};

typedef struct snake snake_node;

struct snake {
	enum direction dir;
	struct coords pos;

	snake_node *next;
	snake_node *prev;
};

void init_snake(struct snake *head, int x, int y);
void destroy_snake(struct snake *head);

snake_node *largen_snake(snake_node *head);
void move_snake(struct snake *head);

#endif
