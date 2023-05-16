/* 快捷欄資料庫、UI圖片顯示(含文字array)、背包cursor 與相關操作 */
#include "../include/basicSetting.h" // 要用的
#include "../include/texture.h" // 要用的
#include "../include/ui.h" // 要放的

#define BACKPACK_POSITION (SDL_position){.x = 100, .y = 100} // 背包的預設位置
#define BACKPACK_SIZE (SDL_size){.height = 100, .width = 100} // 背包的預設大小

private bool backpack_isOn = false; // 背包是否開啟

// 初始化快捷欄
public void HotBar_Init()
{
    // 需匯入材質(背包)資料庫的編號
    // int *textureIDs = TextureBase_GetAllID();

}

// 依輸入開關背包
public void Backpack_Switch(SDL_Event *event)
{
    if((*event).type == SDL_KEYDOWN && (*event).key.keysym.sym == SDLK_e)
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
public void Backpack_MoveCursor()
{
    // 如果移動到search文字部分，需要改變cursor大小，需在此改變
}