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
#define TESTING_MAP_FUNCTIONS 0


// main函式
int main(int argc, char** argv) 
{
    // 創立所有 SDL 需要的東西
    SDL_Window* window; 
    SDL_Renderer* renderer;
    SDL_InitializeAll(&window, &renderer);

    // 資料庫初始化
    // TextureBase_Init();
    // HotBar_Init();
    // Maps_Init();
    
    // 偵測輸入(主迴圈)
    SDL_Event event;
    while (true) 
    {
        // get 輸入
        SDL_PollEvent(&event); // 無論有沒有都輸入都執行 loop ，似真正遊戲

        // 按叉叉，結束
        if (event.type == SDL_QUIT)
            break;

        // 清除畫面
        SDL_RenderClear(renderer);

        // 開關背包
        Backpack_Switch(&event);

        // 是否有開背包：有
        if(Backpack_isOpen() == true)
        {
            // 畫背包到 render
            // Render_RenderBackpack();
        
            // 移動背包的 cursor、是否有輸入
            //Backpack_MoveCursor();
            
            #if TESTING_BACKPACK_FUNCTIONS
            // 有輸入、再執行背包輸入部分
            if(Backpack_isInput(&event) == true)
            {
                
            }
            #endif

            // 畫背包的 cursor 到 renderer
            // Render_RenderBackpackCursor();
        }
        // 是否有開背包：沒有
        else
        {
            #if TESTING_MAP_FUNCTIONS
            // 有輸入、再執行移動地圖部分
            if(Map_isInput(&event) == true)
            {
                
            }
            #endif

            // 畫出地圖、地圖cursor
            //Render_RenderMap();
            //Render_RenderBackpackCursor();
        }

        // 移動、顯示快捷欄
        // HotBar_MoveCursor();
        // Render_RenderHotbar();
        // Render_RenderHotbarCursor();

        // 畫出畫面，迴圈結束
        SDL_RenderPresent(renderer);
    }

    // 結束程式
    // Maps_Clear();
    // HotBar_Clear();
    // TextureBase_Clear();
    SDL_EndAll_StopProgram(&window, &renderer);
}
