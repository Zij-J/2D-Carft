/* map大陣列(需存地圖上絕對位置)、地圖file、地圖cursor 與 相關 function (file I/O：map file)*/
#include "../include/basicSetting.h" // 在這邊 include 就ok！不然include map.h 會重複 inlude 到 basicSetting.h (但因為有寫預防措施所以其實ok，只是這樣比較統一)
#include "../include/render.h"
#include "../include/map.h"

// 要用的 Macro，我想改成這裡 (盡量讓 .h 檔愈簡潔愈好)
#define TOTAL_BLOCK_NUMBER_IN_WIDTH 18
#define TOTAL_BLOCK_NUMBER_IN_HEIGHT (TOTAL_BLOCK_NUMBER_IN_WIDTH * WINDOW_HEIGHT / WINDOW_WIDTH)
#define BLOCK_WIDTH (WINDOW_WIDTH / TOTAL_BLOCK_NUMBER_IN_WIDTH) // 用寬能放幾個推算方塊的邊長
#define ARRAY_MAP_WIDTH 30

// 用此紀錄是否有按滑鼠、按了左還是右 + 是否一直按著
struct pressMouseRecord
{
    bool isPressed;
    int buttonType;
} pressMouseRecord;

SDL_position arr_xy[4]; //big_arr_abs_pos (以方塊為單位)
SDL_position cursor_xy; //cursor_abs_pos
FILE *fp[4];
short map[4][ARRAY_MAP_WIDTH][ARRAY_MAP_WIDTH]; //4 big array
int arr_relative_pos[4]; //[0]: UpLeft, [1]: UpRight, [2]: DownLeft, [3]: DownRight, n: map[n]

// private 的 prototype 放這裡，一樣只是讓 .h 變簡潔！(畢竟 header file 的目的就是給其他 .c 用的 「介面」，介面愈簡單愈好)
private SDL_position Map_ChangePosToBlockPos(SDL_position originaPos);
private void Map_Finput(int n, int x, int y);
private void Map_Foutput(FILE *fp, int n, char* FileName);
private void swap(int* a, int* b); // 只有 map.h 會用？所以改成private
private int Map_GetNumRelative(SDL_position pos);
private char* file_name(int x, int y);

//map initialize when program start
public void Map_Init()
{
    //file input map // 因 file_name 用絕對座標，看來 Map_Finput 也是傳絕對座標！
    Map_Finput(0, 0, 0); 
    Map_Finput(1, 0, ARRAY_MAP_WIDTH); 
    Map_Finput(2, -ARRAY_MAP_WIDTH, 0);
    Map_Finput(3, -ARRAY_MAP_WIDTH, ARRAY_MAP_WIDTH);
    //set 4 array relative position
    arr_relative_pos[0]=3;
    arr_relative_pos[1]=1;
    arr_relative_pos[2]=2;
    arr_relative_pos[3]=0;
}

//map file input (fp[n], n, file_map_x, file_map_x)
private void Map_Finput(int n, int x, int y){
    //open file + save array absolute position (用全域變數存！)
    fp[n] = fopen(file_name(x,y), "r"); 
    if(fp[n]==NULL) fp[n] = fopen(file_name(x,y), "w+"); // 如果打不開，fopen 會回傳 null，代表要創新檔案
    arr_xy[n].x= x; // 因為傳絕對座標了，所以直接存
    arr_xy[n].y= y; // 因為傳絕對座標了，所以直接存
    //read file + save in array
    fseek(fp[n], 0, SEEK_SET);
    int a;
    if(fscanf(fp[n],"%d",&a)==EOF){//if nothing in file
        for(int i=0; i<ARRAY_MAP_WIDTH; i++){
            for(int j=0; j<ARRAY_MAP_WIDTH; j++){
                map[n][i][j]=NO_BLOCK_ID; //no block = -9
            }
        }
        if(y==0) // y == 0 才是水平的map
            for(int j=0; j<ARRAY_MAP_WIDTH; j++)
                map[n][TOTAL_BLOCK_NUMBER_IN_HEIGHT-1 -1][j]=/*GRASS_NUM*/1; // 地板比畫面最下面高一格，個人覺得比較好看！
    }
    else{//if file exist before
        fseek(fp[n], 0, SEEK_SET);
        for(int i=0; i<ARRAY_MAP_WIDTH; i++){
            for(int j=0; j<ARRAY_MAP_WIDTH; j++)
                fscanf(fp[n], "%hd ", &map[n][i][j]); // 一定要用 hd 去 read ，不知道為何不會強制轉型，導致程式直接結束，所以改正
        }
    }
}

