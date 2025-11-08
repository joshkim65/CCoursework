#include "arena.h"
#include "graphics.h"
#include <stdlib.h>
#include <time.h>

void initArena(Arena *a, int rows, int cols) {
    a->rows = rows;
    a->cols = cols;
    
    // 2d array representation of the grid
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            a->grid[r][c] = empty;
        }
    }
}

void drawGrid (Arena *a) {
    background();
    clear();

    int rows = a->rows;
    int cols = a->cols;

    // drawing a grey rectangle for every cell
    setColour(lightgray);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int x = c * TILE_SIZE;
            int y = r * TILE_SIZE;

            drawRect(x, y, TILE_SIZE, TILE_SIZE);
        }
    }
}

void placeRandomMarkers(Arena *a, int count) {
    int rows = a->rows;
    int cols = a->cols;
    
    for (int k = 0; k < count; k++) {
        int r, c;
        do {
            // using modulo to get random coords for markers
            r = rand() % rows;
            c = rand() % cols;
        } while (a->grid[r][c] != empty);


        a->grid[r][c] = marker;

        int x = c * TILE_SIZE;
        int y = r * TILE_SIZE;

        setColour(gray);

        int radius = TILE_SIZE / 4;
        fillOval(x + TILE_SIZE / 2 - radius, y + TILE_SIZE / 2 - radius, 2*radius, 2*radius);
    }
}

void placeRandomObstacles(Arena *a, int count) {
    for (int i = 0; i < count; i++) {
        int r, c;

        do {
            r = rand() % a->rows;
            c = rand() % a->cols;
        } while (a->grid[r][c] != empty);

        a->grid[r][c] = obstacle;

        int x = c * TILE_SIZE;
        int y = r * TILE_SIZE;
        int margin = 5; 
        setColour(black);
        fillRect(x + margin, y + margin, TILE_SIZE - 2*margin, TILE_SIZE - 2*margin);
    }
}

void placeCircularBoundary(Arena *a, int radius) {
    int centerR = (a->rows - 1) / 2;
    int centerC = (a->cols - 1) / 2;
    int maxPossible = 0;

    if (a->rows < a->cols) {
        maxPossible = a->rows / 2 - 1;
    } else {
        maxPossible = a->cols / 2 - 1;
    }


    // calculates distance from the centre of the circle to every cell, marking every cell outside of the distance as an obstacle and every cell within
    // the circle as empty. 
    if (radius <= 0 || radius > maxPossible) radius = maxPossible;
    int r2 = radius * radius;

    for (int r = 0; r < a->rows; r++) {
        for (int c = 0; c < a->cols; c++) {

            int dr = r - centerR;
            int dc = c - centerC;
            int dist2 = dr*dr + dc*dc;

            if (dist2 > r2) {
                a->grid[r][c] = obstacle;
                int x = c * TILE_SIZE, y = r * TILE_SIZE;
                int margin = 5;
                setColour(black);
                fillRect(x+margin, y+margin, TILE_SIZE-2*margin, TILE_SIZE-2*margin);
            }
        }
    }
}

