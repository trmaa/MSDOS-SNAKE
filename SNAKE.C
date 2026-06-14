#include <stdio.h>
#include <stdlib.h>
#include "snake.h"
#include "grid.h"

void
init_snake(struct snake *head, int x, int y)
{
	head->dir = LEFT;

	head->pos.x = x;
	head->pos.y = y;

	head->prev = NULL;
	head->next = malloc(sizeof (snake_node));
	if (head->next == NULL)
		return;
	head->next->prev = head;
	head->next->next = NULL;
	head->next->dir = LEFT;
	head->next->pos.x = x + 1;
	head->next->pos.y = y;
}

void
destroy_snake(snake_node *head)
{
	snake_node *e = head;
	snake_node *next;

	while (e != NULL) {
		next = e->next;
		free(e);
		e = next;
	}
}

static void
move_snake_inm(snake_node *e)
{
	if (e->next == NULL)
		return;	

	move_snake_inm(e->next);

	e->next->dir = e->dir;
	e->next->pos = e->pos;
}

void
move_snake(struct snake *head)
{
	struct coords *pos = &head->pos;

	move_snake_inm(head);	
	
	switch (head->dir) {
	case UP:
		pos->y -= 1;			
		break;
	case DOWN:
		pos->y += 1;			
		break;
	case LEFT:
		pos->x -= 1;			
		break;
	case RIGHT:
		pos->x += 1;			
		break;
	}
}

snake_node *
largen_snake(snake_node *head)
{
	snake_node *new_head = malloc(sizeof (snake_node));

	if (new_head == NULL)
		return head;

	new_head->dir = head->dir;
	new_head->pos = head->pos;
	new_head->prev = NULL;
	new_head->next = head;
	head->prev = new_head;

	switch (head->dir) {
	case UP:
		new_head->pos.y -= 1;
		break;
	case DOWN:
		new_head->pos.y += 1;
		break;
	case LEFT:
		new_head->pos.x -= 1;
		break;
	case RIGHT:
		new_head->pos.x += 1;
		break;
	}

	return new_head;
}
