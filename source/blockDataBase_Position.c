/* 方塊位置資料庫 */
#include "../include/basicSetting.h" //要用的
#include "../include/blockDataBase_Position.h" //要放的


// 初始化資料庫
void placedBlock_InitArray(placedBlock_ArrayAndSize *placedBlock_ArrayRecord)
{
    *placedBlock_ArrayRecord = (placedBlock_ArrayAndSize) {.array = (placedBlcok_Data *)malloc(sizeof(placedBlcok_Data) * INIT_ARRAY_SIZE), \
                                                            .storedSize = 0 , .maxSize = INIT_ARRAY_SIZE}; 
}

// 把所有方塊畫上去
void placedBlock_RenderAllBlocks(SDL_Renderer *renderer, placedBlock_ArrayAndSize *placedBlock_ArrayRecord)
{
    
}