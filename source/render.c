/* 相機、與 renderer 相關 function */
#include <dirent.h>                     // 取得所有 subfile (.ttf) 時用
#include "../SDL2/SDL_ttf.h"            // 此 source file 需要特別使用 ttf 擴充套件
#include "../include/basicSetting.h"    // 要用的
#include "../include/SDL_StartAndEnd.h" // 要用的
#include "../include/texture.h"         // 要用的
#include "../include/map.h"             // 要用的
#include "../include/ui.h"              // 要用的
#include "../include/render.h"          // 要放的

#define ASSETS_FOLDER_RELATIVE_PATH "./assets"
#define DEFULT_ARRAY_SIZE 2            // 預設可擴充 Array 大小，設為2
#define FONT_DEFULT_PIXEL_QUALITY 72   // font 開啟時文字預設大小，會依此去縮放，所以愈大愈好
#define MAX_ABBSOULTE_PATH_LENGTH 4096 // 這是 UNIX 絕對路徑最大值，Windows 只有 255
#define MAX_ALLOW_NOT_CALLING_MOVING_DELTATIME 20   // 如果超過 X 毫秒沒有呼叫過移動 camera 函式(理論上每個 while 都要呼叫)，就推測是 bug
#define MAX_CAMERA_MOVE_SPEED 2       // 相機最大移動速度 (每一秒移動多少像素)
#define CAMERA_MOVE_ACCLERATION 5     // 相機移動加速度 (每一毫秒加速度)
#define BACKGROUND_SLOWER_TIMES 10 // 背景比相機慢幾倍
#define SEARCH_NOTIFY_DISAPPEAR_MILISECOND 1000 // 提醒文字幾豪秒會不見
SDL_Renderer *renderer;

// 記住的文字 Font、成功失敗文字、暫停文字、背景、地圖 cursor、快捷欄cursor、快捷欄
TTF_Font *font;
SDL_Texture *searchNotify_Texture[2]; // read [] and () before *，所以是放2個pointer的array
SDL_Texture *pauseWord_texture;
SDL_Texture *background_texture;
SDL_Texture *mapCurosr_texture;
SDL_Texture *hotbarCursor_texture;
SDL_Texture *hotbar_texture;
SDL_Texture *backpackCursor_texture;
SDL_Texture *backpack_texture;

// 相機、背景位置
SDL_position cameraPosition = (SDL_position){.x = 0, .y = 0};
SDL_position background_position = (SDL_position){.x = 0, .y = 0};

// 紀錄，搜尋是否成功、按Enter的時間點
bool isSuccessSearching = false;
int searchStartTime = -SEARCH_NOTIFY_DISAPPEAR_MILISECOND; // 用負的時間，這樣即使從 0 開始就按，也會大於時間差而影藏

// 搜尋文字 Array 建立 (存字串 for 比對，存 SDL_Texture for 快速顯示，不用每次顯示都要 load Texture)
struct wordsArray_tmpStructName
{
    char *words;
    SDL_Texture **wordsTexture;
    int nowStored, nowMax;
};
typedef struct wordsArray_tmpStructName wordsArray;
wordsArray *searchWords;

// 相機速度紀錄 (預設速度 0,0)
struct cameraVelocityRecord
{
    int x;
    int y;
};
struct cameraVelocityRecord cameraVelocityRecord = (struct cameraVelocityRecord){.x = 0, .y = 0};


private char *GetAssetsInFolder(const char *folderName, const char *extensionName);
private void Render_RenderWords(char *words, SDL_position leftUpPos, SDL_size charSize);
private void Render_RenderBlockInBackpack(SDL_position nowPos, short blockID);
private void SearchWords_Init();
private void SearchWords_chaningPressingShift(char *inputChar, SDL_Event event);
private SDL_Rect Render_GetWordsTotalRect(char *words, SDL_position leftUpPos, SDL_size charSize);


