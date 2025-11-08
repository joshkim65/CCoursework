#ifndef ROBOT_H
#define ROBOT_H

#include "arena.h"

typedef enum {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
} Direction;

typedef struct {
    int row;
    int col;
    Direction dir;
    int markers;
} Robot;

void initRobot(Robot *r, int startRow, int startCol, Direction startDir);
void drawRobot(Robot *r, Direction dir);

void forward(Robot *r, Arena *a);
void left(Robot *r);
void right(Robot *r);
int canMoveForward(Robot *r, Arena *a);

int atMarker(Robot *r, Arena *a);
void pickUpMarker(Robot *r, Arena *a);
void dropMarker(Robot *r, Arena *a);
int  markerCount(Robot *r);

int isInBounds(Arena *a, int row, int col);
void floodFill(Arena *a, int row, int col, int startRow, int startCol, int visited[MAX_ROWS][MAX_COLS], int *bestRow, int *bestCol, int *bestDistance);
void relocateSpawnIfBlocked(Arena *a, int *row, int *col);

#endif

