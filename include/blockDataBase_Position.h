/* 方塊位置資料庫 */

// placedBlcokData 建立！(方塊位置資料庫：(x, y)座標 in 2-D array, 方塊種類(名字) char *) // 用 implete type!
typedef struct placedBlockArray *placedBlock_DataBase;


// prototype

void placedBlock_InitArray(placedBlock_DataBase placedBlock_ArrayRecord);
void placedBlock_AddBlock(placedBlock_DataBase placedBlock_ArrayRecord, char *blockName, int x, int y);
void placedBlock_ClearDataBase(placedBlock_DataBase placedBlock_ArrayRecord);