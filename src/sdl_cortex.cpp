#include <stdio.h>
#include <dlfcn.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "cortex_platform.h"

global bool globalRunning;

struct game_code
{
    void *libHandle;
    game_update *gameUpdate;
};

game_code LoadGameCode(const char *path)
{
    game_code code = {};
    code.libHandle = dlopen(path, RTLD_LAZY);
    if (code.libHandle)
    {
        code.gameUpdate = (game_update *)dlsym(code.libHandle, "GameUpdate");
    }

    return code;
}

void ProcessEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: {
                globalRunning = false;
            } break;
        }
    }
}

int main()
{
    game_code gameCode = LoadGameCode("build/libcortex.so");

    SDL_Window *window = SDL_CreateWindow("Cortex", 0, 0, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GL_CreateContext(window);
    if (window)
    {
        game_memory memory;
        memory.permanentStorageSize = Megabytes(64);
        memory.permanentStorage = malloc(memory.permanentStorageSize);

        Assert(memory.permanentStorage);

        globalRunning = true;
        while (globalRunning) {
            ProcessEvents();

            if (gameCode.gameUpdate)
            {
                gameCode.gameUpdate(&memory);
            }

            glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            SDL_GL_SwapWindow(window);
        }
    }

    return 0;
}