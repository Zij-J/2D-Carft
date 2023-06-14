/* 材質(背包)資料庫(紅黑樹？)、Sort、Search 與 相關function (file I/O：編號對應材質名 file、圖片資料夾)*/
#include "../include/basicSetting.h" // 要用的
#include "../include/render.h" // 要用的
#include "../include/ui.h" // 要用的
#include "../include/SDL_StartAndEnd.h" // 要用的
#include "../include/texture.h" // 要放的
#include <string.h>
#include <dirent.h>
#include <io.h>

#define INIT_ARRAY_SIZE 2
#define MAX_ABBSOULTE_PATH_LENGTH 4096 // 這是 UNIX 絕對路徑最大值，Windows 只有 255
#define IDtoNAME_WORD_NONE_EXISTS "." // 用 . 表示此編號的字不存在，畢竟命名應該不能有 .
#define BLOCK_PICTURE_FOLDER_RELATIVE_PATH "./block_pictures"
#define IDtoNAME_RECORD_FILE_RELATIVE_PATH "./block_data/IDtoNameRecord.txt"

struct blockDataBase_Texture
{
    short blockID;
    int blockIndexInArray;
    char *blockName;
    SDL_Texture *blockTexture;
};
typedef struct blockDataBase_Texture blockBase_Data;

struct expendableArrayOfData
{
    char **array;
    int nowStored, nowMax;
};
typedef struct expendableArrayOfData expendableArray_string;


// 記住 TextureBase_isFindBlockBySearchWords()，找到的ID
short searchedBlockIndex = EOF; // 應該不會用到 EOF，但還是寫一下，代表找到過任何Block
// 創立 方塊(材質資料庫) // 記住 RBtree 遍歷結果，就不用每次呼叫都要重新遍歷
short *RBT_data_array; int RBT_data_array_element_number = 0;

// ID對應名字資料庫 (因ID是從0-數字，所以用string array直接對應)
int IDtoNameBase_totalNameNumber; // 記住總數，那只要算一次就夠
char **IDtoNameBase = NULL; // 要預設沒東西，表示有需要 + 不存在 才 init 用

//RBTree
typedef enum {BLACK, RED} COLOR;//node color
typedef enum {NONE, LEFT, RIGHT} SIDE;//parent's L or R child

struct tNode{
    COLOR color;
    blockBase_Data *blockData;
    struct tNode *Lchild;
    struct tNode *Rchild;
    struct tNode *parent;
    SIDE side;//parent's L or R child
}*root, *first;

struct tNode* RBT_init();//initiallize
void insert(struct tNode *, blockBase_Data*);//insert the node to red black tree
struct tNode* find(struct tNode*, blockBase_Data*);//find node by block's name
struct tNode* find_root(struct tNode *);//find red black tree root
private struct tNode* create_tNode(blockBase_Data*, struct tNode*, SIDE);
private void check(struct tNode*);
private void left_rotate(struct tNode*);
private void right_rotate(struct tNode*);
private void store_data(struct tNode *);//store blockID to array
private void cp_itsRelation(struct tNode *);


//複製資料夾方塊名稱
private char* duplicateString(const char* str)
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
private int compareTextures(const void* a, const void* b)
{
    const blockBase_Data* textureA = (const blockBase_Data*)a;
    const blockBase_Data* textureB = (const blockBase_Data*)b;
    return strcmp(textureA->blockName, textureB->blockName);
}

// 載入圖片資料夾圖片 (load 一次，之後要顯示圖片時只要存資料庫抓，不用再load)
private SDL_Texture *loadTexture(const char *filePath, SDL_Renderer *renderer)
{
    // Load the image as a surface
    SDL_Surface *surface = IMG_Load(filePath);
    if (!surface)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_YELLOW("Failed to load image: %s\n"), IMG_GetError());
        return NULL;
    }

    // Create a texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to create texture: %s\n"), SDL_GetError());
    }

    SDL_FreeSurface(surface);

    return texture;
}

