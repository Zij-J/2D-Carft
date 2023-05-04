/* 方塊位置資料庫 */
#include "../include/basicSetting.h" //要用的
#include "../include/blockDataBase_Position.h" //要放的

// placedBlcokData 建立！(方塊位置資料庫：(x, y)座標 in 2-D array, 方塊種類(名字) char *) // 1-D array是指向 (char *) 的 pointer，2-D要指向此，所以是***
struct placedBlockArray
{
    char ***textureArray; 
};
typedef struct placedBlockArray placedBlock_DataStruct;



// 初始化資料庫
public void placedBlock_InitArray(placedBlock_DataBase placedBlock_ArrayRecord)
{
    // 計算初始 array 的長寬
    int initialRowNumber = TOTAL_BLOCK_NUMBER_IN_WIDTH * 3; //把畫面左右兩邊的也先載好才行
    int initialColumnNumber = (WINDOW_HEIGHT / BLOCK_WIDTH) * 3; //須自己算高度要多少方塊高

    // 分配 2-D array size!
    placedBlock_ArrayRecord = (placedBlock_DataStruct *)malloc(sizeof(placedBlock_DataStruct)); //需要先分配 struct 位置，才能再修正！
    *placedBlock_ArrayRecord = (placedBlock_DataStruct) {.textureArray = (char ***)malloc( sizeof(char **) * initialColumnNumber )}; //第一層，取 column
    for(int i = 0; i < initialColumnNumber; ++i)
        (*placedBlock_ArrayRecord).textureArray[i] = (char **)calloc(initialRowNumber, sizeof(char *)); //第二層，每個 column 都有自己的 row //同時初始化為NULL (calloc預設清空為0)
}

// 加入(放上)方塊
public void placedBlock_AddBlock(placedBlock_DataBase placedBlock_ArrayRecord, char *blockName, int x, int y)
{
    /*
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
    */

   // 把座標換成第幾個
   int rowIndex = x / BLOCK_WIDTH;
   int columnIndex = y / BLOCK_WIDTH;

    // 紀錄！
   (*placedBlock_ArrayRecord).textureArray[columnIndex][rowIndex] = strdup(blockName); //先取cloumn再取row (第幾列的第幾行)
}

// 清除資料庫！
public void placedBlock_ClearDataBase(placedBlock_DataBase placedBlock_ArrayRecord)
{
    // 再計算初始 array 的長寬 (不想花記憶體存)
    int initialRowNumber = TOTAL_BLOCK_NUMBER_IN_WIDTH * 3; //把畫面左右兩邊的也先載好才行
    int initialColumnNumber = (WINDOW_HEIGHT / BLOCK_WIDTH) * 3; //須自己算高度要多少方塊高

    // 清理！
    for(int columnIndex = 0; columnIndex < initialColumnNumber; ++columnIndex)
    {
        for(int rowIndex = 0; rowIndex < initialRowNumber; ++rowIndex)
        {
            if((*placedBlock_ArrayRecord).textureArray[columnIndex][rowIndex] != NULL) // 不知 free NULL 會部會需要號很多，所以先刪除去除此情形
                free((*placedBlock_ArrayRecord).textureArray[columnIndex][rowIndex]); 
        }

        free((*placedBlock_ArrayRecord).textureArray[columnIndex]); //free column部分   
    }
    free(placedBlock_ArrayRecord); //把整個 struct free!
}