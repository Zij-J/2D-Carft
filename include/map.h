/* map大陣列、地圖file、地圖cursor 與 相關 function (file I/O：map file)*/
#ifndef MAP
#define MAP

// unfinished
void Map_Init();
void Map_Clear();
void Map_UpdateMaps();
void Map_EditBlock(SDL_Event event);
bool Map_isInput(SDL_Event event);
short **Map_GetTotalMap();
SDL_position Map_GetUpLeftCornerPosition();
SDL_position Map_GetCursorPosition();
SDL_size Map_GetBlockSize();
SDL_size Map_GetCursorSize();

// finished

#endif