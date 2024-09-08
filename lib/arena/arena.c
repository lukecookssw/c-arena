
#include "arena.h"

#include <stdlib.h>
#include <stddef.h>

// Function to initialize the arena
void arena_init(Arena *arena, size_t size) {
    arena->memory = malloc(size);
    if (arena->memory == NULL) {
        // Handle allocation failure
        exit(1);
    }
    arena->size = size;
    arena->offset = 0;
}

// Function to allocate memory from the arena
void *arena_alloc(Arena *arena, size_t size) {
    if (arena->offset + size > arena->size) {
        // Handle out-of-memory error
        return NULL;
    }
    void *ptr = (char *)arena->memory + arena->offset;
    arena->offset += size;
    return ptr;
}

// Function to reset the arena
void arena_reset(Arena *arena) {
    arena->offset = 0;
}

// Function to free the arena
void arena_free(Arena *arena) {
    free(arena->memory);
    arena->memory = NULL;
    arena->size = 0;
    arena->offset = 0;
}