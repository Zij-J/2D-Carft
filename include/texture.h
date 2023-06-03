/* 材質(背包)資料庫、Sort、Search 與 相關function (file I/O：編號對應材質名IDtoNameBase、圖片資料夾block_pictures)*/
#ifndef TEXTURE
#define TEXTURE

// unfinished
SDL_Texture *TextureBase_GetTextureByID(short ID); // 要有 ID 找到 SDL_Texture 的功能！

// finished
void TextureBase_Init(SDL_Renderer *renderer);// 初始化資料庫
void TextureBase_Clear();// 清除資料庫
void IDtoNameBase_Init();// 匯入編號對應材質名字 file
void IDtoNameBase_Clear();// 清除編號對應材質名字 file
bool TextureBase_isFindBlockBySearchWords();// 依搜尋文字，找找看是否有方塊是此名字，回傳是否找到
short TextureBase_GetSearchedBlockID();// 把搜尋到的方塊編號回傳
short *TextureBase_GetAllID();// 取得材質資料庫所有的材質ID

// unused // 最終沒用到，但很好用！
void TextureBase_DeleteTexture(const char* textureName); //刪除指定圖片
void IDtoNameBase_SortBaseByID(); //依照方塊編號大小排序資料庫
SDL_Texture *TextureBase_GetTextureName(char* textureName);// 依方塊名稱取得圖片 

#endif