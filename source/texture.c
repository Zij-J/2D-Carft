/* 材質(背包)資料庫(紅黑樹？)、Sort、Search 與 相關function (file I/O：編號對應材質名 file、圖片資料夾)*/
#include "../include/basicSetting.h" // 要用的
#include "../include/render.h" // 要用的
#include "../include/ui.h" // 要用的
#include "../include/texture.h" // 要放的
#include <string.h>
#include <dirent.h>
#include <io.h>

#define INIT_ARRAY_SIZE 100
#define MAX_ABBSOULTE_PATH_LENGTH 4096 // 這是 UNIX 絕對路徑最大值，Windows 只有 255
#define BLOCK_PICTURE_FOLDER_RELATIVE_PATH "./block_pictures"

struct blockDataBase_Texture
{
    short blockID;
    int blockIndexInArray;
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

// 創立 方塊(材質資料庫) 、 ID對應名字資料庫
storedBlock_DataBase storedBlock_ArrayRecord;
storedBlock_DataBase IDtoNameBase = NULL; // 要預設沒東西，表示有需要 + 不存在 才 init 用

// 記住 TextureBase_isFindBlockBySearchWords()，找到的ID
short searchedBlockIndex = EOF; // 應該不會用到 EOF，但還是寫一下，代表找到過任何Block
// 記住 RBtree 遍歷結果，就不用每次呼叫都要重新遍歷
short *RBT_data_array; int RBT_data_array_element_number = 0;

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

    int index = 0;  //方塊資料庫索引
    
    SDL_size backpackCellNum = Backpack_GetBlockNumberInWidthAndHeight(); // 取得背包格子總數，資料庫的方塊不能比他多
    int totalCellnum = backpackCellNum.width * backpackCellNum.height;

    while ((entry = readdir(dir)) != NULL && index < totalCellnum)
    {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Check if the file has a ".png" extension
        if (strstr(entry->d_name, ".png") == NULL)
            continue;

        // Construct the full file path
        snprintf(filePath, sizeof(filePath), "%s/%s", BLOCK_PICTURE_FOLDER_RELATIVE_PATH, entry->d_name); 

        // load Texture 應該是在這裡
        SDL_Texture *texture = loadTexture(filePath, renderer); // 可以開相對路徑！
        if (!texture)
            continue;

        // Extract the file name without extension
        char *fileName = entry->d_name;
        char *extension = strstr(fileName, ".png");
        if (extension != NULL)
            *extension = '\0';

        // Add the texture to the blockBase_Data
        storedBlock_ArrayRecord->array[index].blockName = duplicateString(fileName);
        //printf("block name: %s\n", storedBlock_ArrayRecord->array[index].blockName);//test
        storedBlock_ArrayRecord->array[index].blockID = index;//索引為方塊編號
        storedBlock_ArrayRecord->array[index].blockTexture = texture;
        storedBlock_ArrayRecord->array[index].next = NULL;

        //RBTree
        root=find_root(root);
        insert(root, &(storedBlock_ArrayRecord->array[index])); // 取位置運算的優先級較高，所以要括號！
        
        index++;

        if (index >= storedBlock_ArrayRecord->maxSize)
        {           
            storedBlock_ArrayRecord->maxSize *= 2;
            storedBlock_ArrayRecord->array = (blockBase_Data *)realloc(storedBlock_ArrayRecord->array, sizeof(blockBase_Data) * storedBlock_ArrayRecord->maxSize);
        }
    }
    storedBlock_ArrayRecord->storedSize = index;
    root=find_root(root);
    store_data(root); // 在此就先遍歷！

    //依照方塊名稱排序
    qsort(storedBlock_ArrayRecord->array, storedBlock_ArrayRecord->storedSize, sizeof(blockBase_Data), compareTextures);
    closedir(dir);
}

