/* 方塊(材質)資料庫 */

// storedBlcokData 建立！(Encapsulation時間 by incomplete type)
struct storedBlockArrayStruct;
// 方塊位置資料庫：方塊種類(名字) char *, 真的材質 SDL_Texture * (材質都是用 pointer 運作) //用pointer運作all，client就不用記這是pointer，還能達到pointer效果(ex: overload少)
typedef struct storedBlockArrayStruct *storedBlock_DataBase;



// prototype

void storedBlock_InitArray(storedBlock_DataBase storedBlock_ArrayRecord);
void storedBlock_AddTexture(storedBlock_DataBase storedBlock_ArrayRecord, char *blockName, SDL_Window *window, SDL_Renderer *renderer);
SDL_Texture *storedBlock_GetTexture(storedBlock_DataBase storedBlock_ArrayRecord, char *textureName);
void storedBlock_ClearDataBase(storedBlock_DataBase storedBlock_ArrayRecord);


