/* 放預設函式庫與一些要統一的大小設定，有不滿意請盡量說，要一起修正！ */
#ifndef BASIC_SETTING
#define BASIC_SETTING

// 預設函式庫
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // for bool type
#include "../SDL2/SDL.h" //..會回到上層資料夾，就可以再找SDL2
#include "../SDL2/SDL_image.h"

// private / public 設定，我個人喜歡用，看大家要不要！
#define private static
#define public

// debug用 (第一個是要 print 的東西，第二個是 "%d" 之類的)
#define DEBUG_PRINT(n, conversionSpceifier) fprintf(stderr, #n ": " conversionSpceifier "\n", n);SDL_Delay(5);
#define P printf("*file: %s, func: %s, line: %d\n", __FILE__, __func__, __LINE__)

// 不同顏色粗字體，用於 print 錯誤訊息
#define STRING_LITERAL_IN_PRINTF_RED(n) "\033[1;31m" n "\033[0m" 
#define STRING_LITERAL_IN_PRINTF_YELLOW(n) "\033[0;33m" n "\033[0m" 

// windows大小 + 名字
#define WINDOWS_NAME "2D-Craft"
enum sizeOfWindow {WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720};

// 沒有方塊的編號
#define NO_BLOCK_ID -9 //no block = -9

// 位置資訊，用 x,y 一起比較方便
struct pos
{
    int x;
    int y;
};
typedef struct pos SDL_position;

// 素材長寬資訊，也是一起存方便
struct size
{
    int width;
    int height;
};
typedef struct size SDL_size;

#endif