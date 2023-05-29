/* 材質(背包)資料庫(紅黑樹？)、Sort、Search 與 相關function (file I/O：編號對應材質名 file、圖片資料夾)*/
#include "../include/basicSetting.h" // 要用的
#include "../include/texture.h" // 要放的

// 匯入編號對應材質名字 file
private void IDtoNameBase_Init()
{

}

// 清除 編號對應材質名字 file
private void IDtoNameBase_Clear()
{
    // 在這裡 file output!
}

// 初始化材質(背包)資料庫
public void Texture_Init()
{
    // 要匯入編號對應材質名字 file
    // IDtoNameBase_Init(); 

    // 還有圖片資料夾的材質

    // 用完 編號對應材質名字DataBase ，要清除
    // IDtoNameBase_Clear();
}

// 取得材質資料庫所有的材質ID // 我不知道要用什麼回傳，所以暫用short array
public short *TextureBase_GetAllID()
{
    // 要如何回傳所有ID要確定
}

// 依搜尋文字，找找看是否有方塊是此名字，回傳是否找到
public bool TextureBase_isFindBlockBySearchWords()
{
    // 要匯入編號對應材質名字 file 到 編號對應材質名 array (因為這個功能不會常用，所以用了再 Init 就好，不用一直存著)
    // IDtoNameBase_Init();

    // 要記錄著找到的編號，但不用回傳
}

// 把搜尋到的方塊編號回傳
public short TextureBase_GetSearchedBlockID()
{
    // 回傳完，此 DataBase 就沒用了，在此清除
    // IDtoNameBase_Clear();
}