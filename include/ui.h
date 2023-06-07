/* 快捷欄資料庫、UI圖片顯示(含文字array)、背包cursor 與相關操作 */
#ifndef UI
#define UI

// GetCursorOnArea用，也就是溝通 Cursor 在方塊區、還是在搜尋區
typedef enum {blockArea, searchArea, nothingArea} cursorArea;
// searchNotify用，代表成功失敗的 index 
enum searchNotifyIndex{searchNotifyIndex_success, searchNotifyIndex_failure};

// unfinished
void HotBar_Init();
void HotBar_Clear();
void HotBar_MoveCursor(SDL_Event event);
void HotBar_GetAllID();
short HotBar_GetChosenBlockID();
void Backpack_MoveCursor(SDL_Event event);
void Backpack_UpdateBlockToHotbar(SDL_Event event);
void Backpack_FindBlockAndMoveCursor();
bool Backpack_isInput(SDL_Event event);
SDL_position Hotbar_GetPosition();
SDL_position Hotbar_GetCursorPosition();
SDL_position Backpack_GetPosition();
SDL_position Backpack_GetBlockStartPosition();
SDL_position Backpack_GetCursorPosition();
SDL_size Hotbar_GetSize();
SDL_size HotBar_GetCursorSize();
SDL_size Backpack_GetSize();
SDL_size Backpack_GetBlockSize();
SDL_size Backpack_GetCursorSize();
cursorArea Backpack_GetCursorOnArea();

// finished
void Backpack_Switch(SDL_Event event);
bool Backpack_isOpen();
SDL_position Backpack_GetSearchWordPosition();
SDL_size Backpack_GetSearchWordSize();
SDL_Color Backpack_GetSearchWordColor();
SDL_position Backpack_GetSearchNotifyPosition();
SDL_size Backpack_GetSearchNotifySize();
void Backpack_GetSearchNotifyColor(SDL_Color *array);
void Backpack_GetSearchNotifyContent(char **array);

#endif
