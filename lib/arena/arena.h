#ifndef ARENA_H
#define ARENA_H

#include <stdio.h>


typedef struct Arena
{
     void *memory;      // Pointer to the start of the allocated memory
    size_t size;        // Total size of the allocated memory
    size_t offset;      // Current offset within the allocated memory
} Arena;

void arena_init(Arena *arena, size_t size);
void *arena_alloc(Arena *arena, size_t size);
void arena_reset(Arena *arena);
void arena_free(Arena *arena);

#endif