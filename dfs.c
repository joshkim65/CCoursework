#include "dfs.h"

// constants defining each direction to make it easier to calculate movements later
static const int RO[4] = {-1, 0, 1, 0};
static const int CO[4] = { 0, 1, 0,-1};

typedef struct { int r, c; } Cell;

// Determine which direction leads from one cell to another
Direction stepDir(int dr, int dc) {
    if (dr == -1 && dc ==  0) return UP;
    if (dr ==  1 && dc ==  0) return DOWN;
    if (dr ==  0 && dc ==  1) return RIGHT;
    return LEFT; // dr==0 && dc==-1
}

// Turn the shortest way 
void face(Robot *r, Direction want) {
    int d = ((int)want - (int)r->dir + 4) % 4; // 0..3
    if (d == 1) { right(r); drawRobot(r, r->dir); }
    else if (d == 2) { right(r); drawRobot(r, r->dir); right(r); drawRobot(r, r->dir); }
    else if (d == 3) { left(r);  drawRobot(r, r->dir); }
    // d==0 → already facing
}

void exploreDFS(Arena *arena, Robot *robot) {
    unsigned char visited[MAX_ROWS][MAX_COLS] = {0}; 
    Cell stack[MAX_ROWS * MAX_COLS];
    int top = 0;

    visited[robot->row][robot->col] = 1;
    if (atMarker(robot, arena)) pickUpMarker(robot, arena);

    while (1) {
        int advanced = 0;

        // Try all 4 directions (UP, RIGHT, DOWN, LEFT)
        for (int d = 0; d < 4; d++) {
            int nr = robot->row + RO[d];
            int nc = robot->col + CO[d];

            // skip if out of bounds or already visited
            if (nr < 0 || nr >= arena->rows || nc < 0 || nc >= arena->cols) continue;
            if (visited[nr][nc]) continue;

            // Face the direction and check if forward movement is possible
            face(robot, (Direction)d);
            if (!canMoveForward(robot, arena)) continue; // obstacle, try next
                
            // Move physically
            forward(robot, arena);
            visited[robot->row][robot->col] = 1;

            // Pick up marker if present
            if (atMarker(robot, arena)) pickUpMarker(robot, arena);

            // Push previous cell pos for backtracking
            stack[top++] = (Cell){ robot->row - RO[d], robot->col - CO[d] };

            advanced = 1;
            break;
        }

        if (advanced) continue;

        // Dead end so backtrack if possible
        if (top == 0) break; 

        Cell back = stack[--top];
        Direction want = stepDir(back.r - robot->row, back.c - robot->col);
        face(robot, want);

        // Move back one tile
        if (canMoveForward(robot, arena)) {
            forward(robot, arena);
            if (atMarker(robot, arena)) pickUpMarker(robot, arena);
        }
    }
}