// render 內部載入各種初始化用
public void Render_Init(SDL_Renderer *rememberedRenderer)
{
    // 讓 Render 記住 renderer，之後就不用傳入 renderer
    renderer = rememberedRenderer;

    // 設定我要畫有透明度的東西
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // SearchWords 初始化
    SearchWords_Init();

    // 暫停文字，預先 load 好
    SDL_Color wordColor = PauseScreen_GetPauseWordColor(); // 取得文字顏色
    char *wordsContent = PauseScreen_GetPauseWordContent(); // 取得文字
    SDL_Surface *wordsSerface;
    wordsSerface = TTF_RenderText_Solid(font, wordsContent, wordColor);
    pauseWord_texture = SDL_CreateTextureFromSurface(renderer, wordsSerface); // serface 轉 texture
    SDL_FreeSurface(wordsSerface);                                                  // serface 沒用了

    // 載入背景
    background_texture = IMG_LoadTexture(renderer, GetAssetsInFolder("background", "png"));
    if(background_texture == NULL)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to open background picture"));
        SDL_EndAll_StopProgram(false);
    }

    // 載入 mapCursor
    mapCurosr_texture = IMG_LoadTexture(renderer, GetAssetsInFolder("mapCursor", "png"));
    if(background_texture == NULL)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to open mapCurosr picture"));
        SDL_EndAll_StopProgram(false);
    }

    // 載入 hotbar、hotBarCursor
    hotbarCursor_texture = IMG_LoadTexture(renderer, GetAssetsInFolder("hotbarCursor", "png"));
    if(hotbarCursor_texture == NULL)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to open hotbarCursor picture"));
        SDL_EndAll_StopProgram(false);
    }
    hotbar_texture = IMG_LoadTexture(renderer, GetAssetsInFolder("hotbar", "png"));
    if(hotbarCursor_texture == NULL)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to open hotbar picture"));
        SDL_EndAll_StopProgram(false);
    }

    // 載入 背包、背包Cursor
    backpackCursor_texture = IMG_LoadTexture(renderer, GetAssetsInFolder("backpackCursor", "png"));
    if(backpackCursor_texture == NULL)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to open backpackCursor picture"));
        SDL_EndAll_StopProgram(false);
    }
    backpack_texture = IMG_LoadTexture(renderer, GetAssetsInFolder("backpack", "png"));
    if(backpackCursor_texture == NULL)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to open backpack picture"));
        SDL_EndAll_StopProgram(false);
    }
}

// 取得各種素材的位置用(副檔名不用加 .) (失敗回傳 NULL)
private char *GetAssetsInFolder(const char *folderName, const char *extensionName)
{
    // 開 file path 名字 buffer、建立到 font 資料夾的相對路徑
    char path[MAX_ABBSOULTE_PATH_LENGTH]; 
    sprintf(path, "%s/%s/", ASSETS_FOLDER_RELATIVE_PATH, folderName);
    
    // 打開 dir (都是用相對路徑了！)
    DIR *dir;
    if ((dir = opendir(path)) == NULL)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to open folder: %s\n"), ASSETS_FOLDER_RELATIVE_PATH);
        return NULL;
    }

    // 讀 dir 內的所有 subfile
    struct dirent *fileData;
    while ((fileData = readdir(dir)) != NULL) // 會用字母順序排，所以如果有多個需要的 extension，最接近A的會被開啟
    {
        // 副檔名長度
        int extensionLength = strlen(extensionName);

        // 取的最後四個字
        int fileName_lastWordStartIndex = strlen((*fileData).d_name) - 1 - extensionLength; // -1 換 0-base，因不含\0，會到末index，-extensionLength是到前extensionLength index，也就是前extensionLength+1個字
        char fileName_lastWords[extensionLength+1 + 1]; // 含. \0，所以 +1 +1
        for (int i = 0; i < extensionLength+1; ++i)
            fileName_lastWords[i] = (*fileData).d_name[fileName_lastWordStartIndex + i];
        fileName_lastWords[extensionLength+1] = '\0';

        // 是否最後四個字(副檔名) 是 我們要的
        char fullExtension[MAX_ABBSOULTE_PATH_LENGTH]; sprintf(fullExtension, ".%s", extensionName);
        if (strcmp(fileName_lastWords, fullExtension) == 0) 
            return strdup(strcat(path, (*fileData).d_name));  // 接到 目前路徑，回傳 + 結束 loop       
    }
    // 到此，代表沒找到
    fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Can't find any .%s file in %s\n"), extensionName, ASSETS_FOLDER_RELATIVE_PATH);
    return NULL;
}

// render 內部解除各種功能用
public void Render_Clear()
{
    // SearchWords 結束
    SearchWords_Clear();

    // free 暫停文字、背景圖、mapCursor、hotbar、hotbarCursor、背包、背包cursor
    SDL_DestroyTexture(pauseWord_texture);
    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(mapCurosr_texture);
    SDL_DestroyTexture(hotbar_texture);
    SDL_DestroyTexture(hotbarCursor_texture);
    SDL_DestroyTexture(backpack_texture);
    SDL_DestroyTexture(backpackCursor_texture);
}

