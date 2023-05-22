#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <io.h>
#include "./SDL2/SDL.h" 
#include "./SDL2/SDL_image.h"
#undef main
#define INIT_ARRAY_SIZE 1024
#define MAX_TEXTURES 100
#define SCR_N 8

// storedBlcokData
struct blockDataBase_Texture
{
    char *blockName;
    SDL_Texture *blockTexture;
    struct blockDataBase_Texture *next;
};
typedef struct blockDataBase_Texture storedBlock_Data;
// 可變動大小的 storedBlcokData array
struct storedBlockArrayStruct
{
    storedBlock_Data *array;
    int storedSize, maxSize;
    storedBlock_Data *head;    
};
typedef struct storedBlockArrayStruct storedBlock_ArrayAndSize;
typedef storedBlock_ArrayAndSize* storedBlock_DataBase;

void set_image(SDL_Rect *r,int x1,int y1,int w1,int h1){
    r->x=x1;    // x-coordinate of the destination rectangle
    r->y=y1;    // y-coordinate of the destination rectangle
    r->w=w1;    // width of the destination rectangle
    r->h=h1;    // height of the destination rectangle
}

// 初始化資料庫
void storedBlock_InitArray(storedBlock_DataBase* storedBlock_ArrayRecord)
{
    *storedBlock_ArrayRecord = (storedBlock_ArrayAndSize*)malloc(sizeof(storedBlock_ArrayAndSize));
    (*storedBlock_ArrayRecord)->array = (storedBlock_Data*)malloc(sizeof(storedBlock_Data) * INIT_ARRAY_SIZE);
    (*storedBlock_ArrayRecord)->storedSize = 0;
    (*storedBlock_ArrayRecord)->maxSize = INIT_ARRAY_SIZE;
    (*storedBlock_ArrayRecord)->head = NULL; 
}

char* duplicateString(const char* str)
{
    size_t length = strlen(str);
    char* duplicate = (char*)malloc(length + 1); 
    if (duplicate != NULL)
    {
        strcpy(duplicate, str);
    }
    return duplicate;
}

// Compare function for sorting textures by name
int compareTextures(const void* a, const void* b)
{
    const storedBlock_Data* textureA = (const storedBlock_Data*)a;
    const storedBlock_Data* textureB = (const storedBlock_Data*)b;
    return strcmp(textureA->blockName, textureB->blockName);
}
//從圖片資料夾匯入新增圖片並依性質排序
void storedBlock_AddTexture(storedBlock_DataBase storedBlock_ArrayRecord, const char *folderPath, SDL_Window *window, SDL_Renderer *renderer)
{
    DIR *dir;
    struct dirent *entry;
    char filePath[256];

    dir = opendir(folderPath);
    if (dir == NULL)
    {
        fprintf(stderr, "Failed to open directory: %s\n", folderPath);
        return;
    }

    int index = 0;  // Index for storing textures in the array

    while ((entry = readdir(dir)) != NULL)
    {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Check if the file has a ".png" extension
        if (strstr(entry->d_name, ".png") == NULL)
            continue;

        // Construct the full file path
        snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, entry->d_name);

        // Load the image as a surface
        SDL_Surface *surface = IMG_Load(filePath);
        if (!surface)
        {
            fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
            continue;
        }

        // Create a texture from the surface
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture)
        {
            fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
            SDL_FreeSurface(surface);
            continue;
        }

        // Extract the file name without extension
        char *fileName = entry->d_name;
        char *extension = strstr(fileName, ".png");
        if (extension != NULL)
            *extension = '\0';

        // Add the texture to the database
        storedBlock_ArrayRecord->array[index].blockName = duplicateString(fileName);
        storedBlock_ArrayRecord->array[index].blockTexture = texture;
        storedBlock_ArrayRecord->array[index].next = NULL;
        index++;

        if (index >= storedBlock_ArrayRecord->maxSize)
        {           
            storedBlock_ArrayRecord->maxSize *= 2;
            storedBlock_ArrayRecord->array = (storedBlock_Data *)realloc(storedBlock_ArrayRecord->array, sizeof(storedBlock_Data) * storedBlock_ArrayRecord->maxSize);
        }
    }    
    storedBlock_ArrayRecord->storedSize = index;

    // Sort the textures by name
    qsort(storedBlock_ArrayRecord->array, storedBlock_ArrayRecord->storedSize, sizeof(storedBlock_Data), compareTextures);
    closedir(dir);
}
// 載入圖片檔案
SDL_Texture *loadTexture(const char *filePath, SDL_Renderer *renderer)
{
    // Load the image as a surface
    SDL_Surface *surface = IMG_Load(filePath);
    if (!surface)
    {
        fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
        return NULL;
    }

    // Create a texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
    }

    SDL_FreeSurface(surface);

    return texture;
}

