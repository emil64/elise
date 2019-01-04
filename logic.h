#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

struct color{
    int r, g, b;
};

struct cell{
    bool state;
    struct color c;
};

struct game{
    int n, m, ns, ms, ls;
    struct cell grid[16][10];
    struct cell shape[4][4];
    struct cell next[4][4];
    int x, y;
    int score;
    int level;
    unsigned int time;
};

void init(const int n, const int m, struct cell p[n][m]);

static const struct cell empty = {false, {0, 0, 0}};

void copyMatrix(int n, int m, struct cell d[n][m], struct cell s[n][m]);
void rotateRight(struct cell m[4][4]);
void rotateLeft(struct cell m[4][4]);
void moveLeft(struct game *g);
void moveRight(struct game *g);
void rotateUp(struct game *g);
void rotateDown(struct game *g);
bool goDown(struct game *g);
void append(struct game *g);
int check(struct game *g);
void emptyLine(int line, struct game *g);
bool colision(int N, int M, int x, int y, struct cell m[4][4], struct cell s[N][M]);
bool levelCompleted(struct game *g);
void testLogic();
