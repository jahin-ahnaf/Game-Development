#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <bits/stdc++.h>
using namespace std;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_FRect rec;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

    rec.w = 10;
    rec.h = 10;

    rec.x = 10;
    rec.y = 10;

    if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    float mouseX, mouseY;
    SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouseX, &mouseY);

    //Debug Mouse Position
    //cout << mouseX << ' ' << mouseY << endl;

    SDL_PumpEvents();
    const Uint8 *keyPress = (const Uint8*) SDL_GetKeyboardState(NULL);
    
    if (buttons){
        rec.x += (mouseX - rec.x)*0.005;
        rec.y += (mouseY - rec.y)*0.005;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rec);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    cout << "Program Terminated." << endl;
}
