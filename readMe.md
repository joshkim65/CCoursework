This program simulates a robot navigating an grid arena filled with obstacles and markers. 
The obstacles, markers and the grid itself is drawn on the background layer, while the
robot is drawn on the foreground. The robot navigates the arena by conducting a dfs on the
grid, traversing through every cell until it has found all the markers. It then calculates
a path to the bottom right corner to drop off the collected markers. It will drop off the
markers automatically if it cannot find a viable path to the corner. 

To run the program, first compile the code:
gcc main.c arena.c robot.c graphics.c dfs.c bfs.c -o robotArena -lm

Then run the program:
./robotArena x y DIR rows cols mapmode markers | java -jar drawapp-4.5.jar

- x, y is the starting position of the robot (the program will find the nearest empty cell if
there is an obstacle in the way)
- DIR is the starting direction of the robot (UP, DOWN, LEFT, RIGHT)
- rows, cols are the dimensions of the grid (Max 20)
- mapmode is the configuration of the obstacles (random, circle)
- markers is the number of markers you wish there to spawn (randomly)

If you leave the CLI arguments blank, the program will resort to default values:
x, y = 0
DIR = UP
rows, cols = 10
mapmode = MAP_RANDOM
markers = 15

An example of this program would be:
./robotArena 5 5 UP 15 15 circle 20 | java -jar drawapp-4.5.jar

This would spawn the robot at position (5, 5), facing up, inside a grid with dimensions
15x15 with the obstacles arranged in a circle, and 20 markers for the robot to collect. 

If the robot spawns inside an obstacle, a flood fill will be carried out in order to find the nearest free cell.

Sources:

Grid drawing + Robot moving logic: UCL Coursework Assignment brief 
DFS: https://www.geeksforgeeks.org/dsa/depth-first-search-or-dfs-for-a-graph
BFS: https://www.geeksforgeeks.org/dsa/breadth-first-search-or-bfs-for-a-graph
Flood Fill: https://usaco.guide/silver/flood-fill?lang=cpp

