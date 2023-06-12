/* 快捷欄資料庫、UI圖片顯示(含文字array)、背包cursor 與相關操作 */
#include "../include/basicSetting.h" // 要用的
#include "../include/texture.h" // 要用的
#include "../include/render.h" // 要用的
#include "../include/ui.h" // 要放的


// 所有 ui 的位置、大小、顏色資訊都在此
#define BACKPACK_POSITION (SDL_position){.x = 160, .y = 140} // 背包的預設位置
#define BACKPACK_CELL_SIZE (SDL_size){.height = 40, .width = 40} // 一格背包的預設大小
#define BACKPACK_MARGIN_SIZE (SDL_size){.height = 9, .width = 9} // 背包格子外面的邊界大小
#define BACKPACK_WIDTH_CELL_NUM 9 // 背包寬有幾格
#define BACKPACK_HEIGHT_CELL_NUM 3 // 背包高有幾格
#define BACKPACK_BLOCK_IN_CELL_SIZE (SDL_size){.height = 24, .width = 24} // 一格背包裡面的方塊的預設大小 (位置預設置中，不給調)
#define BACKPACK_SEARCH_AREA_SIZE (SDL_size){.height = 40, .width = 40 * BACKPACK_WIDTH_CELL_NUM} // 搜尋區域的大小
#define HOTBAR_POSITION (SDL_position){.x = 160, .y = 425} // 快捷欄的預設位置
#define HOTBAR_CELL_SIZE (SDL_size){.height = 40, .width = 40} // 一格快捷欄的預設大小
#define HOTBAR_MARGIN_SIZE (SDL_size){.height = 9, .width = 9} // 快捷欄格子外面的邊界大小
#define HOTBAR_CELL_NUM 9 // 快捷鍵有幾格 
#define HOTBAR_BLOCK_IN_CELL_SIZE (SDL_size){.height = 24, .width = 24} // 一格快捷欄裡面的方塊的預設大小 (位置預設置中，不給調)
#define SEARCH_WORD_POSITION (SDL_position){.x = Backpack_GetBlockStartPosition().x +10, .y = Backpack_GetBlockStartPosition().y + BACKPACK_CELL_SIZE.height * BACKPACK_HEIGHT_CELL_NUM + BACKPACK_MARGIN_SIZE.height + 8} // 搜尋文字的預設位置
#define SEARCH_WORD_SIZE (SDL_size){.height = 30, .width = 30} // 搜尋文字。一個字的預設大小(方形)
#define SEARCH_WORD_COLOR (SDL_Color){.r = 25, .g = 25, .b = 25, .a = 255} // 搜尋文字的預設顏色
#define SEARCH_NOTIFY_POSITION (SDL_position){.x = 160, .y = 115} // 提醒文字的預設位置
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
private cursorArea backpack_cursorArea = nothingArea; // 紀錄 cursor 在哪區
private SDL_position backpack_cursorPosition = {.x = POS_NOT_EXISTS, .y = POS_NOT_EXISTS};

//linked list node
struct node_Hotbar
{
    short BlockID;
    int num_of_Hotbar; //Hotbar裡的第幾格(1-base的！)
    struct node_Hotbar *next;
}*root, *now;
typedef struct node_Hotbar node_Hotbar;

// 初始化快捷欄
public void HotBar_Init()
{
    // 需匯入材質(背包)資料庫的編號 for 預設 hotbar
    short *textureIDs = NULL; int IDnumbers; // 記得初始化！
    TextureBase_GetAllID(&textureIDs, &IDnumbers);

    root=malloc(sizeof(node_Hotbar));
    node_Hotbar *curNode=root;
    int i;
    for(i = 1; i<HOTBAR_CELL_NUM; i++){
        curNode->num_of_Hotbar=i;
        curNode->BlockID= (i <= IDnumbers)? textureIDs[i-1] : NO_BLOCK_ID; // 換成0-base (資料課沒有了，就是 NO_BLOCK_ID)
        curNode->next=malloc(sizeof(node_Hotbar));
        curNode=curNode->next;
    }
    curNode->num_of_Hotbar=HOTBAR_CELL_NUM;
    curNode->BlockID=(i <= IDnumbers)? textureIDs[i-1] : NO_BLOCK_ID; // 換成0-base;
    curNode->next=root; // 環狀 linked list!
    free(textureIDs); // 這只會用到一次，所以要清 buffer

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
        if(backpack_cursorArea == searchArea) // 在搜尋時不要按e切換！
            return ;

        if(backpack_isOn == false)
            backpack_isOn = true;
        else
        {
            backpack_isOn = false;
            Render_ForceCloseSearchNotify(); // 把提醒文字強制關閉
            if(backpack_cursorArea == searchArea) // 要把 cursor 切回來！
            {
                SDL_FreeCursor(SDL_GetCursor());
                backpack_cursorArea = nothingArea;
            }     
        }
            
    }
}