// 初始化資料庫
public void TextureBase_Init(SDL_Renderer *renderer)
{
    // 資料庫初始化
    storedBlock_ArrayRecord = (storedBlock_ArrayAndSize*)malloc(sizeof(storedBlock_ArrayAndSize));
    storedBlock_ArrayRecord->array = (blockBase_Data*)calloc(INIT_ARRAY_SIZE, sizeof(blockBase_Data)); // 這樣就有 (storedBlock_ArrayRecord->array[i]).blockName = NULL; 效果(i = 0 到 INIT_ARRAY_SIZE)
    storedBlock_ArrayRecord->storedSize = 0;
    storedBlock_ArrayRecord->maxSize = INIT_ARRAY_SIZE;
    storedBlock_ArrayRecord->head = NULL;

    //RBTree
    first=RBT_init();

    // 從圖片資料夾開啟方塊
    TextureBase_GetAllBlock(renderer);
}

// 清除資料庫
public void TextureBase_Clear()
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

// 依方塊名稱取得圖片 (似乎不用用到，我們都是用編號取，但應該很好用！)
public SDL_Texture *TextureBase_GetTextureName(char* textureName) // unused
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

// 依編號取得圖片
SDL_Texture *TextureBase_GetTextureUsingID(short blockID)
{
    for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    {
        if (storedBlock_ArrayRecord->array[i].blockID == blockID)
            return storedBlock_ArrayRecord->array[i].blockTexture;
    }
    
    blockBase_Data* current = storedBlock_ArrayRecord->head;
    while (current != NULL)
    {
        if (current->blockID == blockID)
            return current->blockTexture;
        current = current->next;
    }
    
    return NULL;
}

//刪除指定圖片 (似乎不用用到！但應該很好用！)
public void TextureBase_DeleteTexture(const char* textureName) // unused
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
public void IDtoNameBase_Init()
{
    IDtoNameBase = (storedBlock_ArrayAndSize*)malloc(sizeof(storedBlock_ArrayAndSize));
    IDtoNameBase->array = (blockBase_Data*)malloc(sizeof(blockBase_Data) * INIT_ARRAY_SIZE);
    IDtoNameBase->storedSize = storedBlock_ArrayRecord->storedSize;
    IDtoNameBase->head = NULL;
    
    blockBase_Data* currentNode = NULL; // 要把array轉乘linked list? 可能要記住 舊node
    for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    {
        blockBase_Data* newData = &(storedBlock_ArrayRecord->array[i]);
        // array 儲存部分
        IDtoNameBase->array[i].blockID = newData->blockID;
        IDtoNameBase->array[i].blockName = strdup(newData->blockName);
        IDtoNameBase->array[i].blockTexture = newData->blockTexture;
        // 要把array轉乘linked list? 可能要創 新node，做以下增加
        blockBase_Data* newDataNode = (blockBase_Data *)malloc(sizeof(blockBase_Data)); 
        newDataNode->blockID = newData->blockID;
        newDataNode->blockName = strdup(newData->blockName);
        newDataNode->blockTexture = newData->blockTexture;
         
        if (IDtoNameBase->head == NULL)
        {
            IDtoNameBase->head = newDataNode;
            newDataNode->next = NULL;
            currentNode = newDataNode; // 要把array轉乘linked list? 可能要記住 舊node
        }
        else
        {
            newDataNode->next = currentNode->next;
            currentNode->next = newDataNode;
        }
    }
}

// 清除編號對應材質名字 file
public void IDtoNameBase_Clear()
{
    if (IDtoNameBase != NULL)
    {
        blockBase_Data* current = IDtoNameBase->head;
        while (current != NULL)
        {
            blockBase_Data* temp = current;
            current = current->next;
            free(temp->blockName);
            SDL_DestroyTexture(temp->blockTexture);
            free(temp);
        }         
        free(IDtoNameBase->array);
        free(IDtoNameBase);
        IDtoNameBase = NULL;
    }      
    
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
        // IDtoNameBase_Clear();
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
    // Sort the array by blockID in ascending order using bubble sort
    for (int i = 0; i < IDtoNameBase->storedSize - 1; ++i)
    {
        for (int j = 0; j < IDtoNameBase->storedSize - i - 1; ++j)
        {
            if (IDtoNameBase->array[j].blockID > IDtoNameBase->array[j + 1].blockID)
            {
                // Swap the elements
                blockBase_Data temp = IDtoNameBase->array[j];
                IDtoNameBase->array[j] = IDtoNameBase->array[j + 1];
                IDtoNameBase->array[j + 1] = temp;
            }
        }
    }
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