/* 相機、與 renderer 相關 function */
#ifndef RENDER
#define RENDER

// unfinished
void Render_RenderBackpack();
void Render_RenderMapCursor();
void Render_RenderMap();
void Render_RenderHotbar();
void Render_RenderHotbarCursor();
void Render_RenderSearchMessage(bool isSuccessSearching);
void Render_MoveCamera(SDL_Event event);
SDL_position Render_GetCameraPosition();

// finished
void Render_RenderSearchWords();
void Render_RememberRenderer(SDL_Renderer* rememberedRenderer);
void SearchWords_GetInputWord(SDL_Event event);
void SearchWords_Init();
void SearchWords_Clear();
char *SearchWords_GetSearchingWords();

#endif