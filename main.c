// 需要的函式庫
#include "./include/basicSetting.h" //請把header都寫在 include 資料夾中！
#include "./include/SDL_StartAndEnd.h"
#include "./include/blockDataBase_Position.h"
#include "./include/blockDataBase_Texture.h"
#include "./include/blockDataBase_Render.h"
#undef main

// 顯示所有的圖片 (*placedBlock_ArrayRecord 用 pointer 傳，才不用複製貼上一堆)
private void SDL_RenderAllPicture(SDL_Renderer *renderer, SDL_Window *window);


// main函式
int main(int argc, char** argv) 
{
    // 創立所有 SDL 需要的東西
    SDL_Window* window; 
    SDL_Renderer* renderer;
    SDL_InitializeAll(&window, &renderer);

    // all 資料庫初始化
    storedBlock_DataBase storedBlock_ArrayRecord; //因為其他功能需要使用此array，所以寫在外面而非source code內
    placedBlock_ArrayAndSize placedBlock_ArrayRecord;
    storedBlock_InitArray(storedBlock_ArrayRecord);
    placedBlock_InitArray(&placedBlock_ArrayRecord);
    renderedBlock_InitArray();


    // 給入資料庫 (載入Grass_Block為Texture + 放上 grass_block)
    // 材質部分
    storedBlock_AddTexture(storedBlock_ArrayRecord, "Grass_Block", window, renderer);
    // 放置部分 (這是平面)
    for(int i = 0; i < TOTAL_BLOCK_NUMBER_IN_WIDTH ; ++i)
        placedBlock_AddBlock(&placedBlock_ArrayRecord, "Grass_Block", 0 + BLOCK_WIDTH * i, WINDOW_HEIGHT - 2 * BLOCK_WIDTH);
    // 掃入 renderDataBase
    renderedBlock_ScanWindow(storedBlock_ArrayRecord, &placedBlock_ArrayRecord);

    // 輸入！
    SDL_Event event;
    while (SDL_WaitEvent(&event)) //我們是有輸入才有動作，所以用 SDL_WaitEvent 即可
    {
        // 按叉叉，結束
        if (event.type == SDL_QUIT) 
            break;

        // 輸入 w 就前進 (會卡輸入法！)
        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_w)
                printf("forward");
            else
                printf("backward");
        }
        
        
        // 顯示所有圖片
        SDL_RenderAllPicture(renderer, window);
    }

    // 關閉資料庫
    storedBlock_ClearDataBase(storedBlock_ArrayRecord); //刪除材質在這裡


    // Close Program
    SDL_EndAll_StopProgram(window, renderer);
}




// 感覺大家會用到很多這個(顯示圖片)，所以放在 main.c 中  //或許需要 Texture 資料庫，不然要一直 delete & load
static void SDL_RenderAllPicture(SDL_Renderer *renderer, SDL_Window *window)
{
    // Clear renderer
    SDL_RenderClear(renderer);
    
    // 顯示所有 renderedBlock
    renderedBlock_RenderToRenderer(renderer);
   
    // Update renderer
    SDL_RenderPresent(renderer);
}