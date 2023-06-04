/* map大陣列(需存地圖上絕對位置)、地圖file、地圖cursor 與 相關 function (file I/O：map file)*/
#include "../include/basicSetting.h"
#include "../include/map.h"


SDL_position camera_xy={.x=0, .y=0};
SDL_position arr_xy[4];
SDL_position cursor_xy;


FILE *fp[4];
short map[4][ARRAY_MAP_WIDTH][ARRAY_MAP_WIDTH]; //4 big array
int arr_relative_pos[4]; //[0]: UpLeft, [1]: UpRight, [2]: DownLeft, [3]: DownRight, n: map[n]

//map initialize when program start
public void Map_Init()
{
    //file input map
    Map_Finput(fp[0], 0, 0, 0);
    Map_Finput(fp[1], 1, 0, 1);
    Map_Finput(fp[2], 2, -1, 0);
    Map_Finput(fp[3], 3, -1, 1);
    //set 4 array relative position
    arr_relative_pos[0]=3;
    arr_relative_pos[1]=1;
    arr_relative_pos[2]=2;
    arr_relative_pos[3]=0;
}

//map file input (fp[n], n, file_map_x, file_map_x)
private void Map_Finput(FILE *fp, int n, int x, int y){
    //open file + save array absolute position
    fp = fopen(file_name(x,y), "r");
    if(fp==NULL) fp = fopen(file_name(x,y), "w+");
    arr_xy[n].x= x*ARRAY_MAP_WIDTH;
    arr_xy[n].y= y*ARRAY_MAP_WIDTH;
    //read file + save in array
    fseek(fp, 0, SEEK_SET);
    int a;
    if(fscanf(fp,"%d",&a)==EOF){//if nothing in file
        for(int i=0; i<ARRAY_MAP_WIDTH; i++){
            for(int j=0; j<ARRAY_MAP_WIDTH; j++){
                map[n][i][j]=-9; //no block = -9
            }
        }
        if(x==0)
            for(int j=0; j<ARRAY_MAP_WIDTH; j++)
                map[n][TOTAL_BLOCK_NUMBER_IN_HEIGHT-1][j]=/*GRASS_NUM*/1;
    }
    else{//if file exist before
        fseek(fp, 0, SEEK_SET);
        for(int i=0; i<ARRAY_MAP_WIDTH; i++){
            for(int j=0; j<ARRAY_MAP_WIDTH; j++)
                fscanf(fp, "%d", &map[n][i][j]);
        }
    }
}

// at the end of program
void Map_Clear()
{
    //file output 4 fp
    for(int i=0; i<4; i++)
        Map_Foutput(fp[i], i, file_name(arr_xy[i].x, arr_xy[i].y));
}