//return "map/map_x_y.txt" // 我把地圖放到 map 資料夾了！用 "map/map_x_y.txt" 可在 map 資料夾下創地圖
private char* file_name(int x, int y){
    // 算檔案的x,y座標 // 看起來 map_x_y 的 (x,y) 是「array 左上絕對座標 /ARRAY_MAP_WIDTH」，所以加此段，配合之後 file_name(arr_xy[i].x, arr_xy[i].y) 呼叫法
    x /= ARRAY_MAP_WIDTH;
    y /= ARRAY_MAP_WIDTH;
    // get 檔案名
    char *name=malloc(20), tmp[7]="\0", negative='0';
    strcpy(name, "map/map_");
    if(x==0) strcat(name, "0");
    else{
        if(x<0) x=-x, negative='1';
        while(x>0){
            for(int i=6; i>0; i--) tmp[i]=tmp[i-1];
            tmp[0]= '0'+ (x%10);
            x/=10;
        }
        if(negative=='1') strcat(name, "-");
        strcat(name, tmp);
    }
    strcat(name, "_");
    negative='0';
    tmp[0]='\0';
    if(y==0) strcat(name, "0");
    else{
        if(y<0) y=-y, negative='1';
        while(y>0){
            for(int i=6; i>0; i--) tmp[i]=tmp[i-1];
            tmp[0]= '0'+ (y%10);
            y/=10;
        }
        if(negative=='1') strcat(name, "-"); // y 也有負的！
        strcat(name, tmp);
    }
    strcat(name, ".txt");
    return &name[0];
}

// at the end of program
public void Map_Clear()
{
    //file output 4 fp
    for(int i=0; i<4; i++)
        Map_Foutput(fp[i], i, file_name(arr_xy[i].x, arr_xy[i].y));
}