// 畫出背景
public void Render_RenderBackground()
{
    // 所需設定
    int addSideIndex_x = 1, addSideIndex_y = 1; // 1是加右邊，-1是加左邊 
    SDL_Rect rect = (SDL_Rect){.x = background_position.x, .y =  -background_position.y, .w = WINDOW_WIDTH, .h = WINDOW_HEIGHT};

    // 先畫目前的
    SDL_RenderCopy(renderer, background_texture, NULL, &rect);

    // 判斷是否要換方向 (往左 or 往右填)
    if(background_position.x > 0)
        addSideIndex_x = -1;
    if(background_position.y > 0)
        addSideIndex_y = -1;
    
    // 畫出剩下3個
    rect.x += WINDOW_WIDTH * addSideIndex_x;
    SDL_RenderCopy(renderer, background_texture, NULL, &rect);
    rect.y -= WINDOW_HEIGHT * addSideIndex_y;
    SDL_RenderCopy(renderer, background_texture, NULL, &rect);
    rect.x -= WINDOW_WIDTH * addSideIndex_x;
    SDL_RenderCopy(renderer, background_texture, NULL, &rect);
}

// 畫背包到 renderer
public void Render_RenderBackpack()
{
    // 取得背包位置、大小
    SDL_position backpackPos = Backpack_GetPosition();
    SDL_size backpackSize = Backpack_GetSize();

    // 畫出背包
    SDL_Rect rect = (SDL_Rect){.x = backpackPos.x, .y = backpackPos.y, .w = backpackSize.width, .h = backpackSize.height};
    SDL_RenderCopy(renderer, backpack_texture, NULL, &rect);
    
    // 取得 in cell 方塊資訊
    SDL_position inCellBlockStartPos = Backpack_GetInCellBlockStartPos();
    SDL_size inCellBlockSize = Backpack_GetCellSize();
    SDL_size backpackCellNum = Backpack_GetBlockNumberInWidthAndHeight();
    static short *textureIDs = NULL; int totalIDnum; // 要初始化！
    TextureBase_GetAllID(&textureIDs, &totalIDnum);

    // 把方塊畫上去
    SDL_position nowDrawBlockPos = inCellBlockStartPos;
    for(int y = 0; y < backpackCellNum.height; ++y)
    {
        nowDrawBlockPos.x = inCellBlockStartPos.x;
        for(int x = 0; x < backpackCellNum.width; ++x)
        {
            int nowIndex = x + y * backpackCellNum.width; // 現在是第幾格方塊 (只是2D變1D)
            if(nowIndex < totalIDnum) // 比總數小，有東西，畫出！
                Render_RenderBlockInBackpack(nowDrawBlockPos, textureIDs[nowIndex]);
            nowDrawBlockPos.x += inCellBlockSize.width;
        }
        nowDrawBlockPos.y += inCellBlockSize.height;
    }
}

// 畫背包上的方塊到 renderer
private void Render_RenderBlockInBackpack(SDL_position nowPos, short blockID)
{
    // 取得材質 by 編號
    SDL_Texture *nowTexture = TextureBase_GetTextureUsingID(blockID);
    SDL_size blockSize = Backpack_GetInCellBlockSize();

    // 畫出
    SDL_Rect rect = (SDL_Rect){.x = nowPos.x, .y = nowPos.y, .w = blockSize.width, .h = blockSize.height};
    SDL_RenderCopy(renderer, nowTexture, NULL, &rect);
}

// 畫背包的 cursor 到 renderer
public void Render_RenderBackpackCursor()
{
    // 取得 cursor 位置、大小
    SDL_position cursorPos = Backpack_GetCursorPosition(); 
    if(cursorPos.x == POS_NOT_EXISTS) return ; // 這代表 cursor 不存在，不用畫
    SDL_size cursorSize = Backpack_GetCursorSize();

    // 畫出 CURSOR
    SDL_Rect rect = (SDL_Rect){.x = cursorPos.x, .y = cursorPos.y, .w = cursorSize.width, .h = cursorSize.height};
    SDL_RenderCopy(renderer, backpackCursor_texture, NULL, &rect);
}

