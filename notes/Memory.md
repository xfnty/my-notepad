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

# Horizontal Breaks

Horizontal breaks are character offsets inside each line that indicate which character begins and is inside, even if partially, a vertical ruler / horizontal guide. This is useful for constraining the multiple line query not just by the number of lines on the vertical axis but also by the number of breaks (which are spaced evenly) on the horizontal axis.

This is a way of asking for a 2-dimensional view into a file which is what the renderer needs in the end.

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

# File API

The file module will hold a single file at a time. It will allow reading its contents line by line and inserting or deleting text into/from lines.

Ideally, the program should support reading files of *any* size which means only a small portion of the file will be present in memory. The file still needs to be read completely at startup to find all line breaks and calculate the lower bound of the text. This initial skimming through a file must be done asynchronously. After that the program will have an array of all line breaks in the file. The issue is, it may not be able to keep this array in memory, if the file contained 10 gigs of newlines. But I think I should leave this feature and non-monospaced fonts for later.

So, first part of the interface - reading lines. It will be primarily used by the renderer which will always request multiple consequtive lines at a time. Moreover it would ideally need only a short (100-150 characters) span into each line. Long lines can be split into multiple pieces.