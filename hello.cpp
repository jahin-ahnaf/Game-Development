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

    rec.w = 50;
    rec.h = 50;

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

    if (buttons & SDL_BUTTON_MASK(SDL_BUTTON_LEFT))
    {
        float speed = 500.0f; // Pixels per second (Constant Speed)

        float dx = mouseX - rec.x;
        float dy = mouseY - rec.y;

        // Calculate distance
        float distance = SDL_sqrtf(dx * dx + dy * dy);

        // Only move if not already there (prevent division by zero)
        if (distance > 0.0f)
        {
            // Normalize direction (-1 to 1) and multiply by constant speed & deltaTime
            rec.x += (dx / distance) * speed * deltaTime;
            rec.y += (dy / distance) * speed * deltaTime;
        }
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
