#include "bfs.h"
#include <string.h>  

// constants defining each direction to make it easier to calculate movements later
static const int RO[4] = { -1, 0, 1, 0 };
static const int CO[4] = {  0, 1, 0,-1 };

// Determine which direction leads from one cell to another
static Direction stepDir(int dr, int dc) {
    if (dr == -1 && dc ==  0) return UP;
    if (dr ==  1 && dc ==  0) return DOWN;
    if (dr ==  0 && dc ==  1) return RIGHT;
    return LEFT; // dr==0 && dc==-1
}

// Turn the shortest way 
static void face(Robot *r, Direction want) {
    int d = ((int)want - (int)r->dir + 4) % 4; // 0..3
    if (d == 1) { right(r); drawRobot(r, r->dir); }
    else if (d == 2) { right(r); drawRobot(r, r->dir); right(r); drawRobot(r, r->dir); }
    else if (d == 3) { left(r);  drawRobot(r, r->dir); }
    // d==0 → already facing
}

int isInsideArena(Arena *arena, int row, int col) {
    return row >= 0 && row < arena->rows && col >= 0 && col < arena->cols;
}

int bfsPath(Arena *arena, Cell startCell, Cell goalCell, Cell outPath[], int *outPathLength) {
    unsigned char visited[MAX_ROWS][MAX_COLS];
    memset(visited, 0, sizeof(visited));

    Cell parent[MAX_ROWS][MAX_COLS];
    for (int r = 0; r < arena->rows; r++) {
        for (int c = 0; c < arena->cols; c++) {
            parent[r][c] = (Cell){ -1, -1 };
        }
    }
    Cell queue[MAX_ROWS * MAX_COLS];
    int front = 0, back = 0;

    visited[startCell.r][startCell.c] = 1;
    queue[back++] = startCell;

    int found = 0;

    // using a queue implementation of bfs
    while (front < back) {
        Cell current = queue[front++];

        if (current.r == goalCell.r && current.c == goalCell.c) {
            found = 1;
            break;
        }

        for (int dir = 0; dir < 4; dir++) {
            int nextRow = current.r + RO[dir];
            int nextCol = current.c + CO[dir];

            if (!isInsideArena(arena, nextRow, nextCol)) continue;
            if (visited[nextRow][nextCol]) continue;
            if (arena->grid[nextRow][nextCol] == obstacle) continue;

            visited[nextRow][nextCol] = 1;
            parent[nextRow][nextCol]  = current;
            queue[back++] = (Cell){ nextRow, nextCol };
        }
    }

    if (!found) { *outPathLength = 0; return 0; }

    // Reconstruct path (goal → start)
    int length = 0;
    for (Cell p = goalCell; p.r != -1; p = parent[p.r][p.c]) {
        outPath[length++] = p;
    }
    // Reverse to (start → goal)
    for (int i = 0; i < length / 2; i++) {
        Cell temp = outPath[i];
        outPath[i] = outPath[length - 1 - i];
        outPath[length-1-i] = temp;
    }

    *outPathLength = length;
    return 1;
}

void followPath(Robot *r, Arena *a, Cell path[], int len) {
    for (int i = 1; i < len; i++) {
        int dr = path[i].r - path[i-1].r;
        int dc = path[i].c - path[i-1].c;

        Direction dir = stepDir(dr, dc);
        face(r, dir);
        forward(r, a);
    }
}
