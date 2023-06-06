/* 相機、與 renderer 相關 function */
#include <dirent.h> // 取得所有 subfile (.ttf) 時用
#include <unistd.h> // 取得目前的工作 path (才能知道 subfile) 用
#include "../SDL2/SDL_ttf.h" // 此 source file 需要特別使用 ttf 擴充套件
#include "../include/basicSetting.h" // 要用的
#include "../include/SDL_StartAndEnd.h" // 要用的
#include "../include/texture.h" // 要用的
#include "../include/map.h" // 要用的
#include "../include/ui.h" // 要用的
#include "../include/render.h" // 要放的

#define DEFULT_ARRAY_SIZE 2 // 預設可擴充 Array 大小，設為2
#define FONT_DEFULT_PIXEL_QUALITY 72 // font 開啟時文字預設大小，會依此去縮放，所以愈大愈好
#define MAX_ABBSOULTE_PATH_LENGTH 4096 // 這是 UNIX 絕對路徑最大值，Windows 只有 255
#define CAMERA_MOVE_SPEED WINDOW_WIDTH/50 // 相機移動速度 
enum searchWord_colorData {searchWord_red = 255, searchWord_blue = 255, searchWord_green = 255}; // 大 

#include <windows.h>
// 記住的 renderer、文字 Font
SDL_Renderer *renderer;
TTF_Font *font;

// 相機位置
SDL_position cameraPosition = (SDL_position){.x = 0, .y = 0}; 

// 搜尋文字 Array 建立 (存字串 for 比對，存 SDL_Texture for 快速顯示，不用每次顯示都要 load Texture)
struct wordsArray_tmpStructName
{
    char *words;
    SDL_Texture **wordsTexture;
    int nowStored, nowMax;
};
typedef struct wordsArray_tmpStructName wordsArray;
wordsArray *searchWords;


private void Render_RenderBlockInBackpack(SDL_position nowPos, int blockID);
private TTF_Font *SearchWords_openFont();
private void SearchWords_chaningPressingShift(char *inputChar, SDL_Event event);


// 讓 Render 記住 renderer，之後就不用傳入 renderer
public void Render_RememberRenderer(SDL_Renderer* rememberedRenderer)
{
    renderer = rememberedRenderer;
}

// 畫背包到 renderer
public void Render_RenderBackpack()
{
    // 取得背包位置、大小
    // SDL_position backpackPos = Backpack_GetPosition();
    // SDL_size backpackSize = Backpack_GetSize();

    // 還要把方塊畫上去
    // int *textureIDs = TextureBase_GetAllBlock();
    // SDL_position startPos = Backpack_GetBlockStartPosition();

    //Render_RenderBlockInBackpack();
}

// 畫背包上的方塊到 renderer
private void Render_RenderBlockInBackpack(SDL_position nowPos, int blockID)
{
    // 取得材質 by 編號
    // SDL_Texture *nowTexture = TetxureBase_GetTexture(blockID);
    // SDL_size blockSize = Backpack_GetBlockSize();
}

// 畫背包的 cursor 到 renderer
public void Render_RenderBackpackCursor()
{
    // 取得 cursor 位置、大小
    // SDL_position cursorPos = Backpack_GetCursorPosition();
    // SDL_size cursorSize = Backpack_GetCursorSize();
}

// 依 camera 差距，畫出地圖
public void Render_RenderMap()
{
    // 取得 Map 要顯示大陣列全部(因為要一直分記憶體很麻煩，所以用static，並初始化以判斷是否要分記憶體)、左上角位置、方塊大小、長寬方塊數量
    static short **renderingMap = NULL; SDL_size totalBlockNumInWindow ; Map_GetShowedMapData(&renderingMap, &totalBlockNumInWindow);
    SDL_size blockSize = Map_GetBlockSize();
    SDL_position startBlockPos = Map_GetUpLeftCornerPosition();
    startBlockPos.x *= blockSize.width; startBlockPos.y *= blockSize.height; // 取得的位置是「方塊數量位置」不是真正座標，要換成真正座標 (for renderCopy)

    // get 目前 block 的材質
    // SDL_Texture *blockTexture = TetxureBase_GetTexture();
    SDL_Texture *blockTexture = IMG_LoadTexture(renderer, "block_pictures/Grass_Block.png");

    // 顯示
    SDL_position nowBlockPos = startBlockPos;
    for(int y = 0; y < totalBlockNumInWindow.height; ++y)
    {
        nowBlockPos.x = startBlockPos.x;
        for(int x = 0; x < totalBlockNumInWindow.width; ++x)
        {
            if(renderingMap[y][x] != NO_BLOCK_ID)
            {
                // 算座標 (因實際顯示往下才是 +y ，所以要算完相對 y 座標後加負號)、顯示
                SDL_Rect rect = (SDL_Rect) {.x = nowBlockPos.x - cameraPosition.x, .y = -(nowBlockPos.y - cameraPosition.y), .w = blockSize.width, .h = blockSize.height};
                SDL_RenderCopy(renderer, blockTexture, NULL, &rect);
            }
            nowBlockPos.x += blockSize.width;
        }
        nowBlockPos.y -= blockSize.height;     
    }
}

