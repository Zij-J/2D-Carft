/* 材質(背包)資料庫、Sort、Search 與 相關function (file I/O：編號對應材質名IDtoNameBase、圖片資料夾block_pictures)*/
#ifndef TEXTURE
#define TEXTURE

// finished
void TextureBase_Init();// 初始化資料庫
void TextureBase_Clear();// 清除資料庫
void TextureBase_GetAllBlock();//從圖片資料夾匯入圖片並依名稱排序
void TextureBase_DeleteTexture();//刪除指定圖片
void IDtoNameBase_Init();// 匯入編號對應材質名字 file
void IDtoNameBase_Clear();// 清除編號對應材質名字 file
short int * TextureBase_GetAllID();// 取得材質資料庫所有的材質ID
bool TextureBase_isFindBlockBySearchWords();// 依搜尋文字，找找看是否有方塊是此名字，回傳是否找到
short TextureBase_GetSearchedBlockID();// 把搜尋到的方塊編號回傳
SDL_Texture *loadTexture();// 載入圖片資料夾圖片
SDL_Texture *TextureBase_GetTextureName();// 依方塊名稱取得圖片
SDL_Texture *TextureBase_GetTextureByID();// 依編號取得圖片
SDL_Texture *TextureBase_GetTextureID();//依照方塊編號大小排序資料庫

#endif