//從圖片資料夾匯入圖片並依名稱排序
private void TextureBase_GetAllBlock(SDL_Renderer *renderer)
{
    DIR *dir;
    struct dirent *entry;
    char filePath[MAX_ABBSOULTE_PATH_LENGTH];
    if ((dir = opendir(BLOCK_PICTURE_FOLDER_RELATIVE_PATH)) == NULL) // 開資料夾路徑(可以開相對路徑！)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to open folder: %s\n"), BLOCK_PICTURE_FOLDER_RELATIVE_PATH);
        return ;
    }

    bool IDusedCheckList[IDtoNameBase_totalNameNumber]; // 紀錄哪個ID是實際被用到的 (那些是空的)
    for(int i = 0; i < IDtoNameBase_totalNameNumber; ++i)
        IDusedCheckList[i] = false;
    expendableArray_string unInsertfile = {.array = (char **)malloc(sizeof(char *) * INIT_ARRAY_SIZE)
                                        ,.nowMax = INIT_ARRAY_SIZE, .nowStored = 0}; // 紀錄那些是還沒存的


    // 第一輪，掃過資料夾一遍，insert：1. 有記IDtoName、在資料夾，2. 沒記IDtoName、在資料夾(IDtoName 空間仍夠)
    while ((entry = readdir(dir)) != NULL)
    {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Check if the file has a ".png" extension
        if (strstr(entry->d_name, ".png") == NULL)
            continue;

        // Construct the full file path
        snprintf(filePath, sizeof(filePath), "%s/%s", BLOCK_PICTURE_FOLDER_RELATIVE_PATH, entry->d_name); 

        // Extract the file name without extension
        char *fileName = entry->d_name;
        char *extension = strstr(fileName, ".png");
        if (extension != NULL)
            *extension = '\0';

        // 掃過，看看是否有編號！
        short ID = NO_BLOCK_ID;
        for(int i = 0; i < IDtoNameBase_totalNameNumber; ++i)
        {
            if(IDusedCheckList[i] == false && strcmp(fileName, IDtoNameBase[i]) == 0)
                ID = i;
        }
        // 還是沒編號，就給予編號
        if(ID == NO_BLOCK_ID)
        {
            for(int i = 0; i < IDtoNameBase_totalNameNumber; ++i)
            {
                if(IDusedCheckList[i] == false && strcmp(IDtoNAME_WORD_NONE_EXISTS, IDtoNameBase[i]) == 0)
                {
                    ID = i;
                    IDtoNameBase[i] = strdup(fileName);
                    break;
                }
            }
        }

        // 如果還是沒編號，就是 3.沒記IDtoName、在資料夾(IDtoName 不足)，需要等 4. 有記IDtoName、沒在資料夾被排除，才能盡可能塞進去，先入暫存
        if(ID == NO_BLOCK_ID)
        {
            ++unInsertfile.nowStored;
            int nowIndex = unInsertfile.nowStored - 1;
            unInsertfile.array[nowIndex] = strdup(fileName);
            if(unInsertfile.nowStored == unInsertfile.nowMax)
            {
                unInsertfile.nowMax *= 2;
                unInsertfile.array = (char **)realloc(unInsertfile.array, sizeof(char *) * unInsertfile.nowMax);
            }
        }
        // 有ID了，存入資料庫
        else
        {
            //RBTree
            blockBase_Data *blockData = (blockBase_Data *)malloc(sizeof(blockBase_Data));
            (*blockData).blockName = duplicateString(fileName);
            (*blockData).blockTexture = loadTexture(filePath, renderer); // load Texture 應該是在這裡
            (*blockData).blockID = ID;
            root=find_root(root);
            insert(root, blockData); // 取位置運算的優先級較高，所以要括號！
            // 紀錄此 ID 已用
            IDusedCheckList[ID] = true;
        }
    }
    closedir(dir);

    // 整理出剩的ID
    int leftID[IDtoNameBase_totalNameNumber]; int totalLeftIDnum = 0;
    for(int i = 0; i < IDtoNameBase_totalNameNumber; ++i)
    {
        if(IDusedCheckList[i] == false)
        {
            leftID[totalLeftIDnum] = i;
            ++totalLeftIDnum;
        }
    }
    if(totalLeftIDnum == IDtoNameBase_totalNameNumber)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("You must have at least one block!\n"));
        SDL_EndAll_StopProgram(false);
    }

    // 第二輪，排除 4. 有記IDtoName、沒在資料夾的(會影響地圖，需要通知)，讓 3. 沒記IDtoName、在資料夾(IDtoName 不足)，的補進去 
    for(int i = 0; i < unInsertfile.nowStored;++i)
    {
        if(totalLeftIDnum <= 0) // 沒空缺了！就說它們塞不進去
        {
            fprintf(stderr, "Backpack full, can't add: %s\n", unInsertfile.array[i]);
            continue;
        }

        --totalLeftIDnum; // 還有空缺，排除 4 塞 3
        short replacingID = leftID[totalLeftIDnum];
        char filPath[MAX_ABBSOULTE_PATH_LENGTH]; snprintf(filePath, sizeof(filePath), "%s/%s.png", BLOCK_PICTURE_FOLDER_RELATIVE_PATH, unInsertfile.array[i]); 
        SDL_Texture *texture = loadTexture(filePath, renderer);
        if(texture == NULL) // 可能開啟失敗，那要再繼續開！
        {
            ++totalLeftIDnum;
            continue;
        }
        blockBase_Data *blockData = (blockBase_Data *)malloc(sizeof(blockBase_Data));
        *blockData = (blockBase_Data){.blockID = replacingID, .blockName = duplicateString(unInsertfile.array[i])
                                    , .blockTexture = texture};
        root=find_root(root);
        insert(root, blockData); 

        fprintf(stderr, STRING_LITERAL_IN_PRINTF_YELLOW("Detact %s removed, adding %s as replacement.\n"), IDtoNameBase[replacingID], unInsertfile.array[i]);
        IDtoNameBase[replacingID] = strdup(unInsertfile.array[i]); // 告知、更新
    }

    // free掉 unInsertfile的文字 (strdup都是malloc)
    for(int i = 0; i < unInsertfile.nowStored;++i)
        free(unInsertfile.array[i]);
    free(unInsertfile.array);

    // ID找完，RB樹插入完了，在此就先遍歷！
    root=find_root(root);
    store_data(root); 
}

