/*
 * MAIN.C -- Holds the game status, the snake, the apples...
 *
 * Copyright (C) 2026
 * Author: Pablo Trik Marín (trmaa) <mail@pablotrik.online>
 * License: GPL 3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include "vgalib.h"
#include "snake.h"
#include "grid.h"

static const int H_OFF = SCREEN_W / 2 - GRID_S * TILE_S / 2;
static const int V_OFF = SCREEN_H / 2 - GRID_S * TILE_S / 2;

enum status {
	WIN = 0,
	DEFEAT = 69,
	RUNNING = -1,
	ERROR = 2
};

enum key {
	K_UP = 0x48,
	K_DOWN = 0x50,
	K_LEFT = 0x4B,
	K_RIGHT = 0x4D
};

static enum tile grid[GRID_S][GRID_S] = {0};
static struct snake *snake = NULL;
static struct apple apple;
static const int MAX_POINTS = GRID_S * GRID_S;
static int points = 0;
static int key = 0;
static enum status game_status = RUNNING;

static void draw_tile(int x, int y, unsigned char col);

static void
place_apple(void)
{
	snake_node *sn;

	do {
		apple.x = rand() % GRID_S;
		apple.y = rand() % GRID_S;
		for (sn = snake; sn != NULL; sn = sn->next)
			if (sn->pos.x == apple.x && sn->pos.y == apple.y)
				break;
	} while (sn != NULL);
}

static void
start_game()
{
	clrscr();
	printf("Welcome to SNAKE!\n");
	printf("press (space) to continue. or (q) to quit.\n");

	while ((key = getch()) != ' ')
		if (key == 'q')
			exit(0);

	snake = malloc(sizeof (snake_node));
	init_snake(snake, 10, 10);

	srand((unsigned)time(NULL));
	place_apple();
}

static void
end_game()
{
	destroy_snake(snake);

	clrscr();

	switch (game_status) {
	case WIN:
		printf("Congrats!\n");
		break;
	case DEFEAT:
		printf("What a looser! :v\n");
		break;
	}

	getch();
}

static void
loop_game()
{
	int r, c;
	snake_node *sn = snake;

	for (r = 0; r < GRID_S; r++)
		for (c = 0; c < GRID_S; c++)
			grid[r][c] = VOID;

	grid[apple.y][apple.x] = FOOD;

	switch (key) {
	case K_UP:
		if (snake->dir != DOWN)
			snake->dir = UP;
		break;
	case K_DOWN:
		if (snake->dir != UP)
			snake->dir = DOWN;
		break;
	case K_LEFT:
		if (snake->dir != RIGHT)
			snake->dir = LEFT;
		break;
	case K_RIGHT:
		if (snake->dir != LEFT)
			snake->dir = RIGHT;
		break;
	default:
		break;
	}

	move_snake(snake);

	if (snake->pos.x >= GRID_S)
		game_status = DEFEAT;
	if (snake->pos.x < 0)
		game_status = DEFEAT;
	if (snake->pos.y >= GRID_S)
		game_status = DEFEAT;
	if (snake->pos.y < 0)
		game_status = DEFEAT;

	if (snake->pos.x == apple.x && snake->pos.y == apple.y) {
		snake = largen_snake(snake);
		place_apple();
		points++;
	}

	while (sn != NULL) {
		if (
			sn->pos.x == snake->pos.x &&
			sn->pos.y == snake->pos.y &&
			sn != snake
		)
			game_status = DEFEAT;
		
		grid[sn->pos.y][sn->pos.x] = SNAKE;
		sn = sn->next;
	}

	if (points >= MAX_POINTS)
		game_status = WIN;
}

static void
render_game()
{
	int r, c;
	int x, y;

	for (r = 0; r < GRID_S; r++) {
		for (c = 0; c < GRID_S; c++) {
			x = c * TILE_S + H_OFF;
			y = r * TILE_S + V_OFF;

			switch (grid[r][c]) {
			case VOID:
				draw_tile(x, y, VGA_RGB(100, 100, 100));
				break;	
			case FOOD:
				draw_tile(x, y, VGA_RGB(255, 0, 0));
				break;
			case SNAKE:
				draw_tile(x, y, VGA_RGB(0, 255, 0));
				break;
			}
		}
	}
}

main(void)
{
	start_game();

	vga_init();
	vga_init_cube();

	{
		unsigned i;
		for (i = 0; i < SCREEN_W * SCREEN_H; i++)
			VGA_MEM[i] = 0;
	}

	while (key != 'q' && game_status == RUNNING) {
		loop_game();
		render_game();

		if (kbhit()) {
			key = getch();
			if (key == 0x00 || key == 0xE0)
				key = getch();
		}

		delay(30000);
	}
	
	printf("Score: %d\n", points);
	getch();

	vga_close();
	
	end_game();
}

static void
draw_tile(int x, int y, unsigned char col)
{
	int tx = x + TILE_S;
	int ty = y + TILE_S;
	int px, py;

	for (py = y; py < ty; py++)
		for (px = x; px < tx; px++)
			SET_PX(px, py, col);
}
