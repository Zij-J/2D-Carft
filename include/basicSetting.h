/* 放預設函式庫與一些要統一的大小設定，有不滿意請盡量說，要一起修正！ */

// 預設函式庫
#include <stdio.h>
#include <stdlib.h> // for malloc
#include <string.h> // for strdup，自己寫不知道放哪裡，感覺會使程式更混亂
#include "../SDL2/SDL.h" //..會回到上層資料夾，就可以再找SDL2
#include "../SDL2/SDL_image.h"


// windows大小 + 名字
#define WINDOWS_NAME "2D-Craft"
enum sizeOfWindow {WINDOW_WIDTH = 720, WINDOW_HEIGHT = 480};


// 方塊邊長
#define TOTAL_BLOCK_NUMBER_IN_WIDTH 16
#define BLOCK_WIDTH (WINDOW_WIDTH / TOTAL_BLOCK_NUMBER_IN_WIDTH) // 用寬能放幾個推算方塊的邊長 (from windows大小)

// 初始 Array大小 for 可變動array (三個檔案共用)
#define INIT_ARRAY_SIZE 2


// 設定是 public 還是 private 的要用 (變成似C#)！

#define public
#define private static
 