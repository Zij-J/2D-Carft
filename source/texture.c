/* 材質(背包)資料庫(紅黑樹？)、Sort、Search 與 相關function (file I/O：編號對應材質名 file、圖片資料夾)*/
#include "../include/basicSetting.h" // 要用的
#include "../include/texture.h" // 要放的
#include <string.h>
#include <dirent.h>
#include <io.h>

#define INIT_ARRAY_SIZE 100

struct blockDataBase_Texture
{
    int blockID;
    char *blockName;
    SDL_Texture *blockTexture;
    struct blockDataBase_Texture *next;    
};
typedef struct blockDataBase_Texture blockBase_Data;

struct storedBlockArrayStruct
{
    blockBase_Data *array;
    int storedSize;
    int maxSize;
    blockBase_Data *head;
};
typedef struct storedBlockArrayStruct storedBlock_ArrayAndSize;
typedef storedBlock_ArrayAndSize *storedBlock_DataBase;


// 初始化資料庫
void TextureBase_Init(storedBlock_DataBase* storedBlock_ArrayRecord)
{
    (*storedBlock_ArrayRecord) = (storedBlock_ArrayAndSize*)malloc(sizeof(storedBlock_ArrayAndSize));
    (*storedBlock_ArrayRecord)->array = (blockBase_Data*)malloc(sizeof(blockBase_Data) * INIT_ARRAY_SIZE);    
    (*storedBlock_ArrayRecord)->array->blockName = NULL;    
    (*storedBlock_ArrayRecord)->storedSize = 0;
    (*storedBlock_ArrayRecord)->maxSize = INIT_ARRAY_SIZE;
    (*storedBlock_ArrayRecord)->head = NULL;
}

// 清除資料庫
void TextureBase_Clear(storedBlock_DataBase storedBlock_ArrayRecord)
{    
    blockBase_Data* current = storedBlock_ArrayRecord->head;
    while (current != NULL)
    {
        blockBase_Data* nextBlock = current->next;
        SDL_DestroyTexture(current->blockTexture);       
        free(current->blockName);
        free(current);
        current = nextBlock;
    }

    storedBlock_ArrayRecord = NULL;
    storedBlock_ArrayRecord->storedSize = 0;
    storedBlock_ArrayRecord->maxSize = 0;
    free(storedBlock_ArrayRecord->array);
    free(storedBlock_ArrayRecord);

    for(int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    {
        SDL_Texture *nowBlockTexture = storedBlock_ArrayRecord->array[i].blockTexture;
        SDL_DestroyTexture(nowBlockTexture);
    }    
}

//複製資料夾方塊名稱
char* duplicateString(const char* str)
{
    size_t length = strlen(str);
    char* duplicate = (char*)malloc(length + 1); 
    if (duplicate != NULL)
    {
        strcpy(duplicate, str);
    }
    return duplicate;
}

// 按方塊名稱進行排序的比較
int compareTextures(const void* a, const void* b)
{
    const blockBase_Data* textureA = (const blockBase_Data*)a;
    const blockBase_Data* textureB = (const blockBase_Data*)b;
    return strcmp(textureA->blockName, textureB->blockName);
}

//從圖片資料夾匯入圖片並依名稱排序
void TextureBase_GetAllBlock(storedBlock_DataBase storedBlock_ArrayRecord, const char *folderPath, SDL_Window *window, SDL_Renderer *renderer)
{
    DIR *dir;
    struct dirent *entry;
    char filePath[256];

    dir = opendir(folderPath);
    if (dir == NULL)
    {
        fprintf(stderr, "Failed to open directory: %s\n", folderPath);
        return;
    }

    int index = 0;  //方塊資料庫索引

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Check if the file has a ".png" extension
        if (strstr(entry->d_name, ".png") == NULL)
            continue;

        // Construct the full file path
        snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, entry->d_name);

        // Load the image as a surface
        SDL_Surface *surface = IMG_Load(filePath);
        if (!surface)
        {
            fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
            continue;
        }

        // Create a texture from the surface
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture)
        {
            fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
            SDL_FreeSurface(surface);
            continue;
        }

        // Extract the file name without extension
        char *fileName = entry->d_name;
        char *extension = strstr(fileName, ".png");
        if (extension != NULL)
            *extension = '\0';

        // Add the texture to the blockBase_Data
        storedBlock_ArrayRecord->array[index].blockName = duplicateString(fileName);
        storedBlock_ArrayRecord->array[index].blockID = index;//索引為方塊編號
        storedBlock_ArrayRecord->array[index].blockTexture = texture;
        storedBlock_ArrayRecord->array[index].next = NULL;
        index++;

        if (index >= storedBlock_ArrayRecord->maxSize)
        {           
            storedBlock_ArrayRecord->maxSize *= 2;
            storedBlock_ArrayRecord->array = (blockBase_Data *)realloc(storedBlock_ArrayRecord->array, sizeof(blockBase_Data) * storedBlock_ArrayRecord->maxSize);
        }
    }    
    storedBlock_ArrayRecord->storedSize = index;

    //依照方塊名稱排序
    qsort(storedBlock_ArrayRecord->array, storedBlock_ArrayRecord->storedSize, sizeof(blockBase_Data), compareTextures);
    closedir(dir);
}

// 載入圖片資料夾圖片
SDL_Texture *loadTexture(const char *filePath, SDL_Renderer *renderer)
{
    // Load the image as a surface
    SDL_Surface *surface = IMG_Load(filePath);
    if (!surface)
    {
        fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
        return NULL;
    }

    // Create a texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);

    return texture;
}

