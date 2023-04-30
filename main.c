#include <stdio.h>
#include "./SDL2/SDL.h"
#include "./SDL2/SDL_image.h"
#include <conio.h>
#include <time.h>
#undef main

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

int main(int argc, char** argv) {
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
    char *img[]={"./image_sourse/tex1.png", "./image_sourse/tex2.png", "./image_sourse/tex3.png"};//image sourse
    int n_img=sizeof(img)/sizeof(char*);//number of img
    SDL_Texture* imageTexture[n_img];
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

    //SDL_RenderClear(renderer);
    SDL_Rect destRect;
    for(int i=0; i<n_img; i++){
        char a=getch();//when press key in terminal, then keep going
        printf("a='%c'\n",a);
        // Clear renderer
        SDL_RenderClear(renderer);
        // Set the destination rectangle
        set_image(&destRect,80*(i),0,80,80);
        // Copy texture to renderer
        SDL_RenderCopy(renderer, imageTexture[i], NULL, &destRect);
        // Update renderer
        SDL_RenderPresent(renderer);
    }

    // Wait for user to quit
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
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
