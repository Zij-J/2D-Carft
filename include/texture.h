/* 材質(背包)資料庫(紅黑樹？)、Sort、Search 與 相關function (file I/O：編號對應材質名 file、圖片資料夾)*/
#ifndef TEXTURE
#define TEXTURE

// unfinished
void TextureBase_Init();
void TextureBase_Clear();
bool TextureBase_isFindBlockBySearchWords();
short *TextureBase_GetAllBlock(); // 這裡很不確定要回傳什麼
short TextureBase_GetSearchedBlockID();
SDL_Texture *TetxureBase_GetTexture(short blockID);

// finished

#endif