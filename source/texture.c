/* 材質(背包)資料庫(紅黑樹？)、Sort、Search 與 相關function (file I/O：編號對應材質名 file、圖片資料夾)*/
#include "../include/basicSetting.h" // 要用的
#include "../include/render.h" // 要用的
#include "../include/texture.h" // 要放的
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <io.h>

#define INIT_ARRAY_SIZE 100
#define MAX_ABBSOULTE_PATH_LENGTH 255 // 4096 這是 UNIX 絕對路徑最大值，Windows 只有 255

// 創立 方塊(材質資料庫) 、 ID對應名字資料庫
storedBlock_DataBase storedBlock_ArrayRecord;
storedBlock_DataBase IDtoNameBase = NULL; // 要預設沒東西，表示有需要 + 不存在 才 init 用

// 記住 TextureBase_isFindBlockBySearchWords()，找到的ID
short searchedBlockID = EOF; // 應該不會用到 EOF，但還是寫一下，代表找到過任何Block

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
}*root;

struct tNode* RBT_init();//initiallize
void insert(struct tNode *, blockBase_Data*);//insert the node to red black tree
struct tNode* find(struct tNode*, blockBase_Data*);//find node by block's name
struct tNode* find_root(struct tNode *);//find red black tree root
int* store_data(struct tNode *);//store blockID to array
private struct tNode* create_tNode(blockBase_Data*, struct tNode*, SIDE);
private void check(struct tNode*);
private void left_rotate(struct tNode*);
private void right_rotate(struct tNode*);

// 取得圖片資料夾路徑
private char *getPictureFolderPath()
{
    char buffer[MAX_ABBSOULTE_PATH_LENGTH];
    getcwd(buffer, sizeof(buffer)); // 取得目前工作目錄
    strcat(buffer, "/block_pictures/"); // 接到 block_pictures 資料夾裡，就完成
    return strdup(buffer);
}

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

//從圖片資料夾匯入圖片並依名稱排序
private void TextureBase_GetAllBlock(const char *folderPath, SDL_Renderer *renderer)
{
    DIR *dir;
    struct dirent *entry;
    char filePath[MAX_ABBSOULTE_PATH_LENGTH];

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

        // load Texture 應該是在這裡？
        SDL_Texture *texture = loadTexture(filePath, renderer);
        if (!texture)
            continue;

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

        //RBTree
        insert(root, &storedBlock_ArrayRecord->array[index]);
        
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

// 初始化資料庫
public void TextureBase_Init(SDL_Renderer *renderer)
{
    // 資料庫初始化
    storedBlock_ArrayRecord = (storedBlock_ArrayAndSize*)malloc(sizeof(storedBlock_ArrayAndSize));
    storedBlock_ArrayRecord->array = (blockBase_Data*)calloc(INIT_ARRAY_SIZE, sizeof(blockBase_Data)); // 這樣就有 (storedBlock_ArrayRecord->array[i]).blockName = NULL; 效果(i = 0 到 INIT_ARRAY_SIZE)
    storedBlock_ArrayRecord->storedSize = 0;
    storedBlock_ArrayRecord->maxSize = INIT_ARRAY_SIZE;
    storedBlock_ArrayRecord->head = NULL;

    // 取得資料夾路徑
    char *blockFolderPath = getPictureFolderPath();

    //RBTree
    root=RBT_init();

    // 從圖片資料夾開啟方塊
    TextureBase_GetAllBlock(blockFolderPath, renderer);
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
SDL_Texture* TextureBase_GetTextureByID(storedBlock_DataBase storedBlock_ArrayRecord, int blockID)
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

    /*
    // 比較
    for (int i = 0; i < IDtoNameBase->storedSize; ++i)
    {
        if (strcmp(IDtoNameBase->array[i].blockName, textureName) == 0)
        {
            searchedBlockID = IDtoNameBase->array[i].blockID; // 在這邊記住 ID 就不用再找一次！
            return true;
        }
            
    }
    blockBase_Data* current = IDtoNameBase->head;
    while (current != NULL)
    {
        if (strcmp(current->blockName, textureName) == 0)
            return true;
        current = current->next;
    }
    return false;
    */
    blockBase_Data tmp = {.blockName=textureName};
    struct tNode* ifexist = find(root, &tmp);
    if(ifexist==NULL) return false;
    else{
        searchedBlockID = ifexist->blockData->blockID;
        return true;
    }
} 

// 把搜尋到的方塊編號回傳
public short TextureBase_GetSearchedBlockID()
{
    // Return -1 if the block is not found
    if(searchedBlockID == EOF)
        return -1;
    // 回傳ID
    else
    {
        short retrunID = searchedBlockID;
        searchedBlockID = EOF;
        IDtoNameBase_Clear();
        return retrunID;
    }
}

// 取得材質資料庫所有的材質ID 
public short *TextureBase_GetAllID()
{
    short* materialID = (short*)malloc(sizeof(short) * storedBlock_ArrayRecord->storedSize);
    for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    {
        materialID[i] = storedBlock_ArrayRecord->array[i].blockID;
    }
    return materialID;
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
    struct tNode *root=create_tNode(NULL, NULL, NONE);
    root->color=BLACK;
    return root;
}

void insert(struct tNode *curNode, blockBase_Data *blockData) {
    if(curNode->blockData != NULL && curNode->blockData->blockName != NULL){
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

int* store_data(struct tNode *curNode){
    int count=0, *block_ID=malloc((node_count+1)*sizeof(int));
    char max_print[50], now_print[49]={0,'\0'};
    struct tNode* M=find_root(curNode);
    curNode=M;
    while(M->Rchild!=NULL) M=M->Rchild;
    strcpy(max_print, M->blockData->blockName);
    while(strcmp(now_print, max_print)<0){
        if(curNode->Lchild!=NULL && strcmp(curNode->Lchild->blockData->blockName, now_print)>0) curNode=curNode->Lchild;
        else if(strcmp(curNode->blockData->blockName, now_print)>0){
            strcpy(now_print, curNode->blockData->blockName);
            *(block_ID+(count++))=curNode->blockData->blockID;
        }
        else if(curNode->Rchild!=NULL && strcmp(curNode->Rchild->blockData->blockName, now_print)>0) curNode=curNode->Rchild;
        else curNode=curNode->parent;
    }
    *(block_ID+count)=-1;//stop
    return block_ID;
}
