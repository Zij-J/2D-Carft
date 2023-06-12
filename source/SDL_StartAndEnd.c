/* 處理全部開關程式時會遇到的SDL開關 */
#include "../include/basicSetting.h" //要用的
#include "../include/render.h" //要用的
#include "../include/SDL_StartAndEnd.h" //要放的

#define RESTICT_CURSOR_RANGE (SDL_Rect){.x = 0, .y = 1, .w = WINDOW_WIDTH, .h = WINDOW_HEIGHT-1} // 因為預設限制的 range 還是會點到上框 (仍可移動)，所以要再多限制，少一個 pixel

// 偷記 windows、renderer，這樣 StopProgram 就可以直接呼叫，不用 parameter
SDL_Window *rememberedWindow;
SDL_Renderer *rememberedRenderer;
// 暫停整個遊戲的 flag
bool isPause = false;

// 初始化 SDL、SDL_Image、創立windows、renderer
void SDL_InitializeAll(SDL_Window **window, SDL_Renderer **renderer)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE); //告訴作業系統這是失敗的(但結果上都是結束就是了)！
    }

    // Initialize SDL_image in PNG mode
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) 
    {
        fprintf(stderr, "Failed to initialize SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE); 
    }

    // Create windows
    *window = SDL_CreateWindow(WINDOWS_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if ((*window) == NULL) // SDL_CreateWindow 回傳 NULL if 失敗
    {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE); 
    }
    rememberedWindow = *window;

    // Create renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if ((*renderer) == NULL) 
    {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        IMG_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    rememberedRenderer = *renderer;

    // 限制 cursor 位移，避免在遊戲執行時移動視窗，產生的輸入誤判 bug
    SDL_SetWindowMouseGrab(rememberedWindow, SDL_TRUE);
    SDL_Rect rect = RESTICT_CURSOR_RANGE;
    SDL_SetWindowMouseRect(rememberedWindow, &rect);
}

// 暫停程式
void SDL_SwitchPause()
{
    // 變換
    isPause = !isPause;

    if(isPause)
    {
        // 畫暫停畫面，解除 cursor 限制
        Render_RenderPauseStuff();
        SDL_SetWindowMouseGrab(rememberedWindow, SDL_FALSE);
        SDL_SetWindowMouseRect(rememberedWindow, NULL);
    }
    else
    {
        // 再限制 cursor
        SDL_SetWindowMouseGrab(rememberedWindow, SDL_TRUE);
        SDL_Rect rect = RESTICT_CURSOR_RANGE;
        SDL_SetWindowMouseRect(rememberedWindow, &rect);
    }
}

// 取得是否暫停
bool SDL_GetPauseBool()
{
    return isPause;
}

// 關閉 renderer、windows、SDL_Image、SDL後，結束程式
void SDL_EndAll_StopProgram()
{
    // Destroy renderer, and window
    SDL_DestroyRenderer(rememberedRenderer);
    SDL_DestroyWindow(rememberedWindow);

    // Quit SDL_image and SDL
    IMG_Quit();
    SDL_Quit();

    exit(EXIT_SUCCESS);
}