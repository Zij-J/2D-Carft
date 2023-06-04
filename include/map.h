/* map大陣列、地圖file、地圖cursor 與 相關 function (file I/O：map file)*/
#ifndef MAP
#define MAP

#include "../include/basicSetting.h"
#define TOTAL_BLOCK_NUMBER_IN_WIDTH 18
#define TOTAL_BLOCK_NUMBER_IN_HEIGHT (TOTAL_BLOCK_NUMBER_IN_WIDTH * WINDOW_WIDTH / WINDOW_HEIGHT)
#define BLOCK_WIDTH (WINDOW_WIDTH / TOTAL_BLOCK_NUMBER_IN_WIDTH) // 用寬能放幾個推算方塊的邊長
#define ARRAY_MAP_WIDTH 30

extern short map[4][ARRAY_MAP_WIDTH][ARRAY_MAP_WIDTH]; //4 big array
extern SDL_position camera_xy;//map_UpLeft_abs_pos
extern SDL_position arr_xy[4];//big_arr_abs_pos
extern SDL_position cursor_xy;//cursor_abs_pos

// unfinished
//bool Map_isInput(SDL_Event event);//not need now //merge in Map_EditBlock()
//short **Map_GetTotalMap(); //altered by using extern array


// finished
void Map_Init();
private void Map_Finput(FILE *fp, int n, int x, int y);
void Map_Clear();
void Map_UpdateMaps(SDL_Event event);
void swap(int* a, int* b);
private void Map_Foutput(FILE *fp, int n, char* FileName);
void Map_EditBlock(SDL_Event event);//need to get Hotbar current block number
SDL_position Map_GetUpLeftCornerPosition(SDL_Event event);
SDL_position Map_GetCursorPosition(SDL_Event event);
SDL_size Map_GetBlockSize();
SDL_size Map_GetCursorSize();
private char* file_name(int x, int y);

#endif