// 依 camera 差距，畫出地圖
public void Render_RenderMap()
{
    // 取得 Map 要顯示大陣列全部(因為要一直分記憶體很麻煩，所以用static，並初始化以判斷是否要分記憶體)、左上角位置、方塊大小、長寬方塊數量
    static short **renderingMap = NULL;
    SDL_size totalBlockNumInWindow;
    Map_GetShowedMapData(&renderingMap, &totalBlockNumInWindow);
    SDL_size blockSize = Map_GetBlockSize();
    SDL_position startBlockPos = Map_GetUpLeftCornerPosition();
    startBlockPos.x *= blockSize.width;
    startBlockPos.y *= blockSize.height; // 取得的位置是「方塊數量位置」不是真正座標，要換成真正座標 (for renderCopy)

    // 顯示
    SDL_position nowBlockPos = startBlockPos;
    for (int y = 0; y < totalBlockNumInWindow.height; ++y)
    {
        nowBlockPos.x = startBlockPos.x;
        for (int x = 0; x < totalBlockNumInWindow.width; ++x)
        {
            if (renderingMap[y][x] != NO_BLOCK_ID)
            {
                // get 目前 block 的材質
                SDL_Texture *blockTexture = TextureBase_GetTextureUsingID(renderingMap[y][x]);

                // 算座標 (因實際顯示往下才是 +y ，所以要算完相對 y 座標後加負號)、顯示
                SDL_Rect rect = (SDL_Rect){.x = nowBlockPos.x - cameraPosition.x, .y = -(nowBlockPos.y - cameraPosition.y), .w = blockSize.width, .h = blockSize.height};
                SDL_RenderCopy(renderer, blockTexture, NULL, &rect);
            }
            nowBlockPos.x += blockSize.width;
        }
        nowBlockPos.y -= blockSize.height;
    }
}

// 依 camera 差距，畫出 Map 的 cursor
public void Render_RenderMapCursor()
{
    // 取得 cursor 位置、大小
    SDL_position cursorPos = Map_GetCursorPosition();
    SDL_size cursorize = Map_GetCursorSize();
    cursorPos.x *= cursorize.width;
    cursorPos.y *= cursorize.height;

    // 要算出 camera 相對位置
    SDL_Rect rect = (SDL_Rect){.x = cursorPos.x - cameraPosition.x, .y = -(cursorPos.y - cameraPosition.y), .w = cursorize.width, .h = cursorize.height};

    // 畫出
    SDL_RenderCopy(renderer, mapCurosr_texture, NULL, &rect);
}

// 畫出快捷欄
public void Render_RenderHotbar()
{
    // 取得快捷欄位置、大小
    SDL_position hotbarPos = Hotbar_GetPosition();
    SDL_size hotbarize = Hotbar_GetSize();

    // 畫出
    SDL_Rect rect = (SDL_Rect){.x = hotbarPos.x, .y = hotbarPos.y, .w = hotbarize.width, .h = hotbarize.height};
    SDL_RenderCopy(renderer, hotbar_texture, NULL, &rect);

    // 取得 in cell 方塊資訊
    SDL_position inCellBlockStartPos = HotBar_GetInCellBlockStartPos();
    SDL_size inCellBlockSize = HotBar_GetInCellBlockSize();
    SDL_size cellSize = HotBar_GetCellSize();
    static short *blockIDArray = NULL; // 要初始化！
    int totalCellNum;
    HotBar_GetAllID(&blockIDArray, &totalCellNum);

    // 畫出所有在 快捷欄中的方快
    rect = (SDL_Rect){.x = inCellBlockStartPos.x, .y = inCellBlockStartPos.y, .w = inCellBlockSize.width, .h = inCellBlockSize.height};
    for(int i = 0; i < totalCellNum; ++i)
    {
        if(blockIDArray[i] != NO_BLOCK_ID)
        {
            // 取 Texturew、畫出
            SDL_Texture *blockTexture = TextureBase_GetTextureUsingID(blockIDArray[i]);
            SDL_RenderCopy(renderer, blockTexture, NULL, &rect);
        }
        rect.x += cellSize.width; // 到下一個 cell
    }
}

// 畫出快捷欄的 cursor
public void Render_RenderHotbarCursor()
{
    // 取得 cursor 位置、大小
    SDL_position hotbarCursorPos = Hotbar_GetCursorPosition();
    SDL_size hotbarCursorSize = HotBar_GetCellSize();

    // 畫出
    SDL_Rect rect = (SDL_Rect){.x = hotbarCursorPos.x, .y = hotbarCursorPos.y, .w = hotbarCursorSize.width, .h = hotbarCursorSize.height};
    SDL_RenderCopy(renderer, hotbarCursor_texture, NULL, &rect);
}

