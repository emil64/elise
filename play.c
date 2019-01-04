#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "logic.h"

const int N = 16;
const int M = 10;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT =768;

typedef struct color color;
typedef struct cell cell;

enum {Up_down, Down_down, Left_down, Right_down, Exit, Up_up, Down_up, Left_up, Right_up, Nothing, Space_down, Space_up, P_down, P_up, NewGame};  // Event types
enum {SL, GO, C};
typedef struct game game;

//initializes level
void initLevel(game *g, int level){
    if(level == 0)
        return;
    char filename[20] = "";
    sprintf(filename, "res/lev%d.tet", level);
    FILE* f = fopen(filename, "r");
    char ch;

    for(int i=0; i<g->n; i++){
        for(int j=0; j<g->m; j++){
            ch = fgetc(f);
            //printf("%c", ch);
            if(ch == '1'){
                g->grid[i][j].state = true;
                g->grid[i][j].c.r = 186;
                g->grid[i][j].c.g = 186;
                g->grid[i][j].c.b = 186;
            }
        }
        ch = fgetc(f);
        //printf("\n");
    }

}

//initialises game
void intitGame(game *g, cell s[1000][4][4]){
    init(N, M, g->grid);
    init(4, 4, g->shape);
    init(4, 4, g->next);
    g->x = (M-4)/2;
    g->y = 0;
    g->n = N;
    g->m = M;
    g->ns = 4;
    g->ms = 4;
    g->level = 1;
    g->score = 0;
    g->time = SDL_GetTicks();
    int r = SDL_GetTicks() % (g->ls);
    copyMatrix(g->ns, g->ms, g->shape, s[r]);
    r = SDL_GetTicks() % (g->ls);
    r = (r+1)%(g->ls);
    copyMatrix(g->ns, g->ms, g->next, s[r]);
}

//draws shape
void drawShape(int x, int y, cell grid[4][4], SDL_Renderer* rend){
    int square = (WINDOW_HEIGHT - 20)/ N;
    int f = (WINDOW_WIDTH - M*square)/4;
    SDL_Rect r;
    for (int i=0; i<4; i++)
        for(int j=0; j<4; j++){
            //SDL_RenderClear(rend);
            r.x = (j+x)*square + f;
            r.y = (i+y)*square + 10;
            r.w = square;
            r.h = square;
            if(grid[i][j].state ){
                SDL_SetRenderDrawColor( rend, grid[i][j].c.r, grid[i][j].c.g, grid[i][j].c.b, 255 );
                SDL_RenderFillRect( rend, &r);
            }
        }
}

//draw next shape
void DrawNext(game* g, SDL_Renderer *rend){

    int square = (WINDOW_HEIGHT - 20)/ (N*2);
    int f = (WINDOW_WIDTH - M*square/2);
    f = f/2 + f/3;
    SDL_Rect r;
    for (int i=0; i<4; i++)
        for(int j=0; j<4; j++){
            r.x = j*square + f;
            r.y = i*square + 50;
            r.w = square;
            r.h = square;
            if(g->next[i][j].state == false)
                SDL_SetRenderDrawColor( rend, 255, 255, 255, 0 );
            else
                SDL_SetRenderDrawColor( rend, g->next[i][j].c.r, g->next[i][j].c.g, g->next[i][j].c.b, 255 );
            SDL_RenderFillRect( rend, &r);
        }
}

//draws simple box
void drawBox(SDL_Renderer *rend, int x, int y, int w, int h, int r, int g, int b){

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_SetRenderDrawColor(rend, r, g, b, 255);
    SDL_RenderFillRect(rend, &rect);
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
}

//draw the game
void draw(game* g, SDL_Renderer* rend){
    int square = (WINDOW_HEIGHT - 20)/ N;
    int f = (WINDOW_WIDTH - M*square)/4;
    SDL_Rect r;
    for (int i=0; i<N; i++)
        for(int j=0; j<M; j++){
            //SDL_RenderClear(rend);
            r.x = j*square + f;
            r.y = i*square + 10;
            r.w = square;
            r.h = square;
            if(g->grid[i][j].state == false)
                SDL_SetRenderDrawColor( rend, 0, 0, 0, 255 );
            else
                SDL_SetRenderDrawColor( rend, g->grid[i][j].c.r, g->grid[i][j].c.g, g->grid[i][j].c.b, 255 );
            SDL_RenderFillRect( rend, &r );
        }
    drawShape(g->x, g->y, g->shape, rend);
    DrawNext(g, rend);
}

