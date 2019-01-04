#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "logic.h"

typedef struct color color;
typedef struct cell cell;
typedef struct game game;

//initializes the matrix with empty cells
void init(const int n, const int m, cell p[n][m]){
    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            p[i][j] = empty;
}

//checks if the line is complete
int check(game *g){

    int i, j, s;
    for(i=0; i<g->n; i++){
        s = 0;
        for(j=0; j<g->m; j++)
            if(g->grid[i][j].state)
                s++;
        if(s == (g->m))
            return i;
    }
    return -1;
}

//emptyes the complete lines
void emptyLine(int line, game *g){
    int i, j;
    for(i=line; i>0; i--)
        for(j=0; j<g->m; j++)
            g->grid[i][j] = g->grid[i-1][j];
    for(j=0; j<g->m; j++)
        g->grid[0][j] = empty;
}

//coppies for matrix s (source) to matrix d (destination)
void copyMatrix(int n, int m, cell d[n][m], cell s[n][m]){
    int i, j;
    for(i=0; i<n; i++)
        for(j=0; j<m; j++)
            d[i][j] = s[i][j];
}

//returns the shape rotated clockwise
void rotateRight(cell m[4][4]){
    cell r[4][4] = {{empty}};
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            r[j][3-i] = m[i][j];
    copyMatrix(4, 4, m, r);
}

//returns the shape rotated counterclockwise
void rotateLeft(cell m[4][4]){
    cell r[4][4]={{empty}};
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            r[3-j][i] = m[i][j];
    copyMatrix(4, 4, m, r);

}

//verifies if the shape is colifing (will colide) with the grid
bool colision(int N, int M, int x, int y, cell m[4][4], cell s[N][M]){
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            if(m[i][j].state){
                if((y+i < 0) || (y+i >= N)) return true;
                if((x+j < 0) || (x+j >= M)) return true;
                if(s[i+y][j+x].state)
                    return true;
            }
    return false;
}

//moves the shape left
void moveLeft(game *g){

    if(!(colision(g->n, g->m, (g->x)-1, g->y, g->shape, g->grid))){
        g->x--;
    }
}

//moves the shape right
void moveRight(game *g){

    if(!(colision(g->n, g->m, (g->x)+1, g->y, g->shape, g->grid))){
        g->x++;
    }
}

//rotates the shape cloclwise (if possible)
void rotateUp(game* g){
    cell newShape[g->ns][g->ms];
    copyMatrix(g->ns, g->ms, newShape, g->shape);
    rotateLeft(newShape);
    if(!colision(g->n, g->m, g->x, g->y, newShape, g->grid))
        copyMatrix(g->ns, g->ms, g->shape, newShape);
}

//rotates the shape counterclockwise (if possible)
void rotateDown(game *g){
    cell newShape[g->ns][g->ms];
    copyMatrix(g->ns, g->ms, newShape, g->shape);
    rotateRight(newShape);
    if(!colision(g->n, g->m, g->x, g->y, newShape, g->grid))
        copyMatrix(g->ns, g->ms, g->shape, newShape);

}

//append the landed shape to the grid
void append(game *g){

    for(int i=0; i<g->ns; i++){
        for(int j=0; j<g->ms; j++){
            if(g->shape[i][j].state)
                g->grid[i+g->y][j+g->x] = g->shape[i][j];
        }
    }
}

//move down
bool goDown(game *g){
    if(!(colision(g->n, g->m, g->x, (g->y)+1, g->shape, g->grid))){
        g->y++;
        return false;
    }
    else
        return true;
}

//checks if the level is completed or not(career mode)
bool levelCompleted(game *g){
    for(int i=0; i<g->n; i++)
        for(int j=0; j<g->m; j++)
            if(g->grid[i][j].state == true)
                if(g->grid[i][j].c.r == 186 && g->grid[i][j].c.g == 186 && g->grid[i][j].c.b == 186)
                    return false;
    return true;
}

void assert(int line, bool b) {
    if (b) return;
    printf("The test on line %d fails.\n", line);
    exit(1);
}


//asserts the functions
void testLogic(){
    game g;
    init(16, 10, g.grid);
    init(4, 4, g.shape);
    g.n = 16;
    g.m =10;
    g.ns = 4;
    g.ms = 4;
    g.x = 0;
    g.y = 0;
    assert(__LINE__, levelCompleted(&g) == true);
    goDown(&g);
    assert(__LINE__, g.y == 1);
    g.shape[0][0].state = true;
    append(&g);
    assert(__LINE__, g.grid[1][0].state == true);
    rotateDown(&g);
    assert(__LINE__, g.shape[0][0].state == false);
    moveLeft(&g);
    assert(__LINE__, g.grid[6][0].state = true);
    assert(__LINE__, colision(g.n, g.m, -5, -5, g.shape, g.grid));
    emptyLine(6, &g);
    assert(__LINE__, g.grid[6][0].state == false);
}
