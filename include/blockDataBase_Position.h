/* 方塊位置資料庫 */

// 初始 Array大小 for 可變動array
#define INIT_ARRAY_SIZE 2

// placedBlcokData 建立！(方塊位置資料庫：方塊種類(名字) char *, (x, y)座標 )
struct blockDataBase_Position
{
    char *blockName;
    int x, y;
};
typedef struct blockDataBase_Position placedBlcok_Data;
// 可變動大小的 placedBlcokData array
struct placedBlockArrayStruct
{
    placedBlcok_Data *array;
    int storedSize, maxSize;
};
typedef struct placedBlockArrayStruct placedBlock_ArrayAndSize;


// prototype

void placedBlock_InitArray(placedBlock_ArrayAndSize *placedBlock_ArrayRecord);
void placedBlock_RenderAllBlocks(SDL_Renderer *renderer, placedBlock_ArrayAndSize *placedBlock_ArrayRecord);