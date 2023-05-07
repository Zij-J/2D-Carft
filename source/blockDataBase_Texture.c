/* 方塊(材質)資料庫 */
#include "../include/basicSetting.h" //要用的
#include "../include/SDL_StartAndEnd.h" //要用的
#include "../include/blockDataBase_Texture.h" //要放的


// storedBlcokData 建立！(方塊位置資料庫：方塊種類(名字) char *, 真的材質 SDL_Texture * (材質都是用 pointer 運作) )
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


// 初始化資料庫
public void storedBlock_InitArray(storedBlock_DataBase *storedBlock_ArrayRecord)
{
    //*storedBlock_ArrayRecord = (storedBlock_ArrayAndSize *)malloc(sizeof(storedBlock_ArrayAndSize)); //不知為何可以不先分配位置就給值了。總之還是補上
    //storedBlock_ArrayRecord->storedSize = 10;
    *storedBlock_ArrayRecord = (storedBlock_ArrayAndSize) {.array = (storedBlock_Data *)malloc(sizeof(storedBlock_Data) * INIT_ARRAY_SIZE), \
                                                            .storedSize = 10 ,.maxSize = INIT_ARRAY_SIZE}; 
}

// 加入(匯入)方塊材質
public void storedBlock_AddTexture(storedBlock_DataBase storedBlock_ArrayRecord, char *blockName, SDL_Window *window, SDL_Renderer *renderer)
{
    printf("%d", (*storedBlock_ArrayRecord).storedSize);
    // 滿了！先擴充資料庫！
    if((*storedBlock_ArrayRecord).storedSize == (*storedBlock_ArrayRecord).maxSize)
    {
        (*storedBlock_ArrayRecord).maxSize *= 2; //助教說要擴兩倍比+1快！
        (*storedBlock_ArrayRecord).array = (storedBlock_Data *) realloc((*storedBlock_ArrayRecord).array, sizeof(storedBlock_Data) * (*storedBlock_ArrayRecord).maxSize); //記得realloc需要有變數接住！
    }
    
    // ++總數後算 index 
    ++(*storedBlock_ArrayRecord).storedSize; //先+1，之後再-1說是0-base，才是第n項存在第n-1項
    int nowIndex = (*storedBlock_ArrayRecord).storedSize - 1;
    
    // 算 "block_pictures/%s.png" 的總長度
    int pathWordLength = (int)strlen("block_pictures/.png") + (int)strlen(blockName) + 1; //要含 \0

    // Load Texture
    char fullTexturePath[pathWordLength]; // sprintf 需要可改的空間，所以需用Array
    sprintf(fullTexturePath, "block_pictures/%s.png", blockName); //把名字接到路徑，可以用路徑load！
    SDL_Texture *getBlockTexture = IMG_LoadTexture(renderer, fullTexturePath);
    if (!getBlockTexture) //失敗會回傳 NULL
    {
        fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
        SDL_EndAll_StopProgram(window, renderer);
    }

    // 把Texture、名字直接 Load 到 array 內！
    (*storedBlock_ArrayRecord).array[nowIndex] = (storedBlock_Data) {.blockName = strdup(blockName), .blockTexture = getBlockTexture};  
}

// 依名字取得材質 (return NULL 如果找不到)
public SDL_Texture *storedBlock_GetTexture(storedBlock_DataBase storedBlock_ArrayRecord, char *textureName)
{
    for(int i = 0; i < (*storedBlock_ArrayRecord).storedSize; ++i)
    {
        // 名字一樣，那此方塊的 Texture 就是這個！
        if(strcmp( (*storedBlock_ArrayRecord).array[i].blockName, textureName) == 0)
            return (*storedBlock_ArrayRecord).array[i].blockTexture;
    }

    return NULL;
}

// 清除資料庫，刪除已 load 的材質
public void storedBlock_ClearDataBase(storedBlock_DataBase storedBlock_ArrayRecord)
{
    // 清除材質
    for(int i = 0; i < (*storedBlock_ArrayRecord).storedSize; ++i)
    {
        SDL_Texture *nowBlockTexture = (*storedBlock_ArrayRecord).array[i].blockTexture;
        SDL_DestroyTexture(nowBlockTexture);
    }
    free(storedBlock_ArrayRecord); //把 struct free 掉
}