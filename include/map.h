/* map大陣列、地圖file、地圖cursor 與 相關 function (file I/O：map file)*/
#ifndef MAP
#define MAP

#define NO_BLOCK_ID -9 //no block = -9

// finished
void Map_Init();
void Map_Clear();
void Map_UpdateMaps();
void Map_MoveCursor(SDL_Event event);
void Map_EditBlock(SDL_Event event);//need to get Hotbar current block number + camera Position
void Map_GetShowedMapData(short ***mapInWindow, SDL_size *totalBlockNumberInWindow);
bool Map_isInput(SDL_Event event); //not need now //merge in Map_EditBlock() // 還是要一下！不然要多每個迴圈都要多判斷不少東西
SDL_position Map_GetUpLeftCornerPosition();
SDL_position Map_GetCursorPosition();
SDL_size Map_GetBlockSize();
SDL_size Map_GetCursorSize();


#endif