// 初始化資料庫
public void TextureBase_Init(SDL_Renderer *renderer)
{
    // 資料庫初始化
    IDtoNameBase_Init(); // IDtoName 先初始化，才可適當加入名字
    first=RBT_init(); //RBTree

    // 從圖片資料夾開啟方塊
    TextureBase_GetAllBlock(renderer);
}

// 清除資料庫
public void TextureBase_Clear()
{    
    // 用完 IDtoName (file output) // 因為紅黑樹改用名字去建，所以反而很常用IDtoName(才能用名字find)，所以不一直 free了
    IDtoNameBase_Clear();
    

    // blockBase_Data* current = storedBlock_ArrayRecord->head;
    // while (current != NULL)
    // {
    //     blockBase_Data* nextBlock = current->next;
    //     SDL_DestroyTexture(current->blockTexture);       
    //     free(current->blockName);
    //     free(current);
    //     current = nextBlock;
    // }
    
    // storedBlock_ArrayRecord->storedSize = 0;
    // storedBlock_ArrayRecord->maxSize = 0;
    // free(storedBlock_ArrayRecord->array);
    // free(storedBlock_ArrayRecord);

    // for(int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    // {
    //     SDL_Texture *nowBlockTexture = storedBlock_ArrayRecord->array[i].blockTexture;
    //     SDL_DestroyTexture(nowBlockTexture);
    // }    
}

// 依方塊名稱取得圖片 (似乎不用用到，我們都是用編號取，但應該很好用！)
// public SDL_Texture *TextureBase_GetTextureName(char* textureName) // unused
// {
//     for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
//     {
//         if (strcmp(storedBlock_ArrayRecord->array[i].blockName, textureName) == 0)
//             return storedBlock_ArrayRecord->array[i].blockTexture;
//     }
//     blockBase_Data* current = storedBlock_ArrayRecord->head;
//     while (current != NULL)
//     {
//         if (strcmp(current->blockName, textureName) == 0)
//             return current->blockTexture;
//         current = current->next;
//     }
//     return NULL;
// } 

