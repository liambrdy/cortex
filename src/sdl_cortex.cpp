#include <stdio.h>
#include <dlfcn.h>

#include <sys/stat.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "cortex_platform.h"

global bool globalRunning;

struct game_code
{
    void *libHandle;
    game_update *gameUpdate;
    time_t libraryMTime;
};

game_code LoadGameCode(const char *path)
{
    game_code code = {};

    struct stat statbuf = {};
    uint32 statbufResult = stat(path, &statbuf);
    if (statbufResult != 0)
    {
        printf("Failed to stat game code at %s\n", path);
        return code;
    }
    code.libraryMTime = statbuf.st_mtime;

    code.libHandle = dlopen(path, RTLD_LAZY);
    if (code.libHandle)
    {
        code.gameUpdate = (game_update *)dlsym(code.libHandle, "GameUpdate");
    }

    return code;
}

void UnloadGameCode(game_code *code)
{
    if (code->libHandle)
    {
        dlclose(code->libHandle);
        code->libHandle = 0;
    }

    code->gameUpdate = 0;
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
    const char *gameCodeLibraryPath = "build/libcortex.so";
    game_code gameCode = LoadGameCode(gameCodeLibraryPath);

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
            struct stat libraryStatbuf = {};
            stat(gameCodeLibraryPath, &libraryStatbuf);
            if (libraryStatbuf.st_mtime != gameCode.libraryMTime)
            {
                UnloadGameCode(&gameCode);
                gameCode = LoadGameCode(gameCodeLibraryPath);
            }

            ProcessEvents();

            if (gameCode.gameUpdate)
            {
                gameCode.gameUpdate(&memory);
            }

            // glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
            // glClear(GL_COLOR_BUFFER_BIT);

            SDL_GL_SwapWindow(window);
        }
    }

    return 0;
}