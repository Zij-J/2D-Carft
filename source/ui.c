/* 快捷欄資料庫、UI圖片顯示(含文字array)、背包cursor 與相關操作 */
#include "../include/basicSetting.h" // 要用的
#include "../include/texture.h" // 要用的
#include "../include/ui.h" // 要放的

// 所有 ui 的位置、大小、顏色資訊都在此
#define BACKPACK_POSITION (SDL_position){.x = 100, .y = 100} // 背包的預設位置
#define BACKPACK_SIZE (SDL_size){.height = 100, .width = 100} // 背包的預設大小
#define HOTBAR_POSITION (SDL_position){.x = 160, .y = 425} // 快捷欄的預設位置
#define HOTBAR_CELL_SIZE (SDL_size){.height = 40, .width = 40} // 一格快捷欄的預設大小
#define HOTBAR_MARGIN_SIZE (SDL_size){.height = 9, .width = 9} // 快捷欄格子外面的邊界大小
#define HOTBAR_CELL_NUM 9 // 快捷鍵有幾格 
#define HOTBAR_BLOCK_IN_CELL_SIZE (SDL_size){.height = 24, .width = 24} // 一格快捷欄裡面的方塊的預設大小 (位置預設置中，不給調)
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
    // 需匯入材質(背包)資料庫的編號??
    // int *textureIDs = TextureBase_GetAllID();

    root=malloc(sizeof(node_Hotbar));
    node_Hotbar *curNode=root;
    for(int i=1; i<HOTBAR_CELL_NUM; i++){
        curNode->num_of_Hotbar=i;
        curNode->BlockID= NO_BLOCK_ID;
        /*暫用*/if(i == 1 || i == 3) curNode->BlockID= 1;
        curNode->next=malloc(sizeof(node_Hotbar));
        curNode=curNode->next;
    }
    curNode->num_of_Hotbar=HOTBAR_CELL_NUM;
    curNode->BlockID=NO_BLOCK_ID;
    curNode->next=root; // 環狀 linked list!

    // 預設 now 是第一個
    now = root;
}

// 清除 hotbar
public void HotBar_Clear()
{
    // free linked list用
    node_Hotbar *curNode = root, *tmpNext;
    for(int i = 1; i <= HOTBAR_CELL_NUM; ++i)
    {
        tmpNext = (*curNode).next;
        free(curNode);
        curNode = tmpNext;
    }
}

// 依輸入移動 HotBar 的 cursor
public void HotBar_MoveCursor(SDL_Event event)
{
    // 判斷event (if 鍵盤輸入(或滑鼠滾輪 看你)) //修改*now
    if(event.type == SDL_MOUSEWHEEL)
    {
        if(event.wheel.y > 0) // scroll up (有往上滾，就上移)
            now = (*now).next;
        else if(event.wheel.y < 0) // scroll down (有往下滾，就下移)
        {
            for(int i = 0; i < HOTBAR_CELL_NUM -1; ++i) // 就往前總數 -1，就是往後1了
                now = (*now).next;
        }
    }
}

// 把所有 node 存的 BlockID 回傳，用於一次顯示所有 Hotbar 有的方塊 (需準備 buffer，arrayBuffer要初始化至NULL，static者佳)
void HotBar_GetAllID(short **bufferArray, int *totalIDNumber)
{
   // 幫分配記憶體
   if(*bufferArray == NULL) 
        *bufferArray = (short *)malloc((sizeof(short) * HOTBAR_CELL_NUM));

    // 傳ID
    node_Hotbar *cur = root;
    for(int i = 0; i < HOTBAR_CELL_NUM; ++i)
    {
        (*bufferArray)[i] = (*cur).BlockID;
        cur = (*cur).next;
    }

    // 傳總數
    *totalIDNumber = HOTBAR_CELL_NUM;
}

// 取得目前選的方塊
short HotBar_GetChosenBlockID()
{
    return now->BlockID;
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

// get hotbar左上角座標
SDL_position Hotbar_GetPosition()
{
    return HOTBAR_POSITION;
}
// get 整個 hotbar 大小
SDL_size Hotbar_GetSize()
{
    SDL_size cellSize = HOTBAR_CELL_SIZE;
    SDL_size marginSize = HOTBAR_MARGIN_SIZE;
    return (SDL_size){.width = cellSize.width * HOTBAR_CELL_NUM + 2* marginSize.width , .height = cellSize.height + 2* marginSize.height};
}
// get hotbarCursor左上角座標
SDL_position Hotbar_GetCursorPosition()
{
    SDL_position hotbarLeftUpPos = HOTBAR_POSITION;
    SDL_size cellSize = HOTBAR_CELL_SIZE;
    SDL_size marginSize = HOTBAR_MARGIN_SIZE;
    int index = (*now).num_of_Hotbar - 1;
    return (SDL_position){.x = hotbarLeftUpPos.x + marginSize.width + (index * cellSize.width), .y = hotbarLeftUpPos.y + marginSize.height};
}
// get cell 大小 (就是 hotbarCursor 大小 與 Cell方塊之間間隔)
SDL_size HotBar_GetCellSize()
{
    return HOTBAR_CELL_SIZE;
}
// get hotbar 內部方塊開始位置 (取置中的左上)
SDL_position HotBar_GetInCellBlockStartPos()
{
    SDL_position hotbarLeftUpPos = HOTBAR_POSITION;
    SDL_size marginSize = HOTBAR_MARGIN_SIZE;
    SDL_size cellSize = HOTBAR_CELL_SIZE;
    SDL_size blockInCellSize = HOTBAR_BLOCK_IN_CELL_SIZE;
    return (SDL_position){.x = hotbarLeftUpPos.x +marginSize.width +cellSize.width/2 - blockInCellSize.width/2
                        , .y = hotbarLeftUpPos.y +marginSize.height +cellSize.height/2 - blockInCellSize.height/2};
}
// get hotbar 內部方塊大小
SDL_size HotBar_GetInCellBlockSize()
{
    return HOTBAR_BLOCK_IN_CELL_SIZE;
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
