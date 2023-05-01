/* 方塊(材質)資料庫 */

// 初始 Array大小 for 可變動array
#define INIT_ARRAY_SIZE 2

// placedBlcokData 建立！(方塊位置資料庫：方塊種類(名字) char *, 真的材質 SDL_Texture * (材質都是用 pointer 運作) )
struct blockDataBase_Texture
{
    char *blockName;
    SDL_Texture *blockTexture;
};
typedef struct blockDataBase_Texture storedBlock_Data;
// 可變動大小的 storedBlcokData array
struct storedBlockArrayStruct
{
    storedBlock_Data *array;
    int storedSize, maxSize;
};
typedef struct storedBlockArrayStruct storedBlock_ArrayAndSize;


// prototype

void storedBlock_InitArray(storedBlock_ArrayAndSize *storedBlock_ArrayRecord);
void storedBlock_AddTexture(storedBlock_ArrayAndSize *storedBlock_ArrayRecord, char *blockName, SDL_Window *window, SDL_Renderer *renderer);
