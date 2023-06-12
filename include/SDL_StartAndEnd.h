/* 函式 prototype，不能把此註解寫於SDL_InitializeAll之上，會把功能的註解蓋掉 */
#ifndef START_END_SDL
#define START_END_SDL

void SDL_InitializeAll(SDL_Window **window, SDL_Renderer **renderer);
void SDL_EndAll_StopProgram();
void SDL_SwitchPause();
bool SDL_GetPauseBool();

#endif