// file I/O map if camera is out of range
void Map_UpdateMaps(SDL_Event event)
{
    // SDL_position cameraPos = Render_GetCameraPosition();

    //get camera_position
    SDL_position cam = Map_GetUpLeftCornerPosition(event);
    //if it exceed 4 map range
    if(cam.x < arr_xy[arr_relative_pos[0]].x){ //Left out of range
        Map_Foutput(fp[arr_relative_pos[1]], arr_relative_pos[1], file_name(arr_xy[arr_relative_pos[1]].x, arr_xy[arr_relative_pos[1]].y));
        Map_Foutput(fp[arr_relative_pos[3]], arr_relative_pos[3], file_name(arr_xy[arr_relative_pos[3]].x, arr_xy[arr_relative_pos[3]].y));
        Map_Finput(fp[arr_relative_pos[1]], arr_relative_pos[1], arr_xy[arr_relative_pos[1]].x-2, arr_xy[arr_relative_pos[1]].y);
        Map_Finput(fp[arr_relative_pos[3]], arr_relative_pos[3], arr_xy[arr_relative_pos[3]].x-2, arr_xy[arr_relative_pos[3]].y);
        //change relation
        swap(&arr_relative_pos[0], &arr_relative_pos[1]);
        swap(&arr_relative_pos[2], &arr_relative_pos[3]);
    }
    else if(cam.x > arr_xy[arr_relative_pos[1]].x+ARRAY_MAP_WIDTH-TOTAL_BLOCK_NUMBER_IN_WIDTH){ //Right out of range
        Map_Foutput(fp[arr_relative_pos[0]], arr_relative_pos[0], file_name(arr_xy[arr_relative_pos[0]].x, arr_xy[arr_relative_pos[0]].y));
        Map_Foutput(fp[arr_relative_pos[2]], arr_relative_pos[2], file_name(arr_xy[arr_relative_pos[2]].x, arr_xy[arr_relative_pos[2]].y));
        Map_Finput(fp[arr_relative_pos[0]], arr_relative_pos[0], arr_xy[arr_relative_pos[0]].x+2, arr_xy[arr_relative_pos[0]].y);
        Map_Finput(fp[arr_relative_pos[2]], arr_relative_pos[2], arr_xy[arr_relative_pos[2]].x+2, arr_xy[arr_relative_pos[2]].y);
        //change relation
        swap(&arr_relative_pos[0], &arr_relative_pos[1]);
        swap(&arr_relative_pos[2], &arr_relative_pos[3]);
    }
    else if(cam.y > arr_xy[arr_relative_pos[0]].y){ //Up out of range
        Map_Foutput(fp[arr_relative_pos[2]], arr_relative_pos[2], file_name(arr_xy[arr_relative_pos[2]].x, arr_xy[arr_relative_pos[2]].y));
        Map_Foutput(fp[arr_relative_pos[3]], arr_relative_pos[3], file_name(arr_xy[arr_relative_pos[3]].x, arr_xy[arr_relative_pos[3]].y));
        Map_Finput(fp[arr_relative_pos[2]], arr_relative_pos[2], arr_xy[arr_relative_pos[2]].x, arr_xy[arr_relative_pos[2]].y+2);
        Map_Finput(fp[arr_relative_pos[3]], arr_relative_pos[3], arr_xy[arr_relative_pos[3]].x, arr_xy[arr_relative_pos[3]].y+2);
        //change relation
        swap(&arr_relative_pos[0], &arr_relative_pos[2]);
        swap(&arr_relative_pos[1], &arr_relative_pos[3]);
    }
    else if(cam.y < arr_xy[arr_relative_pos[2]].y+ARRAY_MAP_WIDTH-TOTAL_BLOCK_NUMBER_IN_HEIGHT){ //Down out of range
        Map_Foutput(fp[arr_relative_pos[0]], arr_relative_pos[0], file_name(arr_xy[arr_relative_pos[0]].x, arr_xy[arr_relative_pos[0]].y));
        Map_Foutput(fp[arr_relative_pos[1]], arr_relative_pos[1], file_name(arr_xy[arr_relative_pos[1]].x, arr_xy[arr_relative_pos[1]].y));
        Map_Finput(fp[arr_relative_pos[0]], arr_relative_pos[0], arr_xy[arr_relative_pos[0]].x, arr_xy[arr_relative_pos[0]].y-2);
        Map_Finput(fp[arr_relative_pos[1]], arr_relative_pos[1], arr_xy[arr_relative_pos[1]].x, arr_xy[arr_relative_pos[1]].y-2);
        //change relation
        swap(&arr_relative_pos[0], &arr_relative_pos[2]);
        swap(&arr_relative_pos[1], &arr_relative_pos[3]);
    }
}

// up: U, down: D, left: L, right: R
void swap(int* a, int* b){
    int tmp=*a;
    *a=*b;
    *b=tmp;
}

