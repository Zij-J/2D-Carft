// 需要的函式庫
#include "./include/basicSetting.h" //請把header都寫在 include 資料夾中！
#include "./include/SDL_StartAndEnd.h"
#include "./include/texture.h"
#include "./include/ui.h"
#include "./include/map.h"
#include "./include/render.h"
#undef main

// 背包的search、sort、方塊部分測試開關，完成code後，改成1就可以開啟
#define TESTING_BACKPACK_FUNCTIONS 0
// 地圖移動測試開關，完成code後，改成1就可以開啟
#define TESTING_MAP_FUNCTIONS 1


// main函式
int main(void) 
{
    // 創立所有 SDL 需要的東西
    SDL_Window* window; 
    SDL_Renderer* renderer;
    SDL_InitializeAll(&window, &renderer);

    // 資料庫準備初始化 + render 準備
    // TextureBase_Init();
    // HotBar_Init();
    Map_Init();
    Render_Init(renderer);
    
    // 偵測輸入(主迴圈)
    SDL_Event event;
    while (true) 
    {
        // get 輸入
        SDL_PollEvent(&event); // 無論有沒有都輸入都執行 loop ，似真正遊戲
        
        // 按叉叉，結束
        if (event.type == SDL_QUIT)
            break;
        
        // 清除畫面 + 畫背景
        SDL_RenderClear(renderer);
        Render_RenderBackground();

        // 開關背包
        Backpack_Switch(event);

        // 是否有開背包：有
        if(Backpack_isOpen() == true)
        {
            // 畫背包到 render
            // Render_RenderBackpack();
        
            // 移動背包的 cursor、是否有輸入
            //Backpack_MoveCursor(event);

            #if TESTING_BACKPACK_FUNCTIONS
            // 有輸入、再執行背包輸入部分
            if(Backpack_isInput(event) == true)
            {
                // 在方塊區域、更新選到的方塊到快捷欄
                if(Backpack_GetCursorOnArea() == blockArea)
                    // Backpack_UpdateBlockToHotbar();
                // 在搜尋區域
                if(Backpack_GetCursorOnArea() == searchArea)
                {
                    // 輸入搜尋文字
                    SearchWords_GetInputWord(event);

                    // 搜尋方塊(找到回傳 true)，把 cursor 移到那個方塊上，印 找到了 文字
                    if(TextureBase_isFindBlockBySearchWords() == true)
                    {
                        // Backpack_FindBlockAndMoveCursor();
                        Render_RenderSearchNotify(true);
                    }
                    // 沒找到，印 提醒沒找到文字
                    else
                        Render_RenderSearchNotify(false);
                }   
            }
            #endif
            
            // 畫背包的 cursor、搜尋文字 到 renderer
            Render_RenderSearchWords();
            // Render_RenderBackpackCursor();
        }
        // 是否有開背包：沒有
        else
        {
            #if TESTING_MAP_FUNCTIONS
            // 編輯、移動地圖部分
            Map_EditBlock(event); // 依輸入放置、刪除方塊
            Render_MoveCamera(event); // 依輸入移動 camera
            Map_UpdateMaps(); // 依移動結果更新地圖
            #endif
            // 依輸入移動 cursor 不斷更新 cursor 位置
            Map_MoveCursor(event); 
        }
        
        // 畫出地圖、地圖cursor
        Render_RenderMap();
        //Render_RenderMapCursor();

        // 移動、顯示快捷欄
        // HotBar_MoveCursor();
        // Render_RenderHotbar();
        // Render_RenderHotbarCursor();

        // 畫出畫面，迴圈結束
        SDL_RenderPresent(renderer);
    }

    // 結束程式
    Render_Clear();
    Map_Clear();
    // HotBar_Clear();
    // TextureBase_Clear();
    SDL_EndAll_StopProgram();
}