// 依編號取得圖片
SDL_Texture *TextureBase_GetTextureUsingID(short blockID)
{
    // 用 IDtoName 換到名字，用名字find
    blockBase_Data tmp = {.blockName=IDtoNameBase[blockID]};
    struct tNode* ifexist = find(root, &tmp);

    // 沒找到
    if(ifexist == NULL)
    {
        static bool *isNotifiedID = NULL;
        if(isNotifiedID == NULL) 
            isNotifiedID = (bool *)calloc(IDtoNameBase_totalNameNumber, sizeof(bool));

        // 有提醒過就不提醒了
        if(isNotifiedID[blockID] == false)  
        {
            fprintf(stderr, STRING_LITERAL_IN_PRINTF_YELLOW("Missing %s (ID: %d) with no replacement.\n"), IDtoNameBase[blockID], blockID);
            isNotifiedID[blockID] = true;
        }
        
        return NULL;
    }
    // 找到，回傳Texture
    else
        return (*ifexist).blockData->blockTexture;

    // for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    // {
    //     if (storedBlock_ArrayRecord->array[i].blockID == blockID)
    //         return storedBlock_ArrayRecord->array[i].blockTexture;
    // }
    
    // blockBase_Data* current = storedBlock_ArrayRecord->head;
    // while (current != NULL)
    // {
    //     if (current->blockID == blockID)
    //         return current->blockTexture;
    //     current = current->next;
    // }
    
    // return NULL;
}

//刪除指定圖片 (似乎不用用到！但應該很好用！)
// public void TextureBase_DeleteTexture(const char* textureName) // unused
// {
//     // 搜尋欲刪除方塊名稱
//     int index = -1;
//     for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
//     {
//         if (strcmp(storedBlock_ArrayRecord->array[i].blockName, textureName) == 0)
//         {
//             index = i;
//             break;
//         }
//     }
//     if (index != -1)
//     {
//         // 將方塊從資料庫移除
//         SDL_DestroyTexture(storedBlock_ArrayRecord->array[index].blockTexture);
//         free(storedBlock_ArrayRecord->array[index].blockName);
    
//         for (int i = index; i < storedBlock_ArrayRecord->storedSize - 1; ++i)
//         {
//             storedBlock_ArrayRecord->array[i] = storedBlock_ArrayRecord->array[i + 1];
//         }

//         // 更新資料庫大小
//         storedBlock_ArrayRecord->storedSize--;
//     }
// }

// 匯入編號對應材質名字 file
public void IDtoNameBase_Init()
{
    // IDtoNameBase = (storedBlock_ArrayAndSize*)malloc(sizeof(storedBlock_ArrayAndSize));
    // IDtoNameBase->array = (blockBase_Data*)malloc(sizeof(blockBase_Data) * INIT_ARRAY_SIZE);
    // IDtoNameBase->storedSize = storedBlock_ArrayRecord->storedSize;
    // IDtoNameBase->head = NULL;
    
    // blockBase_Data* currentNode = NULL; // 要把array轉乘linked list? 可能要記住 舊node
    // for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    // {
    //     blockBase_Data* newData = &(storedBlock_ArrayRecord->array[i]);
    //     // array 儲存部分
    //     IDtoNameBase->array[i].blockID = newData->blockID;
    //     IDtoNameBase->array[i].blockName = strdup(newData->blockName);
    //     IDtoNameBase->array[i].blockTexture = newData->blockTexture;
    //     // 要把array轉乘linked list? 可能要創 新node，做以下增加
    //     blockBase_Data* newDataNode = (blockBase_Data *)malloc(sizeof(blockBase_Data)); 
    //     newDataNode->blockID = newData->blockID;
    //     newDataNode->blockName = strdup(newData->blockName);
    //     newDataNode->blockTexture = newData->blockTexture;
         
    //     if (IDtoNameBase->head == NULL)
    //     {
    //         IDtoNameBase->head = newDataNode;
    //         newDataNode->next = NULL;
    //         currentNode = newDataNode; // 要把array轉乘linked list? 可能要記住 舊node
    //     }
    //     else
    //     {
    //         newDataNode->next = currentNode->next;
    //         currentNode->next = newDataNode;
    //     }
    // }
    // 分記憶體(直接分滿，畢竟最多就那些方塊)
    SDL_size totalCellSize = Backpack_GetBlockNumberInWidthAndHeight();
    IDtoNameBase_totalNameNumber = totalCellSize.width * totalCellSize.height; // 記住總數！
    IDtoNameBase = (char **)malloc(sizeof(char *) * IDtoNameBase_totalNameNumber);

    // 打開 file
    FILE *IDtoNameRecodeFile;
    IDtoNameRecodeFile = fopen(IDtoNAME_RECORD_FILE_RELATIVE_PATH, "r"); 
    // 沒有 file，那就沒事，已初始化完
    if(IDtoNameRecodeFile==NULL) 
    {
        for(int i = 0; i < IDtoNameBase_totalNameNumber; ++i)
            IDtoNameBase[i] = IDtoNAME_WORD_NONE_EXISTS;
    }
    // 有 file 就寫入
    else
    {
        for(int i = 0; i < IDtoNameBase_totalNameNumber; ++i)
        {
            char tmpBuffer[MAX_ABBSOULTE_PATH_LENGTH];
            fscanf(IDtoNameRecodeFile, "%s", tmpBuffer);
            IDtoNameBase[i] = strdup(tmpBuffer); // 第 i 個的名字 get!
        }
    }
}