// 取得圖片
SDL_Texture* storedBlock_GetTexture(storedBlock_DataBase storedBlock_ArrayRecord, char* textureName)
{
    for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    {
        if (strcmp(storedBlock_ArrayRecord->array[i].blockName, textureName) == 0)
            return storedBlock_ArrayRecord->array[i].blockTexture;
    }
    storedBlock_Data* current = storedBlock_ArrayRecord->head;
    while (current != NULL)
    {
        if (strcmp(current->blockName, textureName) == 0)
            return current->blockTexture;
        current = current->next;
    }
    return NULL;
}
//刪除指定圖片
void storedBlock_DeleteTexture(storedBlock_DataBase storedBlock_ArrayRecord, const char* textureName)
{
    // Search for the texture with the specified name
    int index = -1;
    for (int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    {
        if (strcmp(storedBlock_ArrayRecord->array[i].blockName, textureName) == 0)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        // Remove the texture from the array
        SDL_DestroyTexture(storedBlock_ArrayRecord->array[index].blockTexture);
        free(storedBlock_ArrayRecord->array[index].blockName);
    
        for (int i = index; i < storedBlock_ArrayRecord->storedSize - 1; ++i)
        {
            storedBlock_ArrayRecord->array[i] = storedBlock_ArrayRecord->array[i + 1];
        }

        // Update the stored size
        storedBlock_ArrayRecord->storedSize--;
    }
}

void update_screen(SDL_Renderer* renderer, SDL_Rect destRect, int n_img, SDL_Texture **imageTexture, SDL_Texture *frameTexture, int frame[], int map[SCR_N][SCR_N])
{
    // Clear renderer
    SDL_RenderClear(renderer);
    for(int i=0; i<SCR_N; i++){
        for(int j=0; j<SCR_N; j++){
            // Set the destination rectangle
            set_image(&destRect, 80*j, 80*i, 80, 80);
            // Get the current image index
            int currentImageIndex = map[i][j];
            // Copy texture to renderer
            SDL_RenderCopy(renderer, imageTexture[map[i][j]], NULL, &destRect);
            // Update the current image index for the next position
            currentImageIndex = (currentImageIndex + 1) % n_img;
        }
    }
    set_image(&destRect, 80*frame[0], 80*frame[1], 80, 80);
    SDL_RenderCopy(renderer, frameTexture, NULL, &destRect);
    // Update renderer
    SDL_RenderPresent(renderer);
}

// 清除資料庫
void storedBlock_ClearDataBase(storedBlock_DataBase storedBlock_ArrayRecord)
{
    // 清除材質
    storedBlock_Data* current = storedBlock_ArrayRecord->head;
    while (current != NULL)
    {
        storedBlock_Data* nextBlock = current->next;
        SDL_DestroyTexture(current->blockTexture);
        free(current->blockName);
        free(current);
        current = nextBlock;
    }
    
    storedBlock_ArrayRecord->head = NULL;
    storedBlock_ArrayRecord->storedSize = 0;
    storedBlock_ArrayRecord->maxSize = 0;
    free(storedBlock_ArrayRecord->array);
    free(storedBlock_ArrayRecord);

    for(int i = 0; i < storedBlock_ArrayRecord->storedSize; ++i)
    {
        SDL_Texture *nowBlockTexture = storedBlock_ArrayRecord->array[i].blockTexture;
        SDL_DestroyTexture(nowBlockTexture);
    }    
}

int main(int argc, char* argv[])
{            
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        fprintf(stderr, "Failed to initialize SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }
    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL Image Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 640, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    int map[SCR_N][SCR_N];
    int currentImageIndex = 0;
    int frame[2]={0, 0};    

    storedBlock_DataBase storedBlockDB = malloc(sizeof(storedBlock_ArrayAndSize));
    storedBlock_InitArray(&storedBlockDB);
    
    // Load image as texture
    storedBlock_AddTexture(storedBlockDB, "block_pictures", window, renderer);

    int n_img = storedBlockDB->storedSize;

    SDL_Texture* imageTexture[MAX_TEXTURES];
    SDL_Texture* frameTexture = storedBlock_GetTexture(storedBlockDB, "frame");

    for (int i = 0; i < n_img; i++) {
        char filePath[100];
        sprintf(filePath, "./block_pictures/%s.png", storedBlockDB->array[i].blockName);
        imageTexture[i] = storedBlock_GetTexture(storedBlockDB, storedBlockDB->array[i].blockName);

        if (!imageTexture[i]) {
            fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
            storedBlock_ClearDataBase(storedBlockDB);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            return 1;
        }
    }
    //顯示圖片
    int pictureIndex = 0;
    for(int i=0; i<SCR_N; i++){
        for(int j=0; j<SCR_N; j++) {
            map[i][j] = pictureIndex % storedBlockDB->storedSize; 
            pictureIndex++;
        }
    }
    
    //SDL_RenderClear(renderer);
    SDL_Rect destRect;
    update_screen(renderer, destRect, n_img, imageTexture, frameTexture, frame, map);

    SDL_Event event;
        while(SDL_WaitEvent(&event)){
            if (event.type == SDL_QUIT) {//quit
                break;
            }
            else if(event.type == SDL_MOUSEMOTION){//move mouse
                int x_mouse, y_mouse;
                SDL_GetMouseState(&x_mouse,&y_mouse);
                frame[0]=x_mouse/80, frame[1]=y_mouse/80;
                update_screen(renderer, destRect, n_img, imageTexture, frameTexture, frame, map);//update screen
            }
            else if(event.type == SDL_KEYDOWN){//press key
                if(event.key.keysym.sym == SDLK_w && frame[1]>0) frame[1]--;
                else if(event.key.keysym.sym == SDLK_a && frame[0]>0) frame[0]--;
                else if(event.key.keysym.sym == SDLK_s && frame[1]<SCR_N-1) frame[1]++;
                else if(event.key.keysym.sym == SDLK_d && frame[0]<SCR_N-1) frame[0]++;
                //printf("*code: %d*\n",event.key.keysym.sym);  //check press key's code
                update_screen(renderer, destRect, n_img, imageTexture, frameTexture, frame, map);//update screen
            }
        }
    //刪除指定圖片example_texture   
    storedBlock_DeleteTexture(storedBlockDB, "example_texture");

    // Destroy texture, renderer, and window
    for(int i=0; i<n_img; i++) {
        SDL_DestroyTexture(imageTexture[i]);
    }
    SDL_DestroyTexture(frameTexture);
    storedBlock_ClearDataBase(storedBlockDB);
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // Quit SDL_image and SDL
    IMG_Quit();
    SDL_Quit();
   
    return 0;

}

