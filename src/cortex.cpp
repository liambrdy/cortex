#include <stdio.h>

#include "cortex_platform.h"
#include "cortex_math.h"

struct memory_arena
{
    size_t size;
    uint8 *base;
    size_t used;
};

void InitializeArena(memory_arena *arena, size_t size, uint8 *storage)
{
    arena->base = storage;
    arena->size = size;
    arena->used = 0;
}

void *PushSize_(memory_arena *arena, size_t size)
{
    Assert(arena->size > size + arena->used);
    uint8 *ptr = arena->base + arena->used;
    arena->used += size;
    return ptr;
}

#define PushStruct(arena, type) (type *)PushSize_(arena, sizeof(type))
#define PushArray(arena, count, type) (type *)PushSize_(arena, (count) * sizeof(type))

struct game_state
{
    memory_arena permanentArena;
};

extern "C" GAME_UPDATE(GameUpdate)
{
    game_state *gameState = (game_state *)memory->permanentStorage;
    if (!memory->isInitialized)
    {
        InitializeArena(&gameState->permanentArena, memory->permanentStorageSize - sizeof(game_state), (uint8 *)memory->permanentStorage + sizeof(game_state));

        memory->isInitialized = true;
    }
}