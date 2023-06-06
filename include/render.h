/* 相機、與 renderer 相關 function */
#ifndef RENDER
#define RENDER

// unfinished
void Render_RenderBackpack();
void Render_RenderMapCursor();
void Render_RenderHotbar();
void Render_RenderHotbarCursor();
void Render_RenderSearchMessage(bool isSuccessSearching);

// finished
void Render_RenderMap();
void Render_RenderSearchWords();
void Render_RememberRenderer(SDL_Renderer* rememberedRenderer);
void SearchWords_GetInputWord(SDL_Event event);
void SearchWords_Init();
void SearchWords_Clear();
void Render_MoveCamera(SDL_Event event);
char *SearchWords_GetSearchingWords();
SDL_position Render_GetCameraPosition();

#endif