/* 方塊位置資料庫 */

// placedBlcokData 建立！(方塊位置資料庫：方塊種類(名字) char *, (x, y)座標 )
struct blockDataBase_Position
{
    char *blockName;
    int x, y;
};
typedef struct blockDataBase_Position placedBlock_Data;
// 可變動大小的 placedBlcokData array
struct placedBlockArrayStruct
{
    placedBlock_Data *array;
    int storedSize, maxSize;
};
typedef struct placedBlockArrayStruct placedBlock_ArrayAndSize;


// prototype

void placedBlock_InitArray(placedBlock_ArrayAndSize *placedBlock_ArrayRecord);
void placedBlock_AddBlock(placedBlock_ArrayAndSize *placedBlock_ArrayRecord, char *blockName, int x, int y);
void placedBlock_RenderAllBlocks(SDL_Renderer *renderer, placedBlock_ArrayAndSize *placedBlock_ArrayRecord);