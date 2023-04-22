// 需要的函式庫
#include "./include/basicSetting.h" //請把header都寫在 include 資料夾中！
#include "./include/SDL_StartAndEnd.h"
#undef main

// main函式
int main(int argc, char** argv) 
{
    // 創立所有 SDL 需要的東西
    SDL_Window* window; 
    SDL_Renderer* renderer;
    SDL_InitializeAll(&window, &renderer);

    // Load image as texture
    SDL_Texture* imageTexture = IMG_LoadTexture(renderer, "block_pictures/Grass_Block.png");
    if (!imageTexture) {
        fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
        SDL_EndAll_StopProgram(&window, &renderer);
    }

    // Clear renderer
    SDL_RenderClear(renderer);

    // Set the destination rectangle
    SDL_Rect destRect;
    destRect.x = 500;  // x-coordinate of the destination rectangle
    destRect.y = 100;  // y-coordinate of the destination rectangle
    destRect.w = BLOCK_WIDTH;  // width of the destination rectangle
    destRect.h = BLOCK_WIDTH;  // height of the destination rectangle

    // Copy texture to renderer
    SDL_RenderCopy(renderer, imageTexture, NULL, &destRect);

    // Update renderer
    SDL_RenderPresent(renderer);

    // Wait for user to quit
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            break;
        }
    }

    // Destroy texture
    SDL_DestroyTexture(imageTexture);

    // Close Program
    SDL_EndAll_StopProgram(&window, &renderer);
}
