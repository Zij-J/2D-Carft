/* 方塊位置資料庫 */
#include "../include/basicSetting.h" //要用的
#include "../include/blockDataBase_Position.h" //要放的



// 初始化資料庫
public void placedBlock_InitArray(placedBlock_ArrayAndSize *placedBlock_ArrayRecord)
{
    *placedBlock_ArrayRecord = (placedBlock_ArrayAndSize) {.array = (placedBlock_Data *)malloc(sizeof(placedBlock_Data) * INIT_ARRAY_SIZE), \
                                                            .storedSize = 0 , .maxSize = INIT_ARRAY_SIZE}; 
}

// 加入(放上)方塊
public void placedBlock_AddBlock(placedBlock_ArrayAndSize *placedBlock_ArrayRecord, char *blockName, int x, int y)
{
    // 滿了！先擴充資料庫！
    if((*placedBlock_ArrayRecord).storedSize == (*placedBlock_ArrayRecord).maxSize)
    {
        (*placedBlock_ArrayRecord).maxSize *= 2; //助教說要擴兩倍比+1快！
        (*placedBlock_ArrayRecord).array = (placedBlock_Data *) realloc((*placedBlock_ArrayRecord).array, sizeof(placedBlock_Data) * (*placedBlock_ArrayRecord).maxSize); //記得realloc需要有變數接住！
    }

    // ++總數後算 index 
    ++(*placedBlock_ArrayRecord).storedSize; //先+1，之後再-1說是0-base，才是第n項存在第n-1項
    int nowIndex = (*placedBlock_ArrayRecord).storedSize - 1;

    // 加入資料進 DataBase
    (*placedBlock_ArrayRecord).array[nowIndex] = (placedBlock_Data) {.blockName = strdup(blockName), .x = x, .y = y};
}