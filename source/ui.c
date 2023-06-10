/* 快捷欄資料庫、UI圖片顯示(含文字array)、背包cursor 與相關操作 */
#include "../include/basicSetting.h" // 要用的
#include "../include/texture.h" // 要用的
#include "../include/ui.h" // 要放的

// 所有 ui 的位置、大小、顏色資訊都在此
#define BACKPACK_POSITION (SDL_position){.x = 100, .y = 100} // 背包的預設位置
#define BACKPACK_SIZE (SDL_size){.height = 100, .width = 100} // 背包的預設大小
#define SEARCH_WORD_POSITION (SDL_position){.x = 100, .y = 100} // 搜尋文字的預設位置
#define SEARCH_WORD_SIZE (SDL_size){.height = 100, .width = 100} // 搜尋文字。一個字的預設大小(方形)
#define SEARCH_WORD_COLOR (SDL_Color){.r = 255, .g = 255, .b = 255, .a = 255} // 搜尋文字的預設顏色
#define SEARCH_NOTIFY_POSITION (SDL_position){.x = 100, .y = 100} // 提醒文字的預設位置
#define SEARCH_NOTIFY_SIZE (SDL_size){.height = 25, .width = 25} // 提醒文字。一個字的預設大小(方形)
#define SEARCH_NOTIFY_SUCCESS_COLOR (SDL_Color){.r = 51, .g = 255, .b = 51, .a = 255} // 提醒文字(成功)的預設顏色
#define SEARCH_NOTIFY_FAILURE_COLOR (SDL_Color){.r = 255, .g = 51, .b = 51, .a = 255} // 提醒文字(失敗)的預設顏色
#define SEARCH_NOTIFY_SUCCESS_WORD "Tha Block is here!" // 提醒文字(成功)的內容
#define SEARCH_NOTIFY_FAILURE_WORD "no Block named as this!" // 提醒文字(失敗)的內容
#define PAUSE_WORD_SIZE (SDL_size){.height = 100, .width = 100} // 暫停文字的大小 (位置是置中，所以不給調)
#define PAUSE_WORD_COLOR (SDL_Color){.r = 175, .g = 175, .b = 175, .a = 255} // 暫停文字的預設顏色
#define PAUSE_WORD "PAUSED" // 暫停文字的內容
#define PAUSE_BACKGROUND_COLOR (SDL_Color){.r = 0, .g = 0, .b = 0, .a = 100} // 暫停時背景顏色

private bool backpack_isOn = false; // 背包是否開啟

// 初始化快捷欄
public void HotBar_Init()
{
    // 需匯入材質(背包)資料庫的編號
    // int *textureIDs = TextureBase_GetAllID();

}

// 依輸入開關背包
public void Backpack_Switch(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e)
    {
        if(backpack_isOn == false)
            backpack_isOn = true;
        else
            backpack_isOn = false;
    }
}

// get 背包是否開啟
public bool Backpack_isOpen()
{
    return backpack_isOn;
}

// 依輸入移動背包的 cursor
public void Backpack_MoveCursor(SDL_Event event)
{
    // 如果移動到search文字部分，需要改變cursor大小，需在此改變
}

// 取得現在 cursor 在方塊區、還是在搜尋區
public cursorArea Backpack_GetCursorOnArea()
{
    // 要回傳以下三種訊息
    // return blockArea;
    // return searchArea;
    // return nothingArea;
}

// 把背包選到的方塊更新到選到的 Hotbar 上
void Backpack_UpdateBlockToHotbar()
{

}

// 把 cursor 移動到 找到的方塊
void Backpack_FindBlockAndMoveCursor()
{
    // 取得找到的編號
    // short FoundBlockID = TextureBase_GetSearchedBlockID();

    // 把編號變成位置，把cursor移到那邊
}

// 回傳 SearchWord 位置
SDL_position Backpack_GetSearchWordPosition()
{
    return SEARCH_WORD_POSITION;
}
// 回傳 SearchWord 大小
SDL_size Backpack_GetSearchWordSize()
{
    return SEARCH_WORD_SIZE;
}
// 回傳 SearchWord 顏色
SDL_Color Backpack_GetSearchWordColor()
{
    return SEARCH_WORD_COLOR;
}

// 回傳 提醒文字 位置
SDL_position Backpack_GetSearchNotifyPosition()
{
    return SEARCH_NOTIFY_POSITION;
}
// 回傳 提醒文字 大小
SDL_size Backpack_GetSearchNotifySize()
{
    return SEARCH_NOTIFY_SIZE;
}
// 回傳 提醒文字 顏色 (需用 array 存著回傳值，0是成功，1是失敗)
void Backpack_GetSearchNotifyColor(SDL_Color *array)
{
    array[searchNotifyIndex_success] = SEARCH_NOTIFY_SUCCESS_COLOR;
    array[searchNotifyIndex_failure] = SEARCH_NOTIFY_FAILURE_COLOR;
}
// 回傳 提醒文字 內容  (需用 array 存著回傳值，0是成功，1是失敗)
void Backpack_GetSearchNotifyContent(char **array)
{
    array[searchNotifyIndex_success] = SEARCH_NOTIFY_SUCCESS_WORD;
    array[searchNotifyIndex_failure] = SEARCH_NOTIFY_FAILURE_WORD;
}


// 回傳 Pause 文字大小
SDL_size PauseScreen_GetPauseWordSize()
{
    return PAUSE_WORD_SIZE;
}
// 回傳 Pause 文字顏色
SDL_Color PauseScreen_GetPauseWordColor()
{
    return PAUSE_WORD_COLOR;
}
// 回傳 Pause 文字本身
char *PauseScreen_GetPauseWordContent()
{
    return PAUSE_WORD;
}
// 回傳 Pause 背景顏色
SDL_Color PauseScreen_GetPauseBackgroundColor()
{
    return PAUSE_BACKGROUND_COLOR;
}