/* 方塊(材質)資料庫 */

#include "../include/basicSetting.h" //要用的
#include "../include/blockDataBase_Texture.h" //要放的

// 初始化資料庫
void storedBlock_InitArray(storedBlock_ArrayAndSize *storedBlock_ArrayRecord)
{
    *storedBlock_ArrayRecord = (storedBlock_ArrayAndSize) {.array = (storedBlock_Data *)malloc(sizeof(storedBlock_Data) * INIT_ARRAY_SIZE), \
                                                            .storedSize = 0 ,.maxSize = INIT_ARRAY_SIZE}; 
}