// get 背包是否開啟
public bool Backpack_isOpen()
{
    return backpack_isOn;
}

// 依輸入(自行get)移動背包的 cursor (更新 cursor 位置)
public void Backpack_MoveCursor()
{
    // 可能因pasue，移到其他地方，解pause，不移動，cursor不會更新，所以需要不斷更新位置
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    // 算方塊區開始位置
    SDL_position blockAeraStartPos = Backpack_GetBlockStartPosition();
    // cell 大小
    SDL_size cellSize = BACKPACK_CELL_SIZE;
    // 算search區開始位置
    SDL_size searchCellSize = BACKPACK_SEARCH_AREA_SIZE;
    SDL_size marginSize = BACKPACK_MARGIN_SIZE;
    SDL_position searchAeraStartPos = {.x = blockAeraStartPos.x, .y = blockAeraStartPos.y + cellSize.height * BACKPACK_HEIGHT_CELL_NUM + marginSize.height};

    // 檢查滑鼠是否在圖塊的範圍內
    if ((mouseX >= blockAeraStartPos.x && mouseX <= blockAeraStartPos.x + cellSize.width * BACKPACK_WIDTH_CELL_NUM) 
        && (mouseY >= blockAeraStartPos.y && mouseY <= blockAeraStartPos.y + cellSize.height * BACKPACK_HEIGHT_CELL_NUM)) 
    {
        backpack_cursorArea = blockArea;
        // 記住 index 就可
        SDL_position relativePosInBlockArea = (SDL_position){.x = mouseX - blockAeraStartPos.x, .y = mouseY - blockAeraStartPos.y};
        int xTimes = relativePosInBlockArea.x / cellSize.width, yTimes = relativePosInBlockArea.y / cellSize.height;
        backpack_cursorPosition.x = xTimes;
        backpack_cursorPosition.y = yTimes;
    }
    // 在搜尋框框內，開啟搜尋mode
    else if ((mouseX >= searchAeraStartPos.x && mouseX <= searchAeraStartPos.x + searchCellSize.width) 
        && (mouseY >= searchAeraStartPos.y && mouseY <= searchAeraStartPos.y + searchCellSize.height))
    {
        if(backpack_cursorArea != searchArea)
        {
            SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM));// 用 cursor 告訴使用者這是搜尋
            backpack_cursorArea = searchArea;
        }
    }
    else
    {
        if(backpack_cursorArea != nothingArea)
        {
            backpack_cursorPosition.x = POS_NOT_EXISTS;
            backpack_cursorPosition.y = POS_NOT_EXISTS;
            if(backpack_cursorArea == searchArea)
                SDL_FreeCursor(SDL_GetCursor()); // 切換回來，要 free  
            backpack_cursorArea = nothingArea;
        }
    }

}

// // 切換是否在搜尋 // 可能因連續呼叫之類的，導致switch兩次，產生bug (不知why反正用此函式舊有bug)，所以不用
// private void Backpack_switchIsSearching()
// {
    
//     backpack_isCursorOnSearchArea = !backpack_isCursorOnSearchArea;
//     if(backpack_isCursorOnSearchArea == true)
//         SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM)); // 用 cursor 告訴使用者這是搜尋
//     else
//         SDL_FreeCursor(SDL_GetCursor()); // 切換回來，要 free
// }



// 把背包選到的方塊更新到選到的 Hotbar 上
public void Backpack_UpdateBlockToHotbar(SDL_Event event)
{
    //因為看起來你要用拖曳的，所以：
    //if 鼠標在方塊位置&選取
        //移動方塊(新增變數+隨時調整方塊座標)
        //if 在hotbar位置放開
            //修改hotbar對應位置的node->BlockID

    // 我先用選取的
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
        static short *totalBlockID; int totalIDnum;
        TextureBase_GetAllID(&totalBlockID, &totalIDnum);

        // 用 cursor 位置取 index, 更新 now，搞定
        int nowBackpackIndex = backpack_cursorPosition.x + backpack_cursorPosition.y * BACKPACK_WIDTH_CELL_NUM;
        (*now).BlockID = totalBlockID[nowBackpackIndex];
    }
}   

