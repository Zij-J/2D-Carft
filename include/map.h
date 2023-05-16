/* map大陣列、地圖file、地圖cursor 與 相關 function (file I/O：map file)*/
#ifndef MAP
#define MAP

// unfinished
void Maps_Init();
void Maps_Clear();
bool Map_isInput(SDL_Event *event);
int ***Map_GetTotalMap();
SDL_position Map_GetUpLeftCornerPosition();
SDL_position Map_GetCursorPosition();
SDL_size Map_GetBlockSize();
SDL_size Map_GetCursorSize();

// finished

#endif