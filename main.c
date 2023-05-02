#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./SDL2/SDL.h"
#include "./SDL2/SDL_image.h"
#undef main
#define SCR_N 8

void set_image(SDL_Rect *r,int x1,int y1,int w1,int h1){
    r->x=x1;    // x-coordinate of the destination rectangle
    r->y=y1;    // y-coordinate of the destination rectangle
    r->w=w1;    // width of the destination rectangle
    r->h=h1;    // height of the destination rectangle
}

void delay(float secs) {
    clock_t start_time = clock();
    while (clock() <start_time+secs*1000);
}

void update_screen(SDL_Renderer* renderer, SDL_Rect destRect, int n_img, SDL_Texture *imageTexture[], SDL_Texture *frameTexture, int frame[], int map[SCR_N][SCR_N]){
    // Clear renderer
    SDL_RenderClear(renderer);
    for(int i=0; i<SCR_N; i++){
        for(int j=0; j<SCR_N; j++){
            // Set the destination rectangle
            set_image(&destRect,80*j,80*i,80,80);
            // Copy texture to renderer
            SDL_RenderCopy(renderer, imageTexture[map[i][j]], NULL, &destRect);
        }
    }
    set_image(&destRect,80*frame[0],80*frame[1],80,80);
    SDL_RenderCopy(renderer, frameTexture, NULL, &destRect);
    // Update renderer
    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv) {
    //setting
    srand( time(NULL) );
    int map[SCR_N][SCR_N],frame[2]={SCR_N/2,SCR_N/2};//screen  //frame x, y
    for(int i=0; i<SCR_N; i++){
        for(int j=0; j<SCR_N; j++) map[i][j]=rand()%8;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        fprintf(stderr, "Failed to initialize SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL Image Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 640, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load image as texture
    char *img[]={"./image_sourse/tex1.png", "./image_sourse/tex2.png", "./image_sourse/tex3.png",
                 "./image_sourse/tex4.png", "./image_sourse/tex5.png", "./image_sourse/tex6.png",
                 "./image_sourse/tex7.png", "./image_sourse/tex8.png"};//image sourse
    int n_img=sizeof(img)/sizeof(char*);//number of img
    SDL_Texture *imageTexture[n_img], *frameTexture;
    frameTexture = IMG_LoadTexture(renderer, "./image_sourse/frame.png");//frame
    for(int i=0; i<n_img; i++){
        imageTexture[i] = IMG_LoadTexture(renderer, img[i]);
        if (!imageTexture[i]) {
            fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            return 1;
        }
    }

    //initialize

    //SDL_RenderClear(renderer);
    SDL_Rect destRect;
    update_screen(renderer, destRect, n_img, imageTexture, frameTexture, frame, map);//update screen

    //key in or moving mouse

    SDL_Event event;
    while(SDL_WaitEvent(&event)){
        if (event.type == SDL_QUIT) {//quit
            break;
        }
        else if(event.type == SDL_MOUSEMOTION){//move mouse
            int x_mouse, y_mouse;
            SDL_GetMouseState(&x_mouse,&y_mouse);
            frame[0]=x_mouse/80, frame[1]=y_mouse/80;
            update_screen(renderer, destRect, n_img, imageTexture, frameTexture, frame, map);//update screen
        }
        else if(event.type == SDL_KEYDOWN){//press key
            if(event.key.keysym.sym == SDLK_w && frame[1]>0) frame[1]--;
            else if(event.key.keysym.sym == SDLK_a && frame[0]>0) frame[0]--;
            else if(event.key.keysym.sym == SDLK_s && frame[1]<SCR_N-1) frame[1]++;
            else if(event.key.keysym.sym == SDLK_d && frame[0]<SCR_N-1) frame[0]++;
            //printf("*code: %d*\n",event.key.keysym.sym);  //check press key's code
            update_screen(renderer, destRect, n_img, imageTexture, frameTexture, frame, map);//update screen
        }
    }
    // Destroy texture, renderer, and window
    for(int i=0; i<n_img; i++) SDL_DestroyTexture(imageTexture[i]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // Quit SDL_image and SDL
    IMG_Quit();
    SDL_Quit();

    return 0;
}