// 依 camera 差距，畫出背包的 cursor
public void Render_RenderMapCursor()
{
    // 取得 cursor 位置、大小
    // SDL_position cursorPos = Map_GetCursorPosition();
    // SDL_size cursorize = Map_GetCursorSize();

    // 要算出 camera 相對位置
}

// 畫出快捷欄
public void Render_RenderHotbar()
{
    // 取得快捷欄位置、大小
    // SDL_position hotbarPos = Map_GetPosition();
    // SDL_size hotbarize = Map_GetSize();
}

// 畫出快捷欄的 cursor
public void Render_RenderHotbarCursor()
{
    // 取得 cursor 位置、大小
    // SDL_position cursorPos = Map_GetCursorPosition();
    // SDL_size cursorize = Map_GetCursorSize();
}

// 畫出已打的字
public void Render_RenderSearchWords()
{
    // 要取得打字處的開頭、字的大小
    // SDL_position wordStartPos = Backpack_GetSearchWordPosition();
    // SDL_size wordize = Backpack_GetSearchWordSize();

    SDL_position wordStartPos = (SDL_position){.x = 50, .y = 60};
    SDL_size wordSize = (SDL_size){.width = 25, .height = 50};

    // 
    int totalWidthNow = 0;
    for(int i = 0; (*searchWords).words[i] != '\0'; ++i)
    {
        // 取得字大小
        int originalwordWidth, originalWordHeight;
        char nowCharToString[2]; sprintf(nowCharToString, "%c\0", (*searchWords).words[i]); // 怕只能處理 string，所以要轉成 string
        TTF_SizeText(font, nowCharToString, &originalwordWidth, &originalWordHeight); // 取得 font 原本大小
        float widthScaling = (float)wordSize.height / (float)originalWordHeight; // 換成 float ，處理縮放
        wordSize.width = originalwordWidth * widthScaling; // height 就是用給的，但 width 需要依 height 去伸縮 (每個字 width 不同)

        // 大小，位置確定
        SDL_Rect wordRect;
        wordRect.x = wordStartPos.x + totalWidthNow;  // 左上座標的 x
        wordRect.y = wordStartPos.y;
        wordRect.w = wordSize.width;
        wordRect.h = wordSize.height;
        totalWidthNow += wordSize.width; // 下一次要加上此字的寬度

        // 顯示此字
        SDL_RenderCopy(renderer, (*searchWords).wordsTexture[i], NULL, &wordRect);
    }
}

