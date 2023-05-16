/* 相機、與 renderer 相關 function */
#include "../include/basicSetting.h" // 要用的
#include "../include/texture.h" // 要用的
#include "../include/map.h" // 要用的
#include "../include/ui.h" // 要用的
#include "../include/render.h" // 要放的

SDL_position camera = (SDL_position){.x = 0, .y = 0}; // 相機位置

// 畫背包上的方塊到 renderer
private void Render_RenderBlockInBackpack(SDL_position nowPos, int blockID)
{
    // 取得材質 by 編號
    // SDL_Texture *nowTexture = TetxureBase_GetTexture(blockID);
    // SDL_size blockSize = Backpack_GetBlockSize();
}

// 畫背包到 renderer
public void Render_RenderBackpack()
{
    // 取得背包位置、大小
    // SDL_position backpackPos = Backpack_GetPosition();
    // SDL_size backpackSize = Backpack_GetSize();

    // 還要把方塊畫上去
    // int *textureIDs = TextureBase_GetAllBlock();
    // SDL_position startPos = Backpack_GetBlockStartPosition();

    //Render_RenderBlockInBackpack();
}

// 畫背包的 cursor 到 renderer
public void Render_RenderBackpackCursor()
{
    // 取得 cursor 位置、大小
    // SDL_position cursorPos = Backpack_GetCursorPosition();
    // SDL_size cursorSize = Backpack_GetCursorSize();
}

// 依 camera 差距，畫出地圖
public void Render_RenderMap()
{
    // 取得 Map 大陣列全部、左上角位置、方塊大小
    // int ***Map = Map_GetTotalMap();
    // SDL_position upLeftCornerPos = Map_GetUpLeftCornerPosition();
    // SDL_size blockSize = Map_GetBlockSize();

    // 要算出 camera 相對位置

    // get 目前 block 的材質
    // SDL_Texture *blockTexture = TetxureBase_GetTexture();
}

// 依 camera 差距，畫出背包的 cursor
public void Render_RenderMapCursor()
{
    // 取得 cursor 位置、大小
    // SDL_position cursorPos = Map_GetCursorPosition();
    // SDL_size cursorize = Map_GetCursorSize();

    // 要算出 camera 相對位置
}

// 畫出快捷欄
public void Render_RenderHotbar()
{
    // 取得快捷欄位置、大小
    // SDL_position hotbarPos = Map_GetPosition();
    // SDL_size hotbarize = Map_GetSize();
}

// 畫出快捷欄的 cursor
public void Render_RenderHotbarCursor()
{
    // 取得 cursor 位置、大小
    // SDL_position cursorPos = Map_GetCursorPosition();
    // SDL_size cursorize = Map_GetCursorSize();
}