File data could be stored in a singly-linked list of chunks each the size of one or multiple memory pages allocated by VirtualAlloc.

```c
struct Chunk {
    prev *Chunk;
    uint64_t size;
    uint64_t capacity;
    uint8_t data[1];
}
```

The program would hold onto the most recently allocated chunk.

Once chunk is allocated, it can only be extended by calling `VirtualAlloc(&chunk + chunk.cap, ...)`. If that call fails, new chunk is allocated at at unknown address.

The current contents of a file are represented by an array of file pieces which fat pointers into the file data chunk list.

```c
struct FilePiece {
    void *data;
    uint64_t size;
}
```