// 依方塊名稱取得圖片
SDL_Texture *TextureBase_GetTextureName(storedBlock_DataBase storedBlock_ArrayRecord, char* textureName)
{
    for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    {
        if (strcmp(storedBlock_ArrayRecord->array[i].blockName, textureName) == 0)
            return storedBlock_ArrayRecord->array[i].blockTexture;
    }
    blockBase_Data* current = storedBlock_ArrayRecord->head;
    while (current != NULL)
    {
        if (strcmp(current->blockName, textureName) == 0)
            return current->blockTexture;
        current = current->next;
    }
    return NULL;
} 

//刪除指定圖片
void TextureBase_DeleteTexture(storedBlock_DataBase storedBlock_ArrayRecord, const char* textureName)
{
    // 搜尋欲刪除方塊名稱
    int index = -1;
    for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    {
        if (strcmp(storedBlock_ArrayRecord->array[i].blockName, textureName) == 0)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        // 將方塊從資料庫移除
        SDL_DestroyTexture(storedBlock_ArrayRecord->array[index].blockTexture);
        free(storedBlock_ArrayRecord->array[index].blockName);
    
        for (int i = index; i < storedBlock_ArrayRecord->storedSize - 1; ++i)
        {
            storedBlock_ArrayRecord->array[i] = storedBlock_ArrayRecord->array[i + 1];
        }

        // 更新資料庫大小
        storedBlock_ArrayRecord->storedSize--;
    }
}

// 匯入編號對應材質名字 file
void IDtoNameBase_Init(storedBlock_DataBase* IDtoNameBase, storedBlock_DataBase storedBlock_ArrayRecord)
{
    *IDtoNameBase = (storedBlock_ArrayAndSize*)malloc(sizeof(storedBlock_ArrayAndSize));
    (*IDtoNameBase)->array = (blockBase_Data*)malloc(sizeof(blockBase_Data) * INIT_ARRAY_SIZE);
    (*IDtoNameBase)->storedSize = storedBlock_ArrayRecord->storedSize;
    (*IDtoNameBase)->head = NULL;

    for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    {
        blockBase_Data* newData = &(storedBlock_ArrayRecord->array[i]);
        (*IDtoNameBase)->array[i].blockID = newData->blockID;
        (*IDtoNameBase)->array[i].blockName = strdup(newData->blockName);
        (*IDtoNameBase)->array[i].blockTexture = newData->blockTexture;

        if ((*IDtoNameBase)->head == NULL)
        {
            (*IDtoNameBase)->head = &((*IDtoNameBase)->array[i]);
            (*IDtoNameBase)->array[i].next = NULL;
        }
        else
        {
            (*IDtoNameBase)->array[i].next = (*IDtoNameBase)->head;
            (*IDtoNameBase)->head = &((*IDtoNameBase)->array[i]);
        }
    }
}

// 清除編號對應材質名字 file
void IDtoNameBase_Clear(storedBlock_DataBase* IDtoNameBase)
{
    if (*IDtoNameBase != NULL)
    {
        blockBase_Data* current = (*IDtoNameBase)->head;
        while (current != NULL)
        {
            blockBase_Data* temp = current;
            current = current->next;
            free(temp->blockName);
            SDL_DestroyTexture(temp->blockTexture);
            free(temp);
        }          
        free((*IDtoNameBase)->array);
        free(*IDtoNameBase);
        *IDtoNameBase = NULL;
    }      
}

// 取得材質資料庫所有的材質ID 
short int * TextureBase_GetAllID(const storedBlock_DataBase* IDtoNameBase)
{
    short* materialID = (short*)malloc(sizeof(short) * (*IDtoNameBase)->storedSize);
    for (int i = 0; i < (*IDtoNameBase)->storedSize; ++i)
    {
        materialID[i] = (*IDtoNameBase)->array[i].blockID;
    }
    return materialID;
}


// 依搜尋文字，找找看是否有方塊是此名字，回傳是否找到
bool TextureBase_isFindBlockBySearchWords(storedBlock_DataBase* IDtoNameBase, char* textureName)
{
    for (int i = 0; i < (*IDtoNameBase)->storedSize; ++i)
    {
        if (strcmp((*IDtoNameBase)->array[i].blockName, textureName) == 0)
            return true;
    }
    blockBase_Data* current = (*IDtoNameBase)->head;
    while (current != NULL)
    {
        if (strcmp(current->blockName, textureName) == 0)
            return true;
        current = current->next;
    }
    return false;
} 

// 把搜尋到的方塊編號回傳
short TextureBase_GetSearchedBlockID(storedBlock_DataBase* IDtoNameBase, char* textureName)
{
    for (int i = 0; i < (*IDtoNameBase)->storedSize; ++i)
    {
        if (strcmp((*IDtoNameBase)->array[i].blockName, textureName) == 0)
            return (*IDtoNameBase)->array[i].blockID;
    }
    
    blockBase_Data* current = (*IDtoNameBase)->head;
    while (current != NULL)
    {
        if (strcmp(current->blockName, textureName) == 0)
            return current->blockID;
        current = current->next;
    }
    
    // Return -1 if the block is not found
    return -1;
}

//依照方塊編號大小排序資料庫
SDL_Texture *TextureBase_GetTextureID(storedBlock_DataBase* IDtoNameBase)
{
    // Sort the array by blockID in ascending order using bubble sort
    for (int i = 0; i < (*IDtoNameBase)->storedSize - 1; ++i)
    {
        for (int j = 0; j < (*IDtoNameBase)->storedSize - i - 1; ++j)
        {
            if ((*IDtoNameBase)->array[j].blockID > (*IDtoNameBase)->array[j + 1].blockID)
            {
                // Swap the elements
                blockBase_Data temp = (*IDtoNameBase)->array[j];
                (*IDtoNameBase)->array[j] = (*IDtoNameBase)->array[j + 1];
                (*IDtoNameBase)->array[j + 1] = temp;
            }
        }
    }
}

