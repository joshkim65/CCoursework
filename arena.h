#ifndef ARENA_H 
#define ARENA_H

#define MAX_ROWS 20
#define MAX_COLS 20
#define TILE_SIZE 40

typedef enum {
    empty = 0,
    marker = 1,
    obstacle = 2
} Tile;

typedef enum {
    MAP_EMPTY = 0,        
    MAP_RANDOM = 1,       
    MAP_CIRCLE = 2        
} MapMode;

typedef struct {
    int rows;
    int cols;
    int grid[MAX_ROWS][MAX_COLS];
} Arena;

void initArena(Arena *a, int rows, int cols);
void drawGrid(Arena *a);
void drawBorder(Arena *a);

void placeRandomMarkers(Arena *a, int count);

void placeRandomObstacles(Arena *a, int count);
void placeCircularBoundary(Arena *a, int radius);

void generateMap(Arena *a, MapMode mode, int markerCount, int circleRadius);

#endif