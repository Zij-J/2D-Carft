/* 材質(背包)資料庫、Sort、Search 與 相關function (file I/O：編號對應材質名IDtoNameBase、圖片資料夾block_pictures)*/
#ifndef TEXTURE
#define TEXTURE

// unfinished

// finished
void TextureBase_Init(SDL_Renderer *renderer);// 初始化資料庫
void TextureBase_Clear();// 清除資料庫
void IDtoNameBase_Init();// 匯入編號對應材質名字 file
void IDtoNameBase_Clear();// 清除編號對應材質名字 file
bool TextureBase_isFindBlockBySearchWords();// 依搜尋文字，找找看是否有方塊是此名字，回傳是否找到
int TextureBase_GetSearchedBlockIndex();// 把搜尋到材質資料庫的第幾個回傳
void TextureBase_GetAllID(short **IDbuffer, int *totalBlockNum);// 取得材質資料庫所有的材質ID
SDL_Texture *TextureBase_GetTextureUsingID(short blockID); // 要有 ID 找到 SDL_Texture 的功能！

// unused // 最終沒用到，但很好用！
void TextureBase_DeleteTexture(const char* textureName); //刪除指定圖片
void IDtoNameBase_SortBaseByID(); //依照方塊編號大小排序資料庫
SDL_Texture *TextureBase_GetTextureName(char* textureName);// 依方塊名稱取得圖片 

#endif
