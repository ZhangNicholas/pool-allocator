#pragma once
struct Chunk {
    /**
     * When a chunk is free, the `next` contains the
     * address of the next chunk in a list.
     *
     * When it's allocated, this space is used by
     * the user.
     */
    Chunk* next;
};