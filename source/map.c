/* map大陣列(需存地圖上絕對位置)、地圖file、地圖cursor 與 相關 function (file I/O：map file)*/
#include "../include/basicSetting.h" // 要用的
#include "../include/map.h" // 要放的

// 方塊邊長
#define TOTAL_BLOCK_NUMBER_IN_WIDTH 12
#define BLOCK_WIDTH (WINDOW_HEIGHT / TOTAL_BLOCK_NUMBER_IN_WIDTH) // 用寬能放幾個推算方塊的邊長

// 地圖初始化
public void Maps_Init()
{
    // 要 file input 地圖 file
}

// 清除地圖
public void Maps_Clear()
{
    // 要 file output!
}

// 取得要顯示的畫面部分地圖
int ***Map_GetTotalgMap()
{
    // 這樣回傳就可以傳遞二維陣列 + 不用複製整個 Array
    // int array[2][2];
    // return &array;
}