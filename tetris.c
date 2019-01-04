#include <stdio.h>
#include "play.h"

typedef struct game game;

enum{Exit, Sandbox, Career};//Game modes

//draw button text
void drawOptions(SDL_Renderer *rend, TTF_Font *Font, int x, int y, int w, int h, char text[]){
    SDL_Color White = {255, 255, 255};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, text, White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = w;
    Message_rect.h = h;
    SDL_RenderCopy(rend, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

//draw title
void drawTitle(SDL_Renderer *rend, TTF_Font *Font, int x, int y, int w, int h, char text[]){
    SDL_Color White = {0, 0, 0};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, text, White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = w;
    Message_rect.h = h;
    SDL_RenderCopy(rend, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

//draw tetris logo
void drawLogo(SDL_Renderer *rend, SDL_Texture *img){
    int w, h;
	SDL_QueryTexture(img, NULL, NULL, &w, &h);
    SDL_Rect texr; texr.x = 0; texr.y = 150; texr.w = w; texr.h = h;
    SDL_RenderCopy(rend, img, NULL, &texr);
}

//draw main menu
void drawMenu(SDL_Renderer *rend, TTF_Font *Font, SDL_Texture *img){
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if(mouse_x >= 650 && mouse_x <=950 && mouse_y >= 120 && mouse_y <=320)
        drawBox(rend, 650, 120, 300, 200, 255, 100, 0);
    else drawBox(rend, 650, 120, 300, 200, 255, 0, 0);
    if(mouse_x >= 650 && mouse_x <=950 && mouse_y >= 420 && mouse_y <=620)
        drawBox(rend, 650, 420, 300, 200, 255, 100, 0);
    else
        drawBox(rend, 650, 420, 300, 200, 255, 0, 0);
    drawOptions(rend, Font, 680, 170, 240, 100, "Sandbox");
    drawOptions(rend, Font, 700, 470, 200, 100, "Career");
    drawLogo(rend, img);
}

//draw level selector menu
void drawLevelSelector(SDL_Renderer *rend, TTF_Font *Font, SDL_Texture *img){

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    drawTitle(rend, Font, 500, 40, 200, 40, "Select level: ");
    if(mouse_x >=600 && mouse_x <=720 && mouse_y >= 140 && mouse_y <=280)
        drawBox(rend, 600, 140, 120, 140, 255, 100, 0);
    else drawBox(rend, 600, 140, 120, 140, 255, 0, 0);
    if(mouse_x >=800 && mouse_x <=1020 && mouse_y >= 140 && mouse_y <=280)
        drawBox(rend, 800, 140, 120, 140, 255, 100, 0);
    else drawBox(rend, 800, 140, 120, 140, 255, 0, 0);
    if(mouse_x >=600 && mouse_x <=720 && mouse_y >= 330 && mouse_y <=470)
        drawBox(rend, 600, 330, 120, 140, 255, 100, 0);
    else drawBox(rend, 600, 330, 120, 140, 255, 0, 0);
    if(mouse_x >=800 && mouse_x <=1020 && mouse_y >= 330 && mouse_y <=470)
        drawBox(rend, 800, 330, 120, 140, 255, 100, 0);
    else drawBox(rend, 800, 330, 120, 140, 255, 0, 0);
    if(mouse_x >=600 && mouse_x <=720 && mouse_y >= 520 && mouse_y <=660)
        drawBox(rend, 600, 520, 120, 140, 255, 100, 0);
    else drawBox(rend, 600, 520, 120, 140, 255, 0, 0);
    if(mouse_x >=800 && mouse_x <=1020 && mouse_y >= 520 && mouse_y <=660)
        drawBox(rend, 800, 520, 120, 140, 255, 100, 0);
    else drawBox(rend, 800, 520, 120, 140, 255, 0, 0);

    drawOptions(rend, Font, 620, 160, 80, 100, "1");
    drawOptions(rend, Font, 820, 160, 80, 100, "2");
    drawOptions(rend, Font, 620, 350, 80, 100, "3");
    drawOptions(rend, Font, 820, 350, 80, 100, "4");
    drawOptions(rend, Font, 620, 540, 80, 100, "5");
    drawOptions(rend, Font, 820, 540, 80, 100, "6");
    drawLogo(rend, img);
}

//event reader for the welcome page
int MouseReader1(){
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT)
       return Exit;
    int mouse_x, mouse_y;
    int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    if(buttons){
        if(mouse_x >= 650 && mouse_x <=950 && mouse_y >= 120 && mouse_y <=320)
            return Sandbox;
        if(mouse_x >= 650 && mouse_x <=950 && mouse_y >= 420 && mouse_y <=620)
            return Career;
    }
    return -1;
}

//event reader for the level selection page
int MouseReader2(){
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT)
       return Exit;
    int mouse_x, mouse_y;
    int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    if(buttons){
        if(mouse_x >=600 && mouse_x <=720 && mouse_y >= 140 && mouse_y <=280)
            return 1;
        if(mouse_x >=800 && mouse_x <=1020 && mouse_y >= 140 && mouse_y <=280)
            return 2;
        if(mouse_x >=600 && mouse_x <=720 && mouse_y >= 330 && mouse_y <=470)
            return 3;
        if(mouse_x >=800 && mouse_x <=1020 && mouse_y >= 330 && mouse_y <=470)
            return 4;
        if(mouse_x >=600 && mouse_x <=720 && mouse_y >= 520 && mouse_y <=660)
            return 5;
        if(mouse_x >=800 && mouse_x <=1020 && mouse_y >= 520 && mouse_y <=660)
            return 6;
    }
    return -1;
}


int main(){

    SDL_Window *win = NULL;
    TTF_Init();
    system("screen -dm -S ./elise");
    TTF_Font* Font = TTF_OpenFont("res/OpenSans-Regular.ttf", 256);
    SDL_Renderer *rend = display(win);
    int event = -1;
    SDL_Texture *img;
    img = IMG_LoadTexture(rend, "res/logo.png");
    while(event == -1){
        SDL_RenderClear(rend);
        drawMenu(rend, Font, img);
        event = MouseReader1();
        SDL_RenderPresent(rend);
        SDL_Delay(1000/60);
    }
    if(event == Sandbox){
        game g;
        start(&g, rend, 0);
    }
    if(event == Career){
        event = -1;
        SDL_Delay(50);
        while(event == -1){
            SDL_RenderClear(rend);
            drawLevelSelector(rend, Font, img);
            event = MouseReader2();
            SDL_RenderPresent(rend);
        }
        if(event != Exit){
            game g;
            start(&g, rend, event);
        }
    }
    SDL_DestroyTexture(img);
    Mix_Quit();
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    img = NULL;
    win = NULL;
    rend = NULL;
    SDL_Quit();
    return 0;
}
