#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <bits/stdc++.h>
using namespace std;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_FRect rec;
SDL_FRect slots[5];
bool grabbed = false;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

    rec.w = 50;
    rec.h = 50;

    rec.x = 10;
    rec.y = 10;

    slots[0].x = 740;
    slots[0].y = 10;
    for (int i = 0; i < 5; i++)
    {
        slots[i].w = 50;
        slots[i].h = 50;

        if (i > 0)
        {
            slots[i].x = slots[i - 1].x - 10 - slots[0].w;
            slots[i].y = 10;
        }
    }

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
    SDL_PumpEvents();
    float mouseX, mouseY;
    SDL_MouseButtonFlags buttons = SDL_GetMouseState(&mouseX, &mouseY);

    // Debug Mouse Position
    // cout << mouseX << ' ' << mouseY << endl;

    const Uint8 *keyPress = (const Uint8 *)SDL_GetKeyboardState(NULL);

    static Uint64 previousTime = 0;
    Uint64 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;

    if (mouseX <= rec.x + rec.w && mouseX >= rec.x && mouseY <= rec.y + rec.h && mouseY >= rec.y && buttons)
    {
        grabbed = true;
    }

    if (!buttons)
        grabbed = false;

    if (!grabbed)
    {
        for (auto &slot : slots)
        {
            if (rec.x + rec.w >= slot.x && rec.y + rec.h >= slot.y && rec.x <= slot.x + slot.w && rec.y <= slot.y + slot.h)
            {
                rec.x = slot.x;
                rec.y = slot.y;
            }
        }
    }

    if (grabbed && buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))
    {
        float t = 1.0f * deltaTime;

        rec.x = mouseX - rec.w / 2;
        rec.y = mouseY - rec.h / 2;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rec);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderRects(renderer, slots, 5);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    cout << "Program Terminated." << endl;
}