// 清除編號對應材質名字 file (要 file output)
public void IDtoNameBase_Clear()
{
    // 避免被亂呼叫
    if (IDtoNameBase == NULL)
        return ;

    // file output
    FILE *IDtoNameRecodeFile;
    IDtoNameRecodeFile = fopen(IDtoNAME_RECORD_FILE_RELATIVE_PATH, "w");
    // 寫入名字 (第幾個就是編號幾)
    for(int i = 0; i < IDtoNameBase_totalNameNumber; ++i)
        fprintf(IDtoNameRecodeFile, "%s\t", IDtoNameBase[i]);
    
    // free  (strdup的東西變lieral了不用free)
    free(IDtoNameBase);

    // if (IDtoNameBase != NULL)
    // {
    //     blockBase_Data* current = IDtoNameBase->head;
    //     while (current != NULL)
    //     {
    //         blockBase_Data* temp = current;
    //         current = current->next;
    //         free(temp->blockName);
    //         SDL_DestroyTexture(temp->blockTexture);
    //         free(temp);
    //     }         
    //     free(IDtoNameBase->array);
    //     free(IDtoNameBase);
    //     IDtoNameBase = NULL;
    // }      
    
}

// 依搜尋文字，找找看是否有方塊是此名字，回傳是否找到
public bool TextureBase_isFindBlockBySearchWords()
{
    // 有需要 + 不存在，才要匯入ID到方塊資料庫
    if(IDtoNameBase == NULL)
        IDtoNameBase_Init();

    // 取得搜尋文字
    char* textureName = SearchWords_GetSearchingWords();

    //find if exist or not
    blockBase_Data tmp = {.blockName=textureName};
    struct tNode* ifexist = find(root, &tmp);
    // printf("ok!");
    if(ifexist==NULL) return false;
    else{
        searchedBlockIndex = ifexist->blockData->blockIndexInArray;
        return true;
    }
} 

// 把搜尋到的方塊是材質資料庫的第幾個回傳
public int TextureBase_GetSearchedBlockIndex()
{
    // Return -1 if the block is not found
    if(searchedBlockIndex == EOF)
        return -1;
    // 回傳ID
    else
    {
        int retrunIndex = searchedBlockIndex;
        searchedBlockIndex = EOF; // 清空搜尋紀錄
        // 找是第幾格
        return retrunIndex;
    }
}


