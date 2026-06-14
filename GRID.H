#ifndef GRID_H
#define GRID_H

#define GRID_S 15
#define TILE_S 10

enum tile {
	VOID,
	FOOD,
	SNAKE
};

struct apple {
	int x, y;
};

#endif
