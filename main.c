// 需要的函式庫
#include "./include/basicSetting.h" //請把header都寫在 include 資料夾中！
#include "./include/SDL_StartAndEnd.h"
#include "./include/blockDataBase_Position.h"
#include "./include/blockDataBase_Texture.h"
#undef main

// 顯示所有的圖片 (*placedBlock_ArrayRecord 用 pointer 傳，才不用複製貼上一堆)
static void SDL_RenderAllPicture(SDL_Renderer *renderer, SDL_Window *window, storedBlock_ArrayAndSize *storedBlock_ArrayRecord, placedBlock_ArrayAndSize *placedBlock_ArrayRecord);


// main函式
int main(int argc, char** argv) 
{
    // 創立所有 SDL 需要的東西
    SDL_Window* window; 
    SDL_Renderer* renderer;
    SDL_InitializeAll(&window, &renderer);

    // all 資料庫初始化
    storedBlock_ArrayAndSize storedBlock_ArrayRecord;
    placedBlock_ArrayAndSize placedBlock_ArrayRecord;
    storedBlock_InitArray(&storedBlock_ArrayRecord);
    placedBlock_InitArray(&placedBlock_ArrayRecord);


    // 給入資料庫 (載入Grass_Block為Texture + 放上兩個 grass_block)
    // 材質部分
    storedBlock_AddTexture(&storedBlock_ArrayRecord, "Grass_Block", window, renderer);
    // 放置部分 (這是平面)
    for(int i = 0; i < TOTAL_BLOCK_NUMBER_IN_WIDTH ; ++i)
        placedBlock_AddBlock(&placedBlock_ArrayRecord, "Grass_Block", 0 + BLOCK_WIDTH * i, WINDOW_HEIGHT - 2 * BLOCK_WIDTH);

    
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
                ++placedBlock_ArrayRecord.array[1].y;
            else
                --placedBlock_ArrayRecord.array[1].y;
        }
        

        // 顯示所有圖片
        SDL_RenderAllPicture(renderer, window, &storedBlock_ArrayRecord, &placedBlock_ArrayRecord);
    }

    
    // 清除材質
    for(int i = 0; i < storedBlock_ArrayRecord.storedSize; ++i)
    {
        SDL_Texture *nowBlockTexture = storedBlock_ArrayRecord.array[i].blockTexture;
        SDL_DestroyTexture(nowBlockTexture);
    }


    // Close Program
    SDL_EndAll_StopProgram(window, renderer);
}




// 感覺大家會用到很多這個(顯示圖片)，所以放在 main.c 中  //或許需要 Texture 資料庫，不然要一直 delete & load
static void SDL_RenderAllPicture(SDL_Renderer *renderer, SDL_Window *window, storedBlock_ArrayAndSize *storedBlock_ArrayRecord, placedBlock_ArrayAndSize *placedBlock_ArrayRecord)
{
    // Clear renderer
    SDL_RenderClear(renderer);

    
    // 顯示所有 placedBlock
    for(int i = 0; i < (*placedBlock_ArrayRecord).storedSize; ++i)
    {
        // Set the destination rectangle
        SDL_Rect destRect;
        destRect.x = (*placedBlock_ArrayRecord).array[i].x;  // x-coordinate of the destination rectangle
        destRect.y = (*placedBlock_ArrayRecord).array[i].y;  // y-coordinate of the destination rectangle
        destRect.w = BLOCK_WIDTH;  // width of the destination rectangle
        destRect.h = BLOCK_WIDTH;  // height of the destination rectangle        

        // get Texture
        SDL_Texture *neededBlockTexture;
        for(int i = 0; i < (*storedBlock_ArrayRecord).storedSize; ++i)
        {
            // 名字一樣，那此方塊的 Texture 就是這個！
            if(strcmp((*storedBlock_ArrayRecord).array[i].blockName, (*placedBlock_ArrayRecord).array[i].blockName) == 0)
            {
                neededBlockTexture = (*storedBlock_ArrayRecord).array[i].blockTexture;
                break;
            }       
        }
        
        // Copy texture to renderer
        SDL_RenderCopy(renderer, neededBlockTexture, NULL, &destRect);
    }
   
    // Update renderer
    SDL_RenderPresent(renderer);
}