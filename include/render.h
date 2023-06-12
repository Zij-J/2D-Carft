/* 相機、與 renderer 相關 function */
#ifndef RENDER
#define RENDER

// unfinished


// finished
void Render_Init(SDL_Renderer* rememberedRenderer);
void Render_Clear();
void Render_RenderBackpack();
void Render_RenderBackpackCursor();
void Render_RenderMapCursor();
void Render_RenderHotbar();
void Render_RenderHotbarCursor();
void Render_RenderBackground();
void Render_RenderSearchNotify();
void Render_EnableSearchNotify(bool isSuccessFound);
void Render_ForceCloseSearchNotify();
void Render_RenderSearchMessage(bool isSuccessSearching);
void Render_RenderMap();
void Render_RenderSearchWords();
void Render_RenderPauseStuff();
void SearchWords_GetInputWord(SDL_Event event);
void SearchWords_Clear();
void Render_MoveCamera();
char *SearchWords_GetSearchingWords();
SDL_position Render_GetCameraPosition();


#endif