// 初始化，搜尋文字 Array
public void SearchWords_Init()
{
    // 分配記憶體、初始化
    searchWords = (wordsArray *)malloc(sizeof(wordsArray));
    *searchWords = (wordsArray){.words = (char *)malloc(sizeof(char) * DEFULT_ARRAY_SIZE)
                                ,.wordsTexture = (SDL_Texture **)malloc(sizeof(SDL_Texture *) * DEFULT_ARRAY_SIZE)
                                ,.nowMax = DEFULT_ARRAY_SIZE, .nowStored = 0};
    // 是字串，所以預設要有 \0
    (*searchWords).words[0] = '\0';
    ++(*searchWords).nowStored; // 所以 SDL_Texture 會永遠空一個

    // 顯示文字的函式庫，初始化
    if(TTF_Init() < 0 )
    {
        fprintf(stderr, "Failed to open SDL_TTF: %s\n", TTF_GetError());
        SDL_EndAll_StopProgram();
    }
    // font 要先記住，之後在結束時才能 Close，記住也就不用每次開啟，方便
    font = SearchWords_openFont();
    if(font == NULL)
    {
        fprintf(stderr, "Failed to open TTF file: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_EndAll_StopProgram();
    }
}

// 開啟 Font ，失敗都回傳 NULL，成功就回傳 TTF_OpenFont get的東西
private TTF_Font *SearchWords_openFont()
{
    // 開 file path 名字 buffer、建立到 font 資料夾的絕對路徑 
    char path[MAX_ABBSOULTE_PATH_LENGTH]; // 這個一下就結束了，不太需要優化，所以 buffer 直接開超大就好
    getcwd(path, sizeof(path)); // 取得目前工作目錄
    strcat(path, "/font/"); // 接到 font 資料夾裡

    // 打開 dir
    DIR *dir;
    if((dir = opendir(path)) == NULL)
    {
        fprintf(stderr, "Failed to open folder: %s\n", path);
        return NULL;
    }

    // 讀 dir 內的所有 subfile
    struct dirent *fileData;
    while((fileData = readdir(dir)) != NULL) // 會用字母順序排，所以如果有多個 .tff，最接近A的會被開啟
    {
        // 取的最後四個字
        int fileName_lastFourWordStartIndex = strlen((*fileData).d_name) -1 -3; // -1 換 0-base，因不含\0，會到末index，-3是到前3 index，也就是前4個字
        char fileName_lastFourWords[4 +1];
        for(int i = 0; i < 4; ++i)
        {
            fileName_lastFourWords[i] = (*fileData).d_name[fileName_lastFourWordStartIndex +i];
        }
        fileName_lastFourWords[4] = '\0';

        // 是否最後四個字(副檔名) 是 ".ttf"
        if(strcmp(fileName_lastFourWords, ".ttf") == 0)
        {
            strcat(path, (*fileData).d_name); // 接到 目前路徑
            return TTF_OpenFont(path, FONT_DEFULT_PIXEL_QUALITY); // 打開 + 回傳找到的 + 結束 loop 
        }
    }
    // 到此，代表沒找到 
    fprintf(stderr, "Can't find any .ttf file in %s\n", path);
    return NULL;
}

// 清除，搜尋文字 Array
public void SearchWords_Clear()
{
    // free 掉 Texture
    for(int i = 0; i < (*searchWords).nowStored; ++i)
        SDL_DestroyTexture((*searchWords).wordsTexture[i]);

    // free 記憶體
    free((*searchWords).words);
    free((*searchWords).wordsTexture);
    free(searchWords);

    // 顯示文字的函式庫，結束
    TTF_CloseFont(font); // 要先關閉 font 再關 TTF
    TTF_Quit();
}

// 取得正在打的字 (不支援按 shift 才能打的文字、中文，支援大小寫)
public void SearchWords_GetInputWord(SDL_Event event)
{   
    // 讀鍵盤
    if(event.type == SDL_KEYDOWN)
    {
        // Backspace，要刪除字
        if(event.key.keysym.sym == SDLK_BACKSPACE)
        {
            // 取得刪除 index
            int nowStoredIndex = (*searchWords).nowStored -1 -1; // -1到 0-base，是\0，再 -1 才到 \0 後面一個 char
            // 刪文字、Texture
            (*searchWords).words[nowStoredIndex] = '\0'; // 把 \0 前移，就是刪除字了
            SDL_DestroyTexture((*searchWords).wordsTexture[nowStoredIndex]);
            // --已存東西
            --(*searchWords).nowStored;
        }
        // 其他，是要新增的字
        else
        {
            // 不能印，是控制字元，不用存
            if(!SDL_isprint(event.key.keysym.sym))
                return ;
            
            // 如果輸入到 e，會開關到背包，需要再開關一次、調回來 (所以在輸入 search 文字時開關背包會失效，要 cursor 移出 search 文字才行)
            if(event.key.keysym.sym == SDLK_e)
                Backpack_Switch(event);

            // 把輸入轉成 char + 小寫轉大寫
            char inputChar = (char)event.key.keysym.sym;
            if('a' <= inputChar && inputChar <= 'z')
            {
                if(event.key.keysym.mod == KMOD_CAPS || (event.key.keysym.mod == KMOD_LSHIFT || event.key.keysym.mod == KMOD_RSHIFT)) // 偵測這是否在 CapsLock 或 按著 shift 時打的
                    inputChar = inputChar - 'a' + 'A';
            }
            else
                 SearchWords_chaningPressingShift(&inputChar, event); // 按 shift 的變動 
            

            // 取得要存入的 index
            ++(*searchWords).nowStored;            
            int nowStoredIndex = (*searchWords).nowStored -1 -1; // -1到 0-base，是\0後面，再 -1 才到 \0
            // 存入(字)
            (*searchWords).words[nowStoredIndex] = inputChar;
            (*searchWords).words[nowStoredIndex +1] = '\0'; // 記得補回 \0
            // 存入(Texture)
            char InputtedcharToString[2]; sprintf(InputtedcharToString, "%c\0", inputChar); // 怕只能處理 string，所以要轉成 string
            SDL_Color wordColor = {searchWord_red, searchWord_blue, searchWord_green};
            SDL_Surface* wordsSerface = TTF_RenderText_Solid(font, InputtedcharToString, wordColor); // 只能畫在 serface
            SDL_Texture* wordsTexture = SDL_CreateTextureFromSurface(renderer, wordsSerface); // serface 轉 texture
            SDL_FreeSurface(wordsSerface); // serface 沒用了
            (*searchWords).wordsTexture[nowStoredIndex] = wordsTexture; 

            // searchWords 空間滿了，擴充它！
            if((*searchWords).nowStored == (*searchWords).nowMax)
            {
                (*searchWords).nowMax *= 2;
                (*searchWords).words = (char *)realloc((*searchWords).words, sizeof(char) * (*searchWords).nowMax);
                (*searchWords).wordsTexture = (SDL_Texture **)realloc((*searchWords).wordsTexture, sizeof(SDL_Texture *) * (*searchWords).nowMax);
            }
        }
    }
}

// 按住 shift 要變換字，需手動寫
private void SearchWords_chaningPressingShift(char *inputChar, SDL_Event event)
{ 
    if(event.key.keysym.mod != KMOD_LSHIFT && event.key.keysym.mod != KMOD_RSHIFT) // 要按 shift 才要改
        return;

    switch (event.key.keysym.sym)
    {
        case SDLK_BACKQUOTE:
            (*inputChar) = '~';
            break;
        case SDLK_1:
            (*inputChar) = '!';
            break;
        case SDLK_2:
            (*inputChar) = '@';
            break;
        case SDLK_3:
            (*inputChar) = '#';
            break;
        case SDLK_4:
            (*inputChar) = '$';
            break;
        case SDLK_5:
            (*inputChar) = '%';
            break;
        case SDLK_6:
            (*inputChar) = '^';
            break;
        case SDLK_7:
            (*inputChar) = '&';
            break;
        case SDLK_8:
            (*inputChar) = '*';
            break;
        case SDLK_9:
            (*inputChar) = '(';
            break;
        case SDLK_0:
            (*inputChar) = ')';
            break;
        case SDLK_MINUS:
            (*inputChar) = '_';
            break;
        case SDLK_EQUALS:
            (*inputChar) = '+';
            break;
        case SDLK_LEFTBRACKET:
            (*inputChar) = '{';
            break;
        case SDLK_RIGHTBRACKET:
            (*inputChar) = '}';
            break;
        case SDLK_BACKSLASH:
            (*inputChar) = '|';
            break;
        case SDLK_SEMICOLON:
            (*inputChar) = ':';
            break;
        case SDLK_QUOTE:
            (*inputChar) = '"';
            break;
        case SDLK_COMMA:
            (*inputChar) = '<';
            break;
        case SDLK_PERIOD:
            (*inputChar) = '>';
            break;
        case SDLK_SLASH:
            (*inputChar) = '?';
            break;
    }
}

// 取得 搜尋文字
public char *SearchWords_GetSearchingWords()
{
    return (*searchWords).words;
}

// 畫出 搜尋成功 or 失敗 的文字
public void Render_RenderSearchMessage(bool isSuccessSearching)
{
    // 成功文字
    if(isSuccessSearching)
    {

    }
    // 失敗文字
    else
    {
        
    }
}

// 移動相機
public void Render_MoveCamera(SDL_Event event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_w:
                cameraPosition.y += CAMERA_MOVE_SPEED;
                break;
            case SDLK_s:
                cameraPosition.y -= CAMERA_MOVE_SPEED;
                break;
            case SDLK_d:
                cameraPosition.x += CAMERA_MOVE_SPEED;
                break;
            case SDLK_a:
                cameraPosition.x -= CAMERA_MOVE_SPEED;
                break;
        }
    }
}

// 取得相機絕對位置
public SDL_position Render_GetCameraPosition()
{
    return cameraPosition;
}