// 畫出已打的字
public void Render_RenderSearchWords()
{
    // 要取得打字處的開頭、字的大小
    SDL_position wordStartPos = Backpack_GetSearchWordPosition();
    SDL_size wordSize = Backpack_GetSearchWordSize();

    // 顯示文字
    int totalWidthNow = 0;
    for (int i = 0; (*searchWords).words[i] != '\0'; ++i)
    {
        // 取得字大小
        SDL_size originalSize;
        char nowCharToString[2];
        sprintf(nowCharToString, "%c", (*searchWords).words[i]);                        // 怕只能處理 string，所以要轉成 string (轉完會自動補 \0)
        TTF_SizeText(font, nowCharToString, &originalSize.width, &originalSize.height); // 取得 font 原本大小
        float widthScaling = (float)wordSize.height / (float)originalSize.height;       // 換成 float ，處理縮放
        wordSize.width = originalSize.width * widthScaling;                             // height 就是用給的，但 width 需要依 height 去伸縮 (每個字 width 不同)

        // 大小，位置確定
        SDL_Rect wordRect;
        wordRect.x = wordStartPos.x + totalWidthNow; // 左上座標的 x
        wordRect.y = wordStartPos.y;
        wordRect.w = wordSize.width;
        wordRect.h = wordSize.height;
        totalWidthNow += wordSize.width; // 下一次要加上此字的寬度

        // 顯示此字
        SDL_RenderCopy(renderer, (*searchWords).wordsTexture[i], NULL, &wordRect);
    }
}

// 畫出暫停 mask 與 文字
public void Render_RenderPauseStuff()
{
    // mask
    SDL_Color maskColor = PauseScreen_GetPauseBackgroundColor();
    SDL_SetRenderDrawColor(renderer, maskColor.r, maskColor.g, maskColor.b, maskColor.a); // 設定要畫的顏色
    SDL_Rect rect = { .x = 0 , .y = 0, .w = WINDOW_WIDTH, .h = WINDOW_HEIGHT }; // 要畫的方形大小 (整個window)
    SDL_RenderFillRect(renderer, &rect); // 畫出來！ (FillRect會畫在最上方，符合所需)

    // 文字位置、大小
    SDL_position pos = {.x = 0, .y = 0}; 
    SDL_size size = PauseScreen_GetPauseWordSize();
    SDL_Rect wordRect = Render_GetWordsTotalRect(PauseScreen_GetPauseWordContent(), pos, size);
    wordRect.x = WINDOW_WIDTH /2 - wordRect.w /2; // 置中
    wordRect.y = WINDOW_HEIGHT /2 - wordRect.h /2; // 置中

    // 顯示文字
    SDL_RenderCopy(renderer, pauseWord_texture, NULL, &wordRect);
    
    // 全部疊在目前畫面上，顯示(暫停後跑不到顯示部分，需要自己顯示)
    SDL_RenderPresent(renderer);
}

// 取得文字 rect 用
private SDL_Rect Render_GetWordsTotalRect(char *words, SDL_position leftUpPos, SDL_size charSize)
{
    // 直接算文字總長度
    SDL_size stringSize = charSize;
    stringSize.width *= strlen(words);

    // 取得字大小
    SDL_size originSize;
    TTF_SizeText(font, words, &originSize.width, &originSize.height); // 取得 font 原本大小
    float widthScaling = (float)stringSize.height / (float)originSize.height;  // 換成 float ，處理縮放
    stringSize.width = originSize.width * widthScaling;                      // height 就是用給的，但 width 需要依 height 去伸縮 (每個字 width 不同)

    // 大小，位置確定
    SDL_Rect wordRect = {.x = leftUpPos.x, .y = leftUpPos.y, .w = stringSize.width, .h = stringSize.height};
    return wordRect;
}

// 初始化，搜尋文字 Array
private void SearchWords_Init()
{
    // 分配記憶體、初始化
    searchWords = (wordsArray *)malloc(sizeof(wordsArray));
    *searchWords = (wordsArray){.words = (char *)malloc(sizeof(char) * DEFULT_ARRAY_SIZE), .wordsTexture = (SDL_Texture **)malloc(sizeof(SDL_Texture *) * DEFULT_ARRAY_SIZE), .nowMax = DEFULT_ARRAY_SIZE, .nowStored = 0};
    // 是字串，所以預設要有 \0
    (*searchWords).words[0] = '\0';
    ++(*searchWords).nowStored; // 所以 SDL_Texture 會永遠空一個

    // 顯示文字的函式庫，初始化
    if (TTF_Init() < 0)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to open SDL_TTF: %s\n") , TTF_GetError());
        SDL_EndAll_StopProgram(false);
    }
    // font 要先記住，之後在結束時才能 Close，記住也就不用每次開啟，方便
    font = TTF_OpenFont(GetAssetsInFolder("font", "ttf"), FONT_DEFULT_PIXEL_QUALITY);;
    if (font == NULL)
    {
        fprintf(stderr, STRING_LITERAL_IN_PRINTF_RED("Failed to open TTF file: %s\n"), TTF_GetError());
        TTF_Quit();
        SDL_EndAll_StopProgram(false);
    }

    // 提醒文字，預先 load 好
    SDL_Color wordColor[2];
    Backpack_GetSearchNotifyColor(wordColor); // 取得文字顏色
    char *wordsContents[2];
    Backpack_GetSearchNotifyContent(wordsContents); // 取得文字
    SDL_Surface *wordsSerface;
    for (int i = 0; i <= 1; ++i)
    {
        wordsSerface = TTF_RenderText_Solid(font, wordsContents[i], wordColor[i]);

        searchNotify_Texture[i] = SDL_CreateTextureFromSurface(renderer, wordsSerface); // serface 轉 texture
        SDL_FreeSurface(wordsSerface);                                                  // serface 沒用了
    }
}

