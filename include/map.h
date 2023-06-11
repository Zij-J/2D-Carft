/* map大陣列、地圖file、地圖cursor 與 相關 function (file I/O：map file)*/
#ifndef MAP
#define MAP


// finished
void Map_Init();
void Map_Clear();
void Map_UpdateMaps();
void Map_MoveCursor(SDL_Event event);
void Map_EditBlock(SDL_Event event);//need to get Hotbar current block number + camera Position
void Map_GetShowedMapData(short ***mapInWindow, SDL_size *totalBlockNumberInWindow);
SDL_position Map_GetUpLeftCornerPosition();
SDL_position Map_GetCursorPosition();
SDL_size Map_GetBlockSize();
SDL_size Map_GetCursorSize();

// unused
// bool Map_isInput(SDL_Event event); //not need now //merge in Map_EditBlock()

#endif