//map file output (fp[n], n, file_name)
private void Map_Foutput(FILE *fp, int n, char* FileName){
    fp = fopen(FileName, "w+"); //clear file
    fseek(fp, 0, SEEK_SET);
    for(int i=0; i<ARRAY_MAP_WIDTH; i++){
        for(int j=0; j<ARRAY_MAP_WIDTH; j++)
            fprintf(fp, "%hd\t", map[n][i][j]);
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// 是否有移動，移動才進行其他步驟 //not need now //merge in Map_EditBlock()
// public bool Map_isInput(SDL_Event event)
// {
    
// }

// 依輸入放置、刪除方塊
public void Map_EditBlock(SDL_Event event)
{
    // 偵測輸入
    if(pressMouseRecord.isPressed == true && event.type == SDL_MOUSEBUTTONUP) // 有按著滑鼠，放開就不執行
    {
        pressMouseRecord.isPressed = false;
        return ;
    }
    if(pressMouseRecord.isPressed == false) // 沒按著滑鼠 + 沒開始按，就不執行
    {
        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            pressMouseRecord.isPressed = true;
            pressMouseRecord.buttonType = event.button.button; // 之後一直按著時不會幫忙記是按住左還是右鍵，要自己記
        }
        else
            return ;  
    }

    // 要取得目前快捷欄選取的方塊編號
    // short NowChoseBlockID = HotBar_GetChosenBlockID();
    
    //check position
    SDL_position cur = cursor_xy; // 用 cursor_xy 就好(已含 camera 位置，是cursor的絕對位置)！我忘記移動與放置是分開的！所以做此改動
    int num_relative = Map_GetNumRelative(cur);
    //place block
    if (pressMouseRecord.isPressed){
        if(pressMouseRecord.buttonType==SDL_BUTTON_LEFT) //put down block
            map[num_relative][ arr_xy[num_relative].y-cur.y ][ cur.x-arr_xy[num_relative].x ]=/*GET_HOTBAR_BLOCK_NUMBER*/0; // 最高在上，cursor只會往下，所以要大減小：arr_xy[num_relative].y-cur.y
        else if(pressMouseRecord.buttonType==SDL_BUTTON_RIGHT) //del block
            map[num_relative][ arr_xy[num_relative].y-cur.y ][ cur.x-arr_xy[num_relative].x ]=NO_BLOCK_ID; //no block = -9
    }
}

// 取得 /BLOCK_WIDTH 後的左上角 position
private SDL_position Map_ChangePosToBlockPos(SDL_position originPos)
{
    // /BLOCK_WIDTH 後不一定是左上角座標(在x正y負時才是)，所以須依不同正負調整 // 先加再除，避免 0.幾 的訊息都變成0，就看不出正負
    if(originPos.x < 0) // x負，除下去會變成右，要減，變左
        originPos.x -= BLOCK_WIDTH;
    if(originPos.y > 0) // y正，除下去會變成下，要加，變上
        originPos.y += BLOCK_WIDTH;

    return (SDL_position){.x = originPos.x / BLOCK_WIDTH, .y = originPos.y / BLOCK_WIDTH};
}

// 取得在大陣列的區域編號
private int Map_GetNumRelative(SDL_position pos)
{
    int num_relative;
    if(pos.x < arr_xy[arr_relative_pos[3]].x){
        if(pos.y > arr_xy[arr_relative_pos[3]].y) num_relative=0;
        else num_relative=2;
    }
    else{
        if(pos.y > arr_xy[arr_relative_pos[3]].y) num_relative=1;
        else num_relative=3;
    }  
    return arr_relative_pos[num_relative]; // 取得區域編號(對應位置)，要取到區域本身，才會改到區域本身！
}

// file I/O map if camera is out of range // 多偵測四次比傳來傳去讓程式更複雜好
public void Map_UpdateMaps()
{
    //get camera_position
    SDL_position cam =  Map_ChangePosToBlockPos(Render_GetCameraPosition());
    
    //if it exceed 4 map range
    if(cam.x < arr_xy[arr_relative_pos[0]].x){ //Left out of range
        Map_Foutput(fp[arr_relative_pos[1]], arr_relative_pos[1], file_name(arr_xy[arr_relative_pos[1]].x, arr_xy[arr_relative_pos[1]].y));
        Map_Foutput(fp[arr_relative_pos[3]], arr_relative_pos[3], file_name(arr_xy[arr_relative_pos[3]].x, arr_xy[arr_relative_pos[3]].y));
        Map_Finput(arr_relative_pos[1], arr_xy[arr_relative_pos[1]].x-2*ARRAY_MAP_WIDTH, arr_xy[arr_relative_pos[1]].y);
        Map_Finput(arr_relative_pos[3], arr_xy[arr_relative_pos[3]].x-2*ARRAY_MAP_WIDTH, arr_xy[arr_relative_pos[3]].y);
        //change relation
        swap(&arr_relative_pos[0], &arr_relative_pos[1]);
        swap(&arr_relative_pos[2], &arr_relative_pos[3]);
    }
    else if(cam.x > arr_xy[arr_relative_pos[1]].x+ARRAY_MAP_WIDTH-TOTAL_BLOCK_NUMBER_IN_WIDTH -1){ //Right out of range // 在邊界的上一格就會用到此邊界(想想相機在方塊中間情形)，所以要 -1
        Map_Foutput(fp[arr_relative_pos[0]], arr_relative_pos[0], file_name(arr_xy[arr_relative_pos[0]].x, arr_xy[arr_relative_pos[0]].y));
        Map_Foutput(fp[arr_relative_pos[2]], arr_relative_pos[2], file_name(arr_xy[arr_relative_pos[2]].x, arr_xy[arr_relative_pos[2]].y));
        Map_Finput(arr_relative_pos[0], arr_xy[arr_relative_pos[0]].x+2*ARRAY_MAP_WIDTH, arr_xy[arr_relative_pos[0]].y);
        Map_Finput(arr_relative_pos[2], arr_xy[arr_relative_pos[2]].x+2*ARRAY_MAP_WIDTH, arr_xy[arr_relative_pos[2]].y);
        //change relation
        swap(&arr_relative_pos[0], &arr_relative_pos[1]);
        swap(&arr_relative_pos[2], &arr_relative_pos[3]);
    }
    else if(cam.y > arr_xy[arr_relative_pos[0]].y){ //Up out of range
        Map_Foutput(fp[arr_relative_pos[2]], arr_relative_pos[2], file_name(arr_xy[arr_relative_pos[2]].x, arr_xy[arr_relative_pos[2]].y));
        Map_Foutput(fp[arr_relative_pos[3]], arr_relative_pos[3], file_name(arr_xy[arr_relative_pos[3]].x, arr_xy[arr_relative_pos[3]].y));
        Map_Finput(arr_relative_pos[2], arr_xy[arr_relative_pos[2]].x, arr_xy[arr_relative_pos[2]].y+2*ARRAY_MAP_WIDTH);
        Map_Finput(arr_relative_pos[3], arr_xy[arr_relative_pos[3]].x, arr_xy[arr_relative_pos[3]].y+2*ARRAY_MAP_WIDTH);
        //change relation
        swap(&arr_relative_pos[0], &arr_relative_pos[2]);
        swap(&arr_relative_pos[1], &arr_relative_pos[3]);
    }
    else if(cam.y < arr_xy[arr_relative_pos[2]].y-ARRAY_MAP_WIDTH+TOTAL_BLOCK_NUMBER_IN_HEIGHT +1){ //Down out of range // 在邊界的上一格就會用到此邊界(想想相機在方塊中間情形)，所以要 +1
        Map_Foutput(fp[arr_relative_pos[0]], arr_relative_pos[0], file_name(arr_xy[arr_relative_pos[0]].x, arr_xy[arr_relative_pos[0]].y));
        Map_Foutput(fp[arr_relative_pos[1]], arr_relative_pos[1], file_name(arr_xy[arr_relative_pos[1]].x, arr_xy[arr_relative_pos[1]].y));
        Map_Finput(arr_relative_pos[0], arr_xy[arr_relative_pos[0]].x, arr_xy[arr_relative_pos[0]].y-2*ARRAY_MAP_WIDTH);
        Map_Finput(arr_relative_pos[1], arr_xy[arr_relative_pos[1]].x, arr_xy[arr_relative_pos[1]].y-2*ARRAY_MAP_WIDTH);
        swap(&arr_relative_pos[0], &arr_relative_pos[2]);
        swap(&arr_relative_pos[1], &arr_relative_pos[3]);
    }
}

// up: U, down: D, left: L, right: R
private void swap(int* a, int* b){
    int tmp=*a;
    *a=*b;
    *b=tmp;
}

// 移動滑鼠 or 按下按鈕(移動camera)時，更新 cursor 位置 // 因加速度的相機，所以完全不按也會移動，因此要不斷更新 cursor 位置
public void Map_MoveCursor(SDL_Event event){  
    SDL_position camera_xy = Render_GetCameraPosition();
    SDL_GetMouseState(&(cursor_xy.x), &(cursor_xy.y));
    cursor_xy.x= (camera_xy.x + cursor_xy.x); // 讀現有的 camera_xy 就好！
    cursor_xy.y= (camera_xy.y -cursor_xy.y); // 原本以下為正，要相反！
    cursor_xy = Map_ChangePosToBlockPos(cursor_xy); // 最後變回 /BLOCK_WIDTH 座標
}

// 取得要顯示的畫面部分地圖 (需準備buffer存)
public void Map_GetShowedMapData(short ***mapInWindow, SDL_size *totalBlockNumberInWindow)
{
    // 先幫分配記憶體 (因為是給 static 變數，所以在整個程式結束時編譯器應該會幫忙free)
    if(*mapInWindow == NULL)
    {
        *mapInWindow = (short **)malloc(sizeof(short *) * (TOTAL_BLOCK_NUMBER_IN_HEIGHT +1));
        for(int i = 0; i < TOTAL_BLOCK_NUMBER_IN_HEIGHT +1; ++i)
            (*mapInWindow)[i] = (short *)malloc(sizeof(short) * (TOTAL_BLOCK_NUMBER_IN_WIDTH +1));
    }
    
    // 取地圖
    SDL_position startBlockPosition =  Map_ChangePosToBlockPos(Render_GetCameraPosition());
    SDL_position nowBlockPosition = startBlockPosition;
    for(int y = 0; y < TOTAL_BLOCK_NUMBER_IN_HEIGHT +1; ++y) // 需多顯示一個，避免cmaera在中間
    {
        nowBlockPosition.x = startBlockPosition.x;
        for(int x = 0; x < TOTAL_BLOCK_NUMBER_IN_WIDTH +1; ++x) // 需多顯示一個，避免cmaera在中間
        {
            // 取得當下座標的編號、存入回傳的地圖
            int num_relative = Map_GetNumRelative(nowBlockPosition);
            (*mapInWindow)[y][x] = map[num_relative][ arr_xy[num_relative].y - nowBlockPosition.y ][ nowBlockPosition.x - arr_xy[num_relative].x ];
            // 往右到下一 column
            nowBlockPosition.x += 1;
        }
        // 往下到下一 row
        nowBlockPosition.y -= 1;
    }

    // 取方塊數量
    *totalBlockNumberInWindow = (SDL_size){.width = TOTAL_BLOCK_NUMBER_IN_WIDTH +1, .height = TOTAL_BLOCK_NUMBER_IN_HEIGHT +1};
}


// 這是取得要顯示大陣列最左上角的絕對座標用的！ (原本的移動改到 render 的 Render_MoveCamera())
public SDL_position Map_GetUpLeftCornerPosition(){
    SDL_position camera_xy =  Map_ChangePosToBlockPos(Render_GetCameraPosition());
    SDL_position return_xy = (SDL_position){.x = camera_xy.x, camera_xy.y}; // 就是 get camera 所在方塊的左上角座標
    return return_xy;
}

// 這是取得 cursor 絕對位置用的！ (原本的移動改到 Map_MoveCursor())
public SDL_position Map_GetCursorPosition(){
    return cursor_xy;   
}

public SDL_size Map_GetBlockSize(){
    return (SDL_size){.height=BLOCK_WIDTH, .width=BLOCK_WIDTH};
}

public SDL_size Map_GetCursorSize(){
    return (SDL_size){.height=BLOCK_WIDTH, .width=BLOCK_WIDTH};
}