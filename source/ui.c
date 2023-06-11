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

//linked list node
struct node_Hotbar
{
    short BlockID;
    int num_of_Hotbar; //Hotbar裡的第幾格
    struct node_Hotbar *next;
}*root, *now;
typedef struct node_Hotbar node_Hotbar;

// 初始化快捷欄
public void HotBar_Init()
{
    // 需匯入材質(背包)資料庫的編號?? Ans: 設定遊戲一開始預設在快捷欄的方塊用，所以如果預設是空白就不用
    // int *textureIDs = TextureBase_GetAllID();

    root = malloc(sizeof(node_Hotbar));
    root->num_of_Hotbar=1;
    //設*next (?) Ans: 對！
    //for
        //malloc新的node + 連到上個node的*next + 設定num_of_Hotbar
    //最後一個node的*next要接到root
}

void HotBar_Clear()
{
    //? Ans: free linked list用
}

void HotBar_MoveCursor(SDL_Event event)
{
    //判斷event (if 鍵盤輸入(或滑鼠滾輪 看你))
        //修改*now
}

void HotBar_GetAllID()
{
    //for//取node的ID ??? Ans: 把所有 node 存的 BlockID 回傳，用於一次顯示所有 Hotbar 有的方塊

}

short HotBar_GetChosenBlockID()
{
    //return now->BlockID;
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

// 把背包選到的方塊更新到選到的 Hotbar 上
void Backpack_UpdateBlockToHotbar(SDL_Event event)
{
    //因為看起來你要用拖曳的，所以：
    //if 鼠標在方塊位置&選取
        //移動方塊(新增變數+隨時調整方塊座標)
        //if 在hotbar位置放開
            //修改hotbar對應位置的node->BlockID
}

// 把 cursor 移動到 找到的方塊
void Backpack_FindBlockAndMoveCursor()
{
    // 取得找到的編號
    // short FoundBlockID = TextureBase_GetSearchedBlockID();

    // 把編號變成位置，把cursor移到那邊
}

bool Backpack_isInput(SDL_Event event)
{

}

SDL_position Hotbar_GetPosition()
{
    //get hotbar左上角座標 ? Ans: 對！要知道 Hotbar 要放在整個畫面的哪裡！所以要回傳在畫面上的絕對座標 (一樣是 左上 (0,0) 右x正 下y正)
}

SDL_position Hotbar_GetCursorPosition()
{
    //
}

SDL_position Backpack_GetPosition()
{
    return BACKPACK_POSITION;
}

SDL_position Backpack_GetBlockStartPosition()
{
    //上面define方塊初始位置 + return
}

SDL_position Backpack_GetCursorPosition()
{

}

SDL_size Hotbar_GetSize()
{
    //上面define HOTBAR_SIZE + return
}

SDL_size HotBar_GetCursorSize()
{

}

SDL_size Backpack_GetSize()
{
    //return BACKPACK_SIZE;
}

SDL_size Backpack_GetBlockSize()
{
    //上面define方塊大小 + return
}

SDL_size Backpack_GetCursorSize()
{

}

// 取得現在 cursor 在方塊區、還是在搜尋區
public cursorArea Backpack_GetCursorOnArea()
{
    // 要回傳以下三種訊息 //在.h的enum
    // return blockArea;
    // return searchArea;
    // return nothingArea;
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
<<<<<<< HEAD


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
=======
>>>>>>> hyse-pr