// 取得材質資料庫所有的材質ID (需有 ID的buffer 與 放方塊總數的buffer) (buffer沒用記得free，還要初始化 buffer為NULL)
public void TextureBase_GetAllID(short **IDbuffer, int *totalBlockNum)
{
    *totalBlockNum = RBT_data_array_element_number;

    if(*IDbuffer == NULL)
    {
        *IDbuffer = (short *)malloc(sizeof(short) * RBT_data_array_element_number); // 因為只傳 pointer 會錯 (推測編譯器是拒絕在不知道是 array 的情況下用[i] ?)，所以還是分記憶體
        for(int i = 0; i < RBT_data_array_element_number; ++i)
            (*IDbuffer)[i] = RBT_data_array[i]; 
    }   
}

//依照方塊編號大小排序資料庫 // (似乎不用用到，但應該很好用！)
void IDtoNameBase_SortBaseByID()
{
    // // Sort the array by blockID in ascending order using bubble sort
    // for (int i = 0; i < IDtoNameBase->storedSize - 1; ++i)
    // {
    //     for (int j = 0; j < IDtoNameBase->storedSize - i - 1; ++j)
    //     {
    //         if (IDtoNameBase->array[j].blockID > IDtoNameBase->array[j + 1].blockID)
    //         {
    //             // Swap the elements
    //             blockBase_Data temp = IDtoNameBase->array[j];
    //             IDtoNameBase->array[j] = IDtoNameBase->array[j + 1];
    //             IDtoNameBase->array[j + 1] = temp;
    //         }
    //     }
    // }
}

//RBTree
int node_count=0;

private struct tNode* create_tNode(blockBase_Data *blockData, struct tNode* Parent, SIDE side){
    //P;
    struct tNode* n=malloc(sizeof(struct tNode));
    n->color= RED, n->blockData=blockData, n->Lchild= NULL;
    n->Rchild= NULL, n->parent= Parent, n->side= side;
    return n;
}

struct tNode* RBT_init(){
    struct tNode *first=create_tNode(NULL, NULL, NONE);
    first->color=BLACK;
    root=first;
    return first;
}

void insert(struct tNode *curNode, blockBase_Data *blockData) {
    if(curNode->blockData != NULL && curNode->blockData->blockName != NULL){
        // DEBUG_PRINT(blockData->blockName, "%s");
        // DEBUG_PRINT(curNode->blockData->blockName, "%s.");
        if(strcmp(blockData->blockName, curNode->blockData->blockName)==0) return;
        else if(strcmp(blockData->blockName, curNode->blockData->blockName)<0){
            if(curNode->Lchild==NULL){
                curNode->Lchild= create_tNode(blockData, curNode, LEFT);
                node_count++;
                check(curNode->Lchild);
            }
            else insert(curNode->Lchild, blockData);
        }
        else{
            if(curNode->Rchild==NULL){
                curNode->Rchild= create_tNode(blockData, curNode, RIGHT);
                node_count++;
                check(curNode->Rchild);
            }
            else insert(curNode->Rchild, blockData);
        }
    }
    else{
        curNode->blockData=blockData;
        node_count++;
        return;
    }
}

struct tNode* find(struct tNode *curNode, blockBase_Data *blockData){
    if(curNode==NULL) return NULL;
    if(strcmp(blockData->blockName, curNode->blockData->blockName)==0) return curNode;
    else if(strcmp(blockData->blockName, curNode->blockData->blockName)>0) find(curNode->Rchild, blockData);
    else if(strcmp(blockData->blockName, curNode->blockData->blockName)<0) find(curNode->Lchild, blockData);
}

private void check(struct tNode *curNode){
    if(curNode->parent==NULL){
        curNode->color=BLACK, curNode->side=NONE;
        return;
    }
    if(curNode->parent->color==RED){
        if(curNode->parent->side==LEFT && curNode->parent->parent->Rchild!=NULL && curNode->parent->parent->Rchild->color==RED){
            curNode->parent->color=BLACK;
            curNode->parent->parent->color=RED;
            curNode->parent->parent->Rchild->color=BLACK;
            check(curNode->parent->parent);
        }
        else if(curNode->parent->side==RIGHT && curNode->parent->parent->Lchild!=NULL && curNode->parent->parent->Lchild->color==RED){
            curNode->parent->color=BLACK;
            curNode->parent->parent->color=RED;
            curNode->parent->parent->Lchild->color=BLACK;
            check(curNode->parent->parent);
        }
        else if(curNode->side==LEFT){
            if(curNode->parent->side==LEFT) right_rotate(curNode->parent->parent);
            else{
                curNode=curNode->parent;
                right_rotate(curNode);
                check(curNode);
            }
        }
        else{
            if(curNode->parent->side==RIGHT) left_rotate(curNode->parent->parent);
            else{
                curNode=curNode->parent;
                left_rotate(curNode);
                check(curNode);
            }
        }
    }

}

