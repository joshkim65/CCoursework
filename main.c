#include "arena.h"
#include "robot.h"
#include "graphics.h"
#include "dfs.h"
#include "bfs.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>



int main(int argc, char **argv) {
    //random seeds that doesn't need recompilation
    srand((unsigned)time(NULL));

    // default values
    int startX = 0;
    int startY = 0;
    char *inputDir = (char*)"UP";
    int startRows = 10;
    int startCols = 10;
    char *inputMapMode = (char*)"random";
    int numberOfMarkers = 10;

    Direction startDir = UP;

    // 8 args including program name
    // ./a.out x y DIR rows cols mapmode markers | java -jar drawapp-4.5.jar
    if (argc == 8) {
        startX = atoi(argv[1]);
        startY = atoi(argv[2]);
        inputDir = argv[3];
        startRows = atoi(argv[4]);
        startCols = atoi(argv[5]);
        inputMapMode = argv[6];
        numberOfMarkers = atoi(argv[7]);

        if      (strcmp(inputDir, "UP")    == 0) startDir = UP;
        else if (strcmp(inputDir, "DOWN")  == 0) startDir = DOWN;
        else if (strcmp(inputDir, "LEFT")  == 0) startDir = LEFT;
        else if (strcmp(inputDir, "RIGHT") == 0) startDir = RIGHT;
    }

    // initialising arena
    Arena a;
    initArena(&a, startRows, startCols);
    setWindowSize(a.cols * TILE_SIZE, a.rows * TILE_SIZE);
    drawGrid(&a);

    // mapmode logic
    if (strcmp(inputMapMode, "random") == 0) {
        placeRandomObstacles(&a, (a.rows * a.cols) / 4);
    } 
    else if (strcmp(inputMapMode, "circle") == 0) {
        placeCircularBoundary(&a, a.rows);
    }

    placeRandomMarkers(&a, numberOfMarkers);
    
    // using floodfill to find the nearest empty cell (if the spawn pos inputted is inside an obstacle)
    relocateSpawnIfBlocked(&a, &startY, &startX);

    // getting ready to draw robot
    foreground();
    clear();

    Robot bot;
    initRobot(&bot, startX, startY, startDir);
    drawRobot(&bot, bot.dir);

    if (atMarker(&bot, &a))
        pickUpMarker(&bot, &a);


    // traversing every cell with dfs (that are reachable)
    exploreDFS(&a, &bot);

    // finding shortest path to bottom right corner using bfs
    Cell start = (Cell){ bot.row, bot.col };
    Cell corner = (Cell){ a.rows - 1, a.cols - 1 };

    Cell path[MAX_ROWS * MAX_COLS];
    int pathLen = 0;

    if (bfsPath(&a, start, corner, path, &pathLen)) {
        followPath(&bot, &a, path, pathLen);
    }

    // dropping markers at specified corner
    while (markerCount(&bot) > 0) {
        dropMarker(&bot, &a);
        sleep(100);
    }

    sleep(1500);
    return 0;
}