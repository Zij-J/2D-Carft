/* 要render的方塊的資料庫 */




// prototype

void renderedBlock_InitArray();
void renderedBlock_ScanWindow(storedBlock_DataBase storedBlock_ArrayRecord, placedBlock_ArrayAndSize *placedBlock_ArrayRecord);
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