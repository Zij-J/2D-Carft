/* map大陣列(需存地圖上絕對位置)、地圖file、地圖cursor 與 相關 function (file I/O：map file)*/
#include "../include/basicSetting.h" // 要用的
#include "../include/ui.h" // 要用的
#include "../include/render.h" // 要用的
#include "../include/map.h" // 要放的

// 方塊邊長
#define TOTAL_BLOCK_NUMBER_IN_WIDTH 12
#define BLOCK_WIDTH (WINDOW_HEIGHT / TOTAL_BLOCK_NUMBER_IN_WIDTH) // 用寬能放幾個推算方塊的邊長

// 地圖初始化
public void Map_Init()
{
    // 要 file input 地圖 file
}

// 清除地圖
public void Map_Clear()
{
    // 要 file output!
}

// 取得要顯示的畫面部分地圖
public short **Map_GetTotalgMap()
{
    // 這樣回傳就可以傳遞二維陣列 + 不用複製整個 Array
    // short array[2][2];
    // return &array;
}

// 依輸入放置、刪除方塊
public void Map_EditBlock(SDL_Event event)
{
    // 要取得目前快捷欄選取的方塊編號
    // short NowChoseBlockID = HotBar_GetChosenBlockID();
}

// 依新的 camera 位置更新(增加、刪除)地圖
public void Map_UpdateMaps()
{
    // 取得 camera 座標
    // SDL_position cameraPos = Render_GetCameraPosition();

    // if 在大陣列外，file output 舊地圖，file input 新地圖 (如果有新地圖，需生成)
}