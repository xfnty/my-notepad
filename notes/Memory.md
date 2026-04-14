# File Data

Contains the initial contents of a file plus everything the user has ever entered into it while editing.

It would be allocated in a chunk allocator - singly-linked list of chunks of arbitrary size (probably multiples of a page size). Each chunk can only be extended in place, it is *pinned* in Rust terms. Allocations will not cross chunk boundaries which will waste memory, but most likely not much. he program would hold onto the most recently allocated chunk.

```c
struct ChunkHeader {
    ChunkHeader *prev;
    uint64_t size, capacity;
    uint8_t data[1];
};
```

# Line Breaks

Absolute line break offsets will be stored in a continuous array inside a growable arena allocator which could move memory.

This array has the same lifetime as File Data.

# File Pieces

The current contents of a file will be described by an array of pairs of views/spans into File Data and line break array.

```c
struct Piece {
    uint8_t *data_ptr;
    uint64_t data_size;
    uint64_t linebreaks_start;
    uint64_t linebreaks_count;
};
```