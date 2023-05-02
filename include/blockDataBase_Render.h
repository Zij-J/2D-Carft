/* 要render的方塊的資料庫 */


// imcomplete type for Encapsulation
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


// prototype

void renderedBlock_InitArray();
void renderedBlock_ScanWindow(storedBlock_ArrayAndSize *storedBlock_ArrayRecord, placedBlock_ArrayAndSize *placedBlock_ArrayRecord);
void renderedBlock_RenderToRenderer(SDL_Renderer *renderer);


// 相機位置，用一個struct紀錄比較不會亂
struct cameraInformation
{
    int x;
    int y;
};
typedef struct cameraInformation renderedBlock_cameraPosition;


// 控制畫面左右的傳入值
enum cameraMovement {camera_MoveRight, camera_MoveLeft};
typedef enum cameraMovement renderedBlock_cameraMovement; //需要當function argument用，所以須typedef起來