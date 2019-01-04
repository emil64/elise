#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include "logic.h"

void start(struct game *g, SDL_Renderer *rend, int type);

void fail();
int I(int n);
void *P(void *p);
SDL_Renderer *display(SDL_Window *win);
void drawBox(SDL_Renderer *rend, int x, int y, int w, int h, int r, int g, int b);