//draws text message
void drawText(game *g, SDL_Renderer* rend, TTF_Font* Font, int type){
    SDL_Color Black = {0, 0, 0};
    SDL_Color Red = {255, 0, 0};
    SDL_Color Green = {0, 0, 255};
    int square = (WINDOW_HEIGHT - 20)/ (N);
    int f = (WINDOW_WIDTH - M*square/2);
    f = f/2 + f/4 + f/20;
    if(Font == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }

    char text[1000]="";

    sprintf(text, "Score: %d  Level: %d", g->score, g->level);
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, text, Black);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.x = f;
    Message_rect.y = WINDOW_HEIGHT - 80;
    Message_rect.w = strlen(text)*15;
    Message_rect.h = 48;
    SDL_RenderCopy(rend, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);

    if(type == GO) {
        sprintf(text, "Game Over! :(");
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, text, Red);
        SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
        SDL_Rect Message_rect;
        Message_rect.x = f+22;
        Message_rect.y = WINDOW_HEIGHT - 550;
        Message_rect.w = strlen(text)*22;
        Message_rect.h = 48;
        SDL_RenderCopy(rend, Message, NULL, &Message_rect);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
    }
    else if(type == C){
        sprintf(text, "Congrats! :)");
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, text, Green);
        SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
        SDL_Rect Message_rect;
        Message_rect.x = f+22;
        Message_rect.y = WINDOW_HEIGHT - 550;
        Message_rect.w = strlen(text)*22;
        Message_rect.h = 48;
        SDL_RenderCopy(rend, Message, NULL, &Message_rect);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
    }
}

//draws new game buttib
void drawNewGame(game *g, SDL_Renderer* rend, TTF_Font *Font){

    SDL_Color White = {255, 255, 255};

    SDL_Rect Message_rect;
    Message_rect.x = 700;
    Message_rect.y = 350;
    Message_rect.w = 200;
    Message_rect.h = 70;

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, "New Game", White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
    SDL_RenderCopy(rend, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

//loades the shapes form shapes.tet
int loadShapes(cell shape[1000][4][4]){
    FILE* f = fopen("res/shapes.tet", "r");
    int i, j, n, l1, l2, l3, l4;
    color c;
    fscanf(f, "%d", &n);
    for(i = 0; i<n; i++){
        fscanf(f, "%d %d %d %d %d %d %d", &c.r, &c.g, &c.b, &l1, &l2, &l3, &l4);
        for(j=0; j<4; j++){
            shape[i][0][3-j].state = ((l1 & (1<<j))!=0);
            shape[i][0][j].c = c;
         }
        for(j=0; j<4; j++){
            shape[i][1][3-j].state = ((l2 & (1<<j))!=0);
            shape[i][1][j].c = c;
         }
        for(j=0; j<4; j++){
            shape[i][2][3-j].state = ((l3 & (1<<j))!=0);
            shape[i][2][j].c = c;
         }
        for(j=0; j<4; j++){
            shape[i][3][3-j].state = ((l4 & (1<<j))!=0);
            shape[i][3][j].c = c;
         }
    }
    return n;
}

//reads the event and returns it
int EventReader(){
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT)
       return Exit;
    int mouse_x, mouse_y;
    int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    if(buttons && (mouse_x >= 650 && mouse_x <= 950 && mouse_y >= 300 && mouse_y <=470))
        return NewGame;
    switch (event.type){
    case SDL_KEYDOWN:
           switch (event.key.keysym.scancode)
           {
               case SDL_SCANCODE_W:
               case SDL_SCANCODE_UP:
                   return Up_down;
                   break;
               case SDL_SCANCODE_A:
               case SDL_SCANCODE_LEFT:
                   return Left_down;
                   break;
               case SDL_SCANCODE_S:
               case SDL_SCANCODE_DOWN:
                   return Down_down;
                   break;
               case SDL_SCANCODE_D:
               case SDL_SCANCODE_RIGHT:
                   return Right_down;
                   break;
               case SDL_SCANCODE_SPACE:
                   return Space_down;
                   break;
               case SDL_SCANCODE_P:
                   return P_down;
                   break;
               default: break;
           }
           break;
   case SDL_KEYUP:
           switch (event.key.keysym.scancode)
           {
               case SDL_SCANCODE_W:
               case SDL_SCANCODE_UP:
                   return Up_up;
                   break;
               case SDL_SCANCODE_A:
               case SDL_SCANCODE_LEFT:
                   return Left_up;
                   break;
               case SDL_SCANCODE_S:
               case SDL_SCANCODE_DOWN:
                   return Down_up;
                   break;
               case SDL_SCANCODE_D:
               case SDL_SCANCODE_RIGHT:
                   return Right_up;
                   break;
               case SDL_SCANCODE_SPACE:
                   return Space_up;
                   break;

               case SDL_SCANCODE_P:
                   return P_up;
                   break;
               default: break;
           }
           break;
       }
     return Nothing;
}

//land the shaoe
void land(game *g, cell s[1000][4][4]){
    append(g);
    int l = check(g);
    int lines = 0;
    while(l != -1){
        emptyLine(l, g);
        l = check(g);
        lines++;
    }

    switch (lines){
        case 1: g->score += (lines)*40*g->level;break;
        case 2: g->score += (lines)*100*g->level;break;
        case 3: g->score += (lines)*300*g->level;break;
        case 4: g->score += (lines)*1200*g->level;break;
        default: break;
    }

    if((g->score)/g->level > 1000*g->level)
        g->level++;
    copyMatrix(g->ns,  g->ms, g->shape, g->next);
    int r =  SDL_GetTicks() % (g->ls);
    copyMatrix(g->ns, g->ms, g->next, s[r]);
    g->x = (g->m - 4)/2;
    g->y = 0;
}

//fail in case of bad sdl init
void fail(){
    fprintf(stderr, "%s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
}

//asserts sdl things
int I(int n){
    if(n<0) fail();
    return n;
}
//asserts other sdl things
void *P(void *p){
    if (p==NULL) fail();
    return p;
}

//create sdl renderer
SDL_Renderer *display(SDL_Window *win){
    I(SDL_Init(SDL_INIT_EVERYTHING));
    win = SDL_CreateWindow("Tetris",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT, 0);//,SDL_WINDOW_FULLSCREEN_DESKTOP);
    P(win);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);
    P(rend);

    SDL_SetRenderDrawColor( rend, 255, 255, 255, 255 );
    SDL_RenderClear( rend );
    SDL_RenderPresent(rend);

     if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) { printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() ); }

    return rend;
}