// 把 cursor 移動到 找到的方塊
void Backpack_FindBlockAndMoveCursor(SDL_Window *window, SDL_Event event)
{
    // 取得找到的位置
    int FoundBlockIndex = TextureBase_GetSearchedBlockIndex();
    // 移動 cursor (強制移動真的 cursor)
    SDL_position blockAreaStartPos = Backpack_GetBlockStartPosition();
    SDL_size cellSize = Backpack_GetCellSize();
    int blockPos_x = (FoundBlockIndex % BACKPACK_WIDTH_CELL_NUM) * cellSize.width;
    int blockPos_y = (FoundBlockIndex / BACKPACK_WIDTH_CELL_NUM) * cellSize.width;
    SDL_WarpMouseInWindow(window, blockAreaStartPos.x + blockPos_x + cellSize.width/2, blockAreaStartPos.y + blockPos_y + cellSize.height/2);
}

// 有輸入，再執行
public bool Backpack_isInput(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN)
        return true;
    else
        return false;
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


// 取得背包左上角位置
SDL_position Backpack_GetPosition()
{
    return BACKPACK_POSITION;
}
// 取得方塊區開始位置
SDL_position Backpack_GetBlockStartPosition()
{
    SDL_position backpacStartPos = BACKPACK_POSITION;
    SDL_size marginSize = BACKPACK_MARGIN_SIZE;
    return (SDL_position){.x = backpacStartPos.x + marginSize.width, backpacStartPos.y + marginSize.height};
}
// 取得 cursor 左上角位置
SDL_position Backpack_GetCursorPosition()
{
    SDL_size cellSize = BACKPACK_CELL_SIZE;
    SDL_position blockAreaStartPos = Backpack_GetBlockStartPosition();
    SDL_position realPos = backpack_cursorPosition;

    // 在搜尋區域，回搜尋區位置
    if(backpack_cursorArea == searchArea)
    {
        SDL_size marginSize = BACKPACK_MARGIN_SIZE;
        realPos.x = blockAreaStartPos.x;
        realPos.y = blockAreaStartPos.y + BACKPACK_HEIGHT_CELL_NUM * cellSize.height + marginSize.height; // 算回搜尋區域
        return realPos;
    }

    // 其他區域，如果不存在(就是 沒有區域 )就回不存在
    if(realPos.x == POS_NOT_EXISTS)
        return realPos;
    // 方塊區域，算真正左上角座標，才更新 cursor 位置
    realPos.x = realPos.x * cellSize.width + blockAreaStartPos.x;
    realPos.y = realPos.y * cellSize.height + blockAreaStartPos.y;
    return realPos;
}
// 取得 背包大小
SDL_size Backpack_GetSize()
{
    SDL_size cellSize = BACKPACK_CELL_SIZE;
    SDL_size marginSize = BACKPACK_MARGIN_SIZE;
    SDL_size searchAresSize = BACKPACK_SEARCH_AREA_SIZE;
    return (SDL_size) {.width = marginSize.width *2 + cellSize.width * BACKPACK_WIDTH_CELL_NUM
                     , .height = marginSize.height * 3 + cellSize.height * BACKPACK_HEIGHT_CELL_NUM + searchAresSize.height}; // *3 來自最上與最下，還有搜尋區域與正常方塊區域的間隔
}
// 取得 背包cursor 大小
SDL_size Backpack_GetCursorSize()
{
    if(backpack_cursorArea != searchArea) 
        return BACKPACK_CELL_SIZE;
    else // 在搜尋區域要回傳特殊大小
        return BACKPACK_SEARCH_AREA_SIZE;
}
// get Cell方塊之間間隔
SDL_size Backpack_GetCellSize()
{
    return BACKPACK_CELL_SIZE;
}
// get Backpack 內部方塊開始位置 (取置中的左上)
SDL_position Backpack_GetInCellBlockStartPos()
{
    SDL_position backpackBlockAreaLeftUpPos = Backpack_GetBlockStartPosition();
    SDL_size cellSize = BACKPACK_CELL_SIZE;
    SDL_size blockInCellSize = BACKPACK_BLOCK_IN_CELL_SIZE;
    return (SDL_position){.x = backpackBlockAreaLeftUpPos.x +cellSize.width/2 - blockInCellSize.width/2
                        , .y = backpackBlockAreaLeftUpPos.y +cellSize.height/2 - blockInCellSize.height/2};
}
// get Backpack 內部方塊大小
SDL_size Backpack_GetInCellBlockSize()
{
    return BACKPACK_BLOCK_IN_CELL_SIZE;
}
// get Backpack 長寬方塊數量
SDL_size Backpack_GetBlockNumberInWidthAndHeight()
{
    return (SDL_size) {.width = BACKPACK_WIDTH_CELL_NUM, .height = BACKPACK_HEIGHT_CELL_NUM};
}


// 取得現在 cursor 在方塊區、還是在搜尋區
cursorArea Backpack_GetCursorOnArea()
{
    return backpack_cursorArea;
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
