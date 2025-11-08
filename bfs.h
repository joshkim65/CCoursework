#define BFS_H

#include "arena.h"
#include "robot.h"

typedef struct { int r, c; } Cell;

/* BFS shortest path on a 4-neighbour grid.
   Returns 1 if found (outPath[0]=start, outPath[len-1]=goal), else 0. */
int bfsPath(Arena *a, Cell start, Cell goal, Cell outPath[], int *outLen);

/* Execute a precomputed path:
   turns with left()/right() (90°) and steps with forward().
   Picks up a marker after each step (if present). */
void followPath(Robot *r, Arena *a, Cell path[], int len);