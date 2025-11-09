#include "robot.h"
#include "graphics.h"
#include "arena.h"
#include "dfs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     
#include <math.h>      

void initRobot(Robot *r, int startRow, int startCol, Direction startDir) {
    r->row = startRow;
    r->col = startCol;
    r->dir = startDir;
    r->markers = 0;
}

int atMarker(Robot *r, Arena *a) {
    return (a->grid[r->row][r->col] == marker);
}

int markerCount(Robot *r) {
    return r->markers;
}

void drawRobot(Robot *r, Direction dir) {
    int x = r->col * TILE_SIZE;
    int y = r->row * TILE_SIZE;

    int centreX = x + TILE_SIZE / 2;
    int centreY = y + TILE_SIZE / 2;
    
    int halfSize = (TILE_SIZE / 2) - 5;

    int xPoints[3];
    int yPoints[3];


    //calculating each point of the triangle for every facing direction
    if (dir == UP) {
        xPoints[0] = centreX;            yPoints[0] = centreY - halfSize;  // tip
        xPoints[1] = centreX - halfSize; yPoints[1] = centreY + halfSize;  // bottom left
        xPoints[2] = centreX + halfSize; yPoints[2] = centreY + halfSize;  // bottom right
    } 
    else if (dir == RIGHT) {
        xPoints[0] = centreX + halfSize; yPoints[0] = centreY;             // tip
        xPoints[1] = centreX - halfSize; yPoints[1] = centreY - halfSize;  // top left
        xPoints[2] = centreX - halfSize; yPoints[2] = centreY + halfSize;  // bottom left
    } 
    else if (dir == DOWN) {
        xPoints[0] = centreX;            yPoints[0] = centreY + halfSize;  // tip
        xPoints[1] = centreX - halfSize; yPoints[1] = centreY - halfSize;  // top left
        xPoints[2] = centreX + halfSize; yPoints[2] = centreY - halfSize;  // top right
    } 
    else { // LEFT
        xPoints[0] = centreX - halfSize; yPoints[0] = centreY;             // tip
        xPoints[1] = centreX + halfSize; yPoints[1] = centreY - halfSize;  // top right
        xPoints[2] = centreX + halfSize; yPoints[2] = centreY + halfSize;  // bottom right
    }

    setColour(blue);
    fillPolygon(3, xPoints, yPoints);

    // drawing the "tail" of the triangle
    setColour(red);
    drawLine(xPoints[1], yPoints[1], xPoints[2], yPoints[2]);

    // drawing marker count
    if (r->markers >= 0) {
        char count[12];
        sprintf(count, "%02d", r->markers);
        setColour(green);
        drawString(count, x + TILE_SIZE / 2, y + TILE_SIZE / 2);
    }
}

void left(Robot *robot) {
    robot->dir = (robot->dir + 3) % 4;  
}

void right(Robot *robot) {
    robot->dir = (robot->dir + 1) % 4;  
}

int canMoveForward(Robot *r, Arena *a) {
    int nextRow = r->row;
    int nextCol = r->col;

    if (r->dir == UP)       nextRow--;
    else if (r->dir == RIGHT) nextCol++;
    else if (r->dir == DOWN)  nextRow++;
    else if (r->dir == LEFT)  nextCol--;

    if (nextRow < 0 || nextRow >= a->rows || nextCol < 0 || nextCol >= a->cols || a->grid[nextRow][nextCol] == obstacle) {
        return 0;
    }
    return 1;
}

void forward(Robot *r, Arena *a) {
    if (!canMoveForward(r, a))
        return;

    if (r->dir == UP)       r->row--;
    else if (r->dir == RIGHT) r->col++;
    else if (r->dir == DOWN)  r->row++;
    else if (r->dir == LEFT)  r->col--;

    foreground();
    clear();
    drawRobot(r, r->dir);
    sleep(100);
}

void pickUpMarker(Robot *r, Arena *a) {
    if (a->grid[r->row][r->col] == marker) {
        a->grid[r->row][r->col] = empty;

        int x = r->col * TILE_SIZE;
        int y = r->row * TILE_SIZE;

        // drawing over the marker with a white rectangle to erase it when it is picked up
        background();
        int padding = 2; 
        setColour(white);
        fillRect(x + padding, y + padding, TILE_SIZE - 2*padding, TILE_SIZE - 2*padding);
        
        //drawing robot with new marker count
        foreground();
        clear();
        drawRobot(r, r->dir);

        r->markers++;
    }
}


void dropMarker(Robot *r, Arena *a) {
    if (r->markers <= 0) return;
    if (a->grid[r->row][r->col] != marker) {
        a->grid[r->row][r->col] = marker;

        int x = r->col * TILE_SIZE, y = r->row * TILE_SIZE;
        int radius = TILE_SIZE / 4;

        // drawing dropped marker in background
        background(); 
        setColour(gray);
        fillOval(x + TILE_SIZE/2 - radius, y + TILE_SIZE/2 - radius, 2*radius, 2*radius);
    }
    r->markers--;

    // drawing robot with new marker count 
    foreground();
    clear();
    drawRobot(r, r->dir);
}


int isInBounds(Arena *a, int row, int col) {
    return (row >= 0 && row < a->rows && col >= 0 && col < a->cols);
}

void floodFill(
    Arena *a,
    int row, int col,
    int startRow, int startCol,
    int visited[MAX_ROWS][MAX_COLS],
    int *bestRow, int *bestCol, int *bestDistance
) {
    if (!isInBounds(a, row, col) || visited[row][col])
        return;

    visited[row][col] = 1;

    if (a->grid[row][col] != obstacle) {
        int distance = abs(row - startRow) + abs(col - startCol);
        if (distance < *bestDistance) {
            *bestDistance = distance;
            *bestRow = row;
            *bestCol = col;
        }
    }

    // recursively visits every cell around the starting point to find the nearest free cell
    floodFill(a, row - 1, col, startRow, startCol, visited, bestRow, bestCol, bestDistance);
    floodFill(a, row + 1, col, startRow, startCol, visited, bestRow, bestCol, bestDistance);
    floodFill(a, row, col - 1, startRow, startCol, visited, bestRow, bestCol, bestDistance);
    floodFill(a, row, col + 1, startRow, startCol, visited, bestRow, bestCol, bestDistance);
}

void relocateSpawnIfBlocked(Arena *a, int *row, int *col) {
    if (!a || !row || !col) return;

    // If current tile is already free then no need to run flood fill
    if (a->grid[*row][*col] != obstacle) return;

    int visited[MAX_ROWS][MAX_COLS];
    memset(visited, 0, sizeof(visited));

    int bestRow = -1, bestCol = -1;
    int bestDistance = 999999999;

    floodFill(a, *row, *col, *row, *col, visited, &bestRow, &bestCol, &bestDistance);

    if (bestRow != -1 && bestCol != -1) {
        *row = bestRow;
        *col = bestCol;
        return;
    }
}