private void left_rotate(struct tNode *curNode){
    //swap color
    COLOR c=curNode->color;
    curNode->color=curNode->Rchild->color;
    curNode->Rchild->color=c;
    //change relation
    if(curNode->parent!=NULL){
        if(curNode->side==LEFT) curNode->parent->Lchild=curNode->Rchild;
        else curNode->parent->Rchild=curNode->Rchild;
    }
    //update side
    curNode->Rchild->side=curNode->side;
    curNode->side=LEFT;
    //change relation
    curNode->Rchild->parent=curNode->parent;
    curNode->parent=curNode->Rchild;
    curNode->Rchild=curNode->parent->Lchild;
    if(curNode->Rchild!=NULL){
        curNode->Rchild->parent=curNode;
        curNode->Rchild->side=RIGHT;
    }
    curNode->parent->Lchild=curNode;
}

private void right_rotate(struct tNode *curNode){
    //swap color
    COLOR c=curNode->color;
    curNode->color=curNode->Lchild->color;
    curNode->Lchild->color=c;
    //change relation
    if(curNode->parent!=NULL){
        if(curNode->side==LEFT) curNode->parent->Lchild=curNode->Lchild;
        else curNode->parent->Rchild=curNode->Lchild;
    }
    //update side
    curNode->Lchild->side=curNode->side;
    curNode->side=RIGHT;
    //change relation
    curNode->Lchild->parent=curNode->parent;
    curNode->parent=curNode->Lchild;
    curNode->Lchild=curNode->parent->Rchild;
    if(curNode->Lchild!=NULL){
        curNode->Lchild->parent=curNode;
        curNode->Lchild->side=LEFT;
    }
    curNode->parent->Rchild=curNode;
}

struct tNode* find_root(struct tNode *curNode){
    while(curNode->parent!=NULL) curNode=curNode->parent;
    return curNode;
}

struct tNode* return_root(){
    return root;
}

void store_data(struct tNode *curNode){
    int count=0;
    RBT_data_array=malloc((node_count+1)*sizeof(short));
    char max_print[50], now_print[49]={0,'\0'};
    struct tNode* M=find_root(curNode);
    curNode=M;
    while(M->Rchild!=NULL) M=M->Rchild;
    strcpy(max_print, M->blockData->blockName);
    while(strcmp(now_print, max_print)<0){
        if(curNode->Lchild!=NULL && strcmp(curNode->Lchild->blockData->blockName, now_print)>0) curNode=curNode->Lchild;
        else if(strcmp(curNode->blockData->blockName, now_print)>0){
            strcpy(now_print, curNode->blockData->blockName);
            *(RBT_data_array+count)=curNode->blockData->blockIndexInArray = count; // 紀錄在 array 的第幾個
            *(RBT_data_array+(count++))=curNode->blockData->blockID;
        }
        else if(curNode->Rchild!=NULL && strcmp(curNode->Rchild->blockData->blockName, now_print)>0) curNode=curNode->Rchild;
        else curNode=curNode->parent;
    }
    *(RBT_data_array+count)=-1;//stop
   RBT_data_array_element_number = count; // 記住元素總數
}


private void cp_itsRelation(struct tNode *curNode){//cp: check and print
    printf("%p\t", curNode->blockData);
    curNode->color==RED ? printf("RED\t") : printf("BLACK\t");
    curNode->Lchild ? printf("L:%p\t",curNode->Lchild->blockData) : printf("L:NULL\t");
    curNode->parent ? printf("p:%p\t",curNode->parent->blockData) : printf("p:NULL\t");
    curNode->Rchild ? printf("R:%p\n",curNode->Rchild->blockData) : printf("R:NULL\n");
}