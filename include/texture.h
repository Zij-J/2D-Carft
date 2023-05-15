/* 材質(背包)資料庫(紅黑樹？)、Sort、Search 與 相關function (file I/O：編號對應材質名 file、圖片資料夾)*/
#ifndef TEXTURE
#define TEXTURE

void TextureBase_Init();
int *TextureBase_GetAllID(); // 這裡很不確定要回傳什麼
void TextureBase_Clear();

#endif