#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <bits/stdc++.h>
using namespace std;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int width = 800;
int height = 600;

const int CHUNK_HEIGHT = height;
const int CHUNK_WIDTH = width;

float cameraX = 0.0f;
float cameraY = 0.0f;

const float cameraSpeed = 5.0f;

struct ChunkCoord {
    int x,y;
    bool operator<(const ChunkCoord& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

struct Chunk {
    vector<SDL_FRect> trees;
};

map<ChunkCoord, Chunk> chunkCache;

Chunk generateChunk(int chunkX, int chunkY) {
    Chunk newChunk;

    // Use a hash combined of chunkX and chunkY as a seed
    // This ensures chunk (1,5) always generates the exact same trees
    std::mt19937 gen(chunkX * 49321 + chunkY * 96174); 
    std::uniform_int_distribution<> disX(0, CHUNK_WIDTH - 50);
    std::uniform_int_distribution<> disY(0, CHUNK_HEIGHT - 50);
    std::uniform_int_distribution<> disCount(5, 15); // 5 to 15 trees per chunk

    int treeCount = disCount(gen);
    for (int i = 0; i < treeCount; i++) {
        SDL_FRect tree;
        // Store coordinates relative to the world
        tree.x = (chunkX * CHUNK_WIDTH) + disX(gen);
        tree.y = (chunkY * CHUNK_HEIGHT) + disY(gen);
        tree.w = 50;
        tree.h = 50;
        newChunk.trees.push_back(tree);
    }
    return newChunk;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_CreateWindowAndRenderer("Hello World", width, height, SDL_WINDOW_RESIZABLE, &window, &renderer))
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

    const bool *key_states = SDL_GetKeyboardState(NULL);

    if (key_states[SDL_SCANCODE_A]){
        cameraX -= cameraSpeed;
    }
    if (key_states[SDL_SCANCODE_W]){
        cameraY -= cameraSpeed;
    }
    if (key_states[SDL_SCANCODE_D]){
        cameraX += cameraSpeed;
    }
    if (key_states[SDL_SCANCODE_S]){
        cameraY += cameraSpeed;
    }

    int currentChunkX = (int)floor((cameraX + width / 2.0f) / CHUNK_WIDTH);
    int currentChunkY = (int)floor((cameraY + height / 2.0f) / CHUNK_HEIGHT);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 34, 139, 34, SDL_ALPHA_OPAQUE);

    vector<SDL_FRect> visibleTrees;

    for (int xOffset = -1; xOffset <= 1; xOffset++) {
        for (int yOffset = -1; yOffset <= 1; yOffset++) {
            int targetChunkX = currentChunkX + xOffset;
            int targetChunkY = currentChunkY + yOffset;
            ChunkCoord coord = {targetChunkX, targetChunkY};

            // If chunk hasn't been generated yet, build it!
            if (chunkCache.find(coord) == chunkCache.end()) {
                chunkCache[coord] = generateChunk(targetChunkX, targetChunkY);
            }

            // 3. Collect and translate tree coordinates to screen space
            const Chunk& activeChunk = chunkCache[coord];
            for (const auto& tree : activeChunk.trees) {
                float screenX = tree.x - cameraX;
                float screenY = tree.y - cameraY;

                // Only render if it sits on our screen space
                if (screenX >= -50 && screenX <= width && screenY >= -50 && screenY <= height) {
                    visibleTrees.push_back({screenX, screenY, tree.w, tree.h});
                }
            }
        }
    }

    if (!visibleTrees.empty()) {
        SDL_RenderFillRects(renderer, visibleTrees.data(), visibleTrees.size());
    }

    if (chunkCache.size() > 100){
        chunkCache.clear();
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    cout << "Program Terminated." << endl;
}