//combine the functions above in a playable game
int play(game *g, SDL_Renderer *rend, TTF_Font* Font, int level){

    Mix_Music *music = NULL;
    music = Mix_LoadMUS("res/Korobeiniki.mp3");
    if( music == NULL ) { printf("No music :( \n\n"); }
    Mix_PlayMusic( music, -1 );

    cell shapes[10000][4][4];
    g->ls = loadShapes(shapes);

    intitGame(g, shapes);
    initLevel(g, level);
    bool left=false, right=false, up=false, down=false, space=false, p= false, pause = false;
    bool close_requested = false;
    bool vu=false, vd=false, vl=false, vr=false, vs=false, vp = false;
    bool gameOver = false, completed = false;
    unsigned int currentTime;
    while (!close_requested && !gameOver && !completed){
        int event = EventReader();
        if(event == Exit)
            close_requested = true;
        switch(event){
            case Up_down: up = 1; break;
            case Down_down: down = 1; break;
            case Left_down: left = 1; break;
            case Right_down: right = 1; break;
            case Up_up: up = 0; break;
            case Down_up: down  = 0; break;
            case Left_up: left = 0; break;
            case Right_up: right = 0; break;
            case Space_up: space = 0; break;
            case Space_down: space = 1; break;
            case P_up: p = 0; break;
            case P_down: p = 1; break;
        }

        if(vp == 1 && p == 0){
            pause ^= 1;
        }
        if(!pause){
            if(vl == 1 && left == 0){
                moveLeft(g);
            }
            if(vr == 1 && right == 0){
                moveRight(g);
            }
            if(vu == 1 && up == 0)
                rotateUp(g);

            if(vd == 1 && down == 0)
                if(goDown(g))
                    land(g, shapes);

            if(vs == 1 && space == 0){
                while(!goDown(g));
                land(g, shapes);
                if(level != 0){
                    completed = levelCompleted(g);
                }
            }

            currentTime = SDL_GetTicks();

            if(((double)(currentTime - (g->time))/1000.0) >= (double)(1.0 / ((double)(g->level*0.75)))) {
                if(goDown(g)){
                    land(g, shapes);
                    if(level != 0){
                        completed = levelCompleted(g);
                    }
                    if(colision(g->n, g->m, g->x, g->y, g->shape, g->grid)){
                        gameOver = true;
                        break;
                    }
                }
                g->time = SDL_GetTicks();
            }
            if(completed)
                break;
        }
        vl = left;vr = right;vu = up;vd = down;vs = space;vp = p;

        SDL_RenderClear(rend);
        draw(g, rend);
        drawText(g, rend, Font, SL);
        SDL_RenderPresent(rend);
     }
     while(!close_requested){
         int event = EventReader();
         if(event == Exit)
             close_requested = true;
         if(event == NewGame){
             return -2;
         }
         if(completed){
             SDL_RenderClear(rend);
             drawBox(rend, 650, 300, 300, 170, 0, 0, 255);
             draw(g, rend);
             drawText(g, rend, Font, C);
             drawNewGame(g, rend, Font);
             SDL_RenderPresent(rend);
         }
         else {
             SDL_RenderClear(rend);
             drawBox(rend, 650, 300, 300, 170, 0, 0, 255);
             draw(g, rend);
             drawText(g, rend, Font, GO);
             drawNewGame(g, rend, Font);
             SDL_RenderPresent(rend);
         }
     }
     return 0;
}

//initialises and calls play, after destroys
void start(game*g, SDL_Renderer *rend, int type){

    testLogic();
    SDL_Window *win = NULL;
    TTF_Init();
    TTF_Font* Font = TTF_OpenFont("res/OpenSans-Regular.ttf", 256);
    //SDL_Renderer *rend = display(win);
    int action = play(g, rend, Font, type);
    while(action == -2){
        action = play(g, rend, Font, type);
    }

    SDL_RenderClear(rend);
    Mix_Quit();
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    win = NULL;
    rend = NULL;
    SDL_Quit();
 }