//map file output (fp[n], n, file_name)
private void Map_Foutput(FILE *fp, int n, char* FileName){
    fp = fopen(FileName, "w+"); //clear file
    fseek(fp, 0, SEEK_SET);
    for(int i=0; i<ARRAY_MAP_WIDTH; i++){
        for(int j=0; j<ARRAY_MAP_WIDTH; j++)
            fprintf(fp, "%d\t", &map[n][i][j]);
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// 依輸入放置、刪除方塊
public void Map_EditBlock(SDL_Event event)
{
    // 要取得目前快捷欄選取的方塊編號
    // short NowChoseBlockID = HotBar_GetChosenBlockID();

    //check position
    int num_relative;
    SDL_position cam = Map_GetUpLeftCornerPosition(event);
    if(cam.x < arr_xy[arr_relative_pos[3]].x){
        if(cam.y > arr_xy[arr_relative_pos[3]].y) num_relative=0;
        else num_relative=2;
    }
    else{
        if(cam.y > arr_xy[arr_relative_pos[3]].y) num_relative=1;
        else num_relative=3;
    }
    //if mouse clicked
    SDL_position cur = Map_GetCursorPosition(event);
    if (event.type == SDL_MOUSEBUTTONDOWN){
        if(event.button.button==SDL_BUTTON_LEFT) //put down block
            map[num_relative][cur.y-arr_xy[num_relative].y][cur.x-arr_xy[num_relative].x]=/*GET_HOTBAR_BLOCK_NUMBER*/0;
        else if(event.button.button==SDL_BUTTON_RIGHT) //del block
            map[num_relative][cur.y-arr_xy[num_relative].y][cur.x-arr_xy[num_relative].x]=-9; //no block = -9
    }
}

/* //not need now //merge in Map_EditBlock()
bool Map_isInput(SDL_Event event)
{

}

// 取得要顯示的畫面部分地圖 //altered by using extern array

// 清除地圖
public void Map_Clear()
{
    // 要 file output!
}

// 取得要顯示的畫面部分地圖

public short **Map_GetTotalgMap()
{
    // 這樣回傳就可以傳遞二維陣列 + 不用複製整個 Array
    // short array[2][2];
    // return &array;
}
*/

//when each PollEvent
SDL_position Map_GetUpLeftCornerPosition(SDL_Event event){
    if (event.type == SDL_KEYDOWN){ // press key
        if (event.key.keysym.sym == SDLK_w) camera_xy.y++;
        else if (event.key.keysym.sym == SDLK_a) camera_xy.x--;
        else if (event.key.keysym.sym == SDLK_s) camera_xy.y--;
        else if (event.key.keysym.sym == SDLK_d) camera_xy.x++;
    }
    return camera_xy;
}

//when each PollEvent
SDL_position Map_GetCursorPosition(SDL_Event event){
    if(event.type == SDL_MOUSEMOTION){
        SDL_GetMouseState(&(cursor_xy.x), &(cursor_xy.y));
        cursor_xy.x= Map_GetUpLeftCornerPosition(event).x + cursor_xy.x/BLOCK_WIDTH;
        cursor_xy.y= Map_GetUpLeftCornerPosition(event).y + (-cursor_xy.y)/BLOCK_WIDTH;
    }
    return cursor_xy;
}

SDL_size Map_GetBlockSize(){
    return (SDL_size){.height=BLOCK_WIDTH, .width=BLOCK_WIDTH};
}

SDL_size Map_GetCursorSize(){
    return (SDL_size){.height=BLOCK_WIDTH, .width=BLOCK_WIDTH};
}

//return "map_x_y.txt"
private char* file_name(int x, int y){
    char *name=malloc(20), tmp[7]="\0", negative='0';
    strcpy(name, "map_");
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
        strcat(name, tmp);
    }
    strcat(name, ".txt");
    printf("%s\n", name);
    return &name[0];
}

// 依輸入放置、刪除方塊
public void Map_EditBlock(SDL_Event event)
{
    // 要取得目前快捷欄選取的方塊編號
    // short NowChoseBlockID = HotBar_GetChosenBlockID();
}

// 依新的 camera 位置更新(增加、刪除)地圖
public void Map_UpdateMaps()
{
    // 取得 camera 座標
    // SDL_position cameraPos = Render_GetCameraPosition();

    // if 在大陣列外，file output 舊地圖，file input 新地圖 (如果有新地圖，需生成)
}