// 清除，搜尋文字 Array
public void SearchWords_Clear()
{
    // free 掉 提醒文字 Texture
    SDL_DestroyTexture(searchNotify_Texture[searchNotifyIndex_success]);
    SDL_DestroyTexture(searchNotify_Texture[searchNotifyIndex_failure]);

    // free 掉搜尋 Texture
    for (int i = 0; i < (*searchWords).nowStored; ++i)
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
    if (event.type == SDL_KEYDOWN)
    {
        // Backspace，要刪除字
        if (event.key.keysym.sym == SDLK_BACKSPACE)
        {
            if((*searchWords).nowStored >= 2)
            {
                // 取得刪除 index
                int nowStoredIndex = (*searchWords).nowStored - 1 - 1; // -1到 0-base，是\0，再 -1 才到 \0 後面一個 char
                // 刪文字、Texture
                (*searchWords).words[nowStoredIndex] = '\0'; // 把 \0 前移，就是刪除字了
                SDL_DestroyTexture((*searchWords).wordsTexture[nowStoredIndex]);
                // --已存東西
                --(*searchWords).nowStored;
            }
        }
        // 其他，是要新增的字
        else
        {
            // 不能印，是控制字元，不用存
            if (!SDL_isprint(event.key.keysym.sym))
                return;

            // 把輸入轉成 char + 小寫轉大寫
            char inputChar = (char)event.key.keysym.sym;
            if ('a' <= inputChar && inputChar <= 'z')
            {
                if (event.key.keysym.mod & (KMOD_CAPS | (KMOD_LSHIFT | KMOD_RSHIFT))) // 偵測這是否在 CapsLock 或 按著 shift 時打的
                    inputChar = inputChar - 'a' + 'A';
            }
            else
                SearchWords_chaningPressingShift(&inputChar, event); // 按 shift 的變動

            // 取得要存入的 index
            ++(*searchWords).nowStored;
            int nowStoredIndex = (*searchWords).nowStored - 1 - 1; // -1到 0-base，是\0後面，再 -1 才到 \0
            // 存入(字)
            (*searchWords).words[nowStoredIndex] = inputChar;
            (*searchWords).words[nowStoredIndex + 1] = '\0'; // 記得補回 \0
            // 存入(Texture)
            SDL_Color wordColor = Backpack_GetSearchWordColor(); // 取得文字顏色
            char InputtedcharToString[2];
            sprintf(InputtedcharToString, "%c", inputChar); // sprintf 轉完會自動補 \0                                        // 怕只能處理 string，所以要轉成 string
            SDL_Surface *wordsSerface = TTF_RenderText_Solid(font, InputtedcharToString, wordColor); // 只能畫在 serface
            SDL_Texture *wordsTexture = SDL_CreateTextureFromSurface(renderer, wordsSerface);        // serface 轉 texture
            SDL_FreeSurface(wordsSerface);                                                           // serface 沒用了
            (*searchWords).wordsTexture[nowStoredIndex] = wordsTexture;

            // searchWords 空間滿了，擴充它！
            if ((*searchWords).nowStored == (*searchWords).nowMax)
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
    if (event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT)) // 他是用 bitstream 設計是否有按 or 重複按鍵盤的
    {
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
}

// 取得 搜尋文字
public char *SearchWords_GetSearchingWords()
{
    return (*searchWords).words;
}

// 切換 搜尋成功 or 失敗，重設秒數
public void Render_EnableSearchNotify(bool isSuccessFound)
{
    isSuccessSearching = isSuccessFound;
    searchStartTime = SDL_GetTicks64();
}

// 強制關閉搜尋提醒
public void Render_ForceCloseSearchNotify()
{
    searchStartTime = -SEARCH_NOTIFY_DISAPPEAR_MILISECOND;
}

// 畫出 搜尋成功 or 失敗 的文字
public void Render_RenderSearchNotify()
{
    // 算是否暫停
    static int beforeTime; int nowtime = SDL_GetTicks64(); 
    if(nowtime - beforeTime > MAX_ALLOW_NOT_CALLING_MOVING_DELTATIME) // 是暫停，加上暫停時的時數，才會是正確實際消失時間
        searchStartTime += (nowtime - beforeTime);
    beforeTime = nowtime;

    // 時間差大，代表沒有按搜尋，時間差夠小才顯示 (因一開始 SEARCH_NOTIFY_DISAPPEAR_MILISECOND 也不顯示)
    if(nowtime - searchStartTime > SEARCH_NOTIFY_DISAPPEAR_MILISECOND || nowtime < SEARCH_NOTIFY_DISAPPEAR_MILISECOND)
        return;

    // 取得所有要顯示的資訊
    SDL_position rectPos = Backpack_GetSearchNotifyPosition();
    SDL_size rectSize = Backpack_GetSearchNotifySize();
    char *noticeContents[2];
    Backpack_GetSearchNotifyContent(noticeContents);

    // 要顯示甚麼文字
    int shownNoticeIndex = isSuccessSearching ? searchNotifyIndex_success : searchNotifyIndex_failure;

    // 大小，位置確定
    SDL_Rect wordRect = Render_GetWordsTotalRect(noticeContents[shownNoticeIndex], rectPos, rectSize);

    // 畫出底色
    SDL_Color maskColor = Backpack_GetSearchNotifygroundColor();
    SDL_SetRenderDrawColor(renderer, maskColor.r, maskColor.g, maskColor.b, maskColor.a); // 設定要畫的顏色
    SDL_RenderFillRect(renderer, &wordRect); // 畫出來！ (FillRect會畫在最上方，符合所需)

    // 顯示此段字
    SDL_RenderCopy(renderer, searchNotify_Texture[shownNoticeIndex], NULL, &wordRect);
}

// 移動相機 (會另外 get 鍵盤輸入)
public void Render_MoveCamera()
{
    // 偵測輸入 (偵測是否有按著鍵盤)
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    // 算出上次移動與這次移動的時間差 (每次呼叫時間不同)
    static int beforeTime = 0;
    int nowtime = SDL_GetTicks64();
    int deltaTime = nowtime - beforeTime;
    beforeTime = nowtime;
    if(deltaTime >  MAX_ALLOW_NOT_CALLING_MOVING_DELTATIME) // 太久沒呼叫此函式，是暫停，所以之前的時間不算，要重算一次真正從沒暫停開始時間
        return ;

    // 移動 (加速度 * 時間 = 速度) // 反方向輸入，直接把速度歸0
    bool isPressing = false;
    if (keystate[SDL_SCANCODE_W]) // 回傳 1 代表按著(測試了快按也會偵測到)，0 代表沒按
    {
        if (cameraVelocityRecord.y < 0) 
            cameraVelocityRecord.y = 0;
        if(cameraVelocityRecord.y < MAX_CAMERA_MOVE_SPEED * 1000) // 最大速限要把秒換毫秒
            cameraVelocityRecord.y += CAMERA_MOVE_ACCLERATION * deltaTime;
        isPressing = true;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        if (cameraVelocityRecord.y > 0)
            cameraVelocityRecord.y = 0;
        if(cameraVelocityRecord.y > -MAX_CAMERA_MOVE_SPEED * 1000)
            cameraVelocityRecord.y -= CAMERA_MOVE_ACCLERATION * deltaTime;
        isPressing = true;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        if (cameraVelocityRecord.x < 0)
            cameraVelocityRecord.x = 0;
        if (cameraVelocityRecord.x < MAX_CAMERA_MOVE_SPEED * 1000)
            cameraVelocityRecord.x += CAMERA_MOVE_ACCLERATION * deltaTime;
        isPressing = true;
    }
    if (keystate[SDL_SCANCODE_A])
    {
        if (cameraVelocityRecord.x > 0)
            cameraVelocityRecord.x = 0;
        if (cameraVelocityRecord.x > -MAX_CAMERA_MOVE_SPEED * 1000)
            cameraVelocityRecord.x -= CAMERA_MOVE_ACCLERATION * deltaTime;
        isPressing = true;
    }

    // 計算速度 (放開減速)
    if (!isPressing)
    {
        if (cameraVelocityRecord.x > 0)
        {
            cameraVelocityRecord.x -= CAMERA_MOVE_ACCLERATION * deltaTime;
            if(cameraVelocityRecord.x < 0) cameraVelocityRecord.x = 0;
        }    
        if (cameraVelocityRecord.x < 0)
        {
            cameraVelocityRecord.x += CAMERA_MOVE_ACCLERATION * deltaTime;
            if(cameraVelocityRecord.x > 0) cameraVelocityRecord.x = 0;
        }
        if (cameraVelocityRecord.y > 0)
        {
            cameraVelocityRecord.y -= CAMERA_MOVE_ACCLERATION * deltaTime;
            if(cameraVelocityRecord.y < 0) cameraVelocityRecord.y = 0;
        }   
        if (cameraVelocityRecord.y < 0)
        {
            cameraVelocityRecord.y += CAMERA_MOVE_ACCLERATION * deltaTime;
            if(cameraVelocityRecord.y > 0) cameraVelocityRecord.y = 0;
        }
    }

    // 相機移動 (速度 * 時間 = 距離)
    static SDL_position acculmatedMovement = (SDL_position){.x = 0, .y = 0}, acculmatedRealMove = (SDL_position){.x = 0, .y = 0};
    acculmatedMovement.x += cameraVelocityRecord.x * deltaTime;
    acculmatedMovement.y += cameraVelocityRecord.y * deltaTime;
    // 因微小變化 /0 會消失，所以要用累計，夠多才能實際移動 camera
    if(acculmatedMovement.x >= 1000 || acculmatedMovement.x <= -1000) // 要把毫秒換秒，所以以 1000 為單位
    {
        cameraPosition.x += acculmatedMovement.x /1000; // 多了幾個 1000 就多幾步
        acculmatedRealMove.x += acculmatedMovement.x /1000; // 累積實際移動量 for 背景
        acculmatedMovement.x %= 1000; // 剩的留著
    }
    if(acculmatedMovement.y >= 1000 || acculmatedMovement.y <= -1000)
    {
        cameraPosition.y += acculmatedMovement.y /1000;
        acculmatedRealMove.y += acculmatedMovement.y /1000;
        acculmatedMovement.y %= 1000;
    }
    // 背景移動 (每一動 BACKGROUND_SLOWER_TIMES 才移動1步)
    if(acculmatedRealMove.x >= BACKGROUND_SLOWER_TIMES || acculmatedRealMove.x <= -BACKGROUND_SLOWER_TIMES) // 要把毫秒換秒，所以以 1000 為單位
    {
        background_position.x -= acculmatedRealMove.x / BACKGROUND_SLOWER_TIMES; // camera 往前，backbround 往後
        acculmatedRealMove.x %= BACKGROUND_SLOWER_TIMES; // 剩的留著
    }
    if(acculmatedRealMove.y >= BACKGROUND_SLOWER_TIMES || acculmatedRealMove.y <= -BACKGROUND_SLOWER_TIMES) 
    {
        background_position.y -= acculmatedRealMove.y / BACKGROUND_SLOWER_TIMES; 
        acculmatedRealMove.y %= BACKGROUND_SLOWER_TIMES; 
    }


    // 背景移動過頭，返回原點
    if(background_position.x > WINDOW_WIDTH || background_position.x < -WINDOW_WIDTH )
        background_position.x = 0;
    if(background_position.y > WINDOW_HEIGHT || background_position.y < -WINDOW_HEIGHT )
        background_position.y = 0; 
}

// 取得相機絕對位置
public SDL_position Render_GetCameraPosition()
{
    return cameraPosition;
}