/* 快捷欄資料庫、UI圖片顯示(含文字array)、背包cursor 與相關操作 */
#ifndef UI
#define UI

// unfinished
void HotBar_Init();
void HotBar_Clear();
void HotBar_MoveCursor();
void HotBar_GetAllID();
int HotBar_GetChosenBlockID();
void Backpack_MoveCursor(SDL_Event *event);
bool Backpack_isInput(SDL_Event *event);
SDL_position Hotbar_GetPosition();
SDL_position Hotbar_GetCursorPosition();
SDL_position Backpack_GetPosition();
SDL_position Backpack_GetBlockStartPosition();
SDL_position Backpack_GetCursorPosition();
SDL_position Hotbar_GetSize();
SDL_size HotBar_GetCursorSize();
SDL_size Backpack_GetSize();
SDL_size Backpack_GetBlockSize();
SDL_size Backpack_GetCursorSize();

// finished
void Backpack_Switch(SDL_Event *event);
bool Backpack_isOpen();

#endif