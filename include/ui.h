/* 快捷欄資料庫、UI圖片顯示(含文字array)、背包cursor 與相關操作 */
#ifndef UI
#define UI

void HotBar_Init();
void HotBar_Clear();
void Backpack_Switch(SDL_Event *event);
bool Backpack_isOpen();
void Backpack_MoveCursor();
bool Backpack_isInput();

#endif