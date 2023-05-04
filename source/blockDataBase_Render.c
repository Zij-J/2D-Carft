/* 要render的方塊的資料庫 */
#include "../include/basicSetting.h" //要用的
#include "../include/blockDataBase_Position.h" //要用的
#include "../include/blockDataBase_Texture.h" //要用的
#include "../include/blockDataBase_Render.h" //要放的


// renderedBlcokData 建立！(要render的方塊的資料庫：真的材質 SDL_Texture * (材質都是用 pointer 運作), (x, y)座標 ) //因為只是render，不用外用，放給自己用即可
struct blockDataBase_Render
{
    SDL_Texture *blockTexture;
    int x, y;
};
typedef struct blockDataBase_Render renderedBlock_Data;
// 可變動大小的 renderedBlcokData array
struct renderedBlockArrayStruct
{
    renderedBlock_Data *array;
    int storedSize, maxSize;
};
typedef struct renderedBlockArrayStruct renderedBlock_ArrayAndSize;



// 自用的 renderedBlcokData array, camera位置(比預設移動了多少)
static renderedBlock_ArrayAndSize renderedBlock_ArrayRecord;
static renderedBlock_cameraPosition cameraPosition;


// 初始化資料庫
void renderedBlock_InitArray()
{
    renderedBlock_ArrayRecord = (renderedBlock_ArrayAndSize) {.array = (renderedBlock_Data *)malloc(sizeof(renderedBlock_Data) * INIT_ARRAY_SIZE), \
                                                            .storedSize = 0 , .maxSize = INIT_ARRAY_SIZE}; 
    cameraPosition = (renderedBlock_cameraPosition) {.x = 0, .y = 0}; //預設是沒移動
}

// 加入(要畫的)方塊
private void renderedBlock_AddBlock(SDL_Texture *blockTexture, int x, int y)
{
    // 滿了！先擴充資料庫！
    if(renderedBlock_ArrayRecord.storedSize == renderedBlock_ArrayRecord.maxSize)
    {
        renderedBlock_ArrayRecord.maxSize *= 2; //助教說要擴兩倍比+1快！
        renderedBlock_ArrayRecord.array = (renderedBlock_Data *) realloc(renderedBlock_ArrayRecord.array, sizeof(renderedBlock_Data) * renderedBlock_ArrayRecord.maxSize); //記得realloc需要有變數接住！
    }

    // ++總數後算 index 
    ++renderedBlock_ArrayRecord.storedSize; //先+1，之後再-1說是0-base，才是第n項存在第n-1項
    int nowIndex = renderedBlock_ArrayRecord.storedSize - 1;

    // 加入資料進 DataBase
    renderedBlock_ArrayRecord.array[nowIndex] = (renderedBlock_Data) {.blockTexture = blockTexture, .x = x, .y = y};
}

// // 從 placedblock 掃現在 windows 應有的方塊，更改要顯示的方塊資料庫
// void renderedBlock_ScanWindow(storedBlock_DataBase storedBlock_ArrayRecord, placedBlock_DataBase placedBlock_ArrayRecord)
// {
//     for(int i = 0; i < (*placedBlock_ArrayRecord).storedSize; ++i)
//     {
//         // get Texture
//         SDL_Texture *neededBlockTexture = storedBlock_GetTexture(storedBlock_ArrayRecord, (*placedBlock_ArrayRecord).array[i].blockName);

//         // 放到顯示的renderer內
//         renderedBlock_AddBlock(neededBlockTexture, (*placedBlock_ArrayRecord).array[i].x, (*placedBlock_ArrayRecord).array[i].y);
//     }
// }


// 畫上所有要顯示的方塊
void renderedBlock_RenderToRenderer(SDL_Renderer *renderer)
{
    for(int i = 0; i < renderedBlock_ArrayRecord.storedSize; ++i)
    {
        // Set the destination rectangle
        SDL_Rect destRect;
        destRect.x = renderedBlock_ArrayRecord.array[i].x;  // x-coordinate of the destination rectangle
        destRect.y = renderedBlock_ArrayRecord.array[i].y;  // y-coordinate of the destination rectangle
        destRect.w = BLOCK_WIDTH;  // width of the destination rectangle
        destRect.h = BLOCK_WIDTH;  // height of the destination rectangle        

        // get Texture
        SDL_Texture *neededBlockTexture = renderedBlock_ArrayRecord.array[i].blockTexture;
        
        // Copy texture to renderer
        SDL_RenderCopy(renderer, neededBlockTexture, NULL, &destRect);
    }
}