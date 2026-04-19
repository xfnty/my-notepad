#include "file.h"

#include "util.h"
#include "debug.h"

#define _FILE_READ_CHUNK_SIZE 4096

static struct {
    LPCWSTR path, name;
    HANDLE handle, mapping, heap;
    uint64_t size;
    char *text;
    uint64_t *lines;
    uint64_t line_count;
} _s_file;

void file_init(void) {
    _s_file.handle = INVALID_HANDLE_VALUE;
    _s_file.heap = GetProcessHeap();
    _s_file.name = L"Untitled";
}

void file_open(LPCWSTR path) {
    HANDLE new_handle;
    LARGE_INTEGER new_size;
    uint64_t i;

    new_handle = CreateFileW(
        path,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        0,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        0
    );
    if (new_handle == INVALID_HANDLE_VALUE) {
        debug_log(DEBUG_ERROR, L"failed to open \"%s\" (%d)", path, GetLastError());
        return;
    }

    if (!GetFileSizeEx(new_handle, &new_size)) {
        debug_log(DEBUG_ERROR, L"failed to get size of \"%s\" (%d)", path, GetLastError());
        CloseHandle(new_handle);
        return;
    }

    file_close();

    _s_file.path = path;
    _s_file.handle = new_handle;
    _s_file.size = new_size.QuadPart;
    _s_file.name = PathFindFileNameW(_s_file.path);

    _s_file.mapping = CreateFileMappingW(
        _s_file.handle,
        0,
        PAGE_READWRITE,
        (DWORD)(_s_file.size >> 32),
        (DWORD)(_s_file.size),
        0
    );
    ASSERT(_s_file.mapping);

    _s_file.text = MapViewOfFile(_s_file.mapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    ASSERT(_s_file.text);

    for (i = 0; i < _s_file.size; i++) {
        if (_s_file.text[i] == '\n') {
            if (!_s_file.lines) {
                _s_file.lines = HeapAlloc(
                    _s_file.heap,
                    HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
                    sizeof(_s_file.lines[0])
                );
            } else {
                _s_file.lines = HeapReAlloc(
                    _s_file.heap,
                    HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
                    _s_file.lines,
                    sizeof(_s_file.lines[0]) * (_s_file.line_count + 1)
                );
            }

            _s_file.lines[_s_file.line_count] = i;
            _s_file.line_count++;
        }
    }

    debug_log(
        DEBUG_VERBOSE,
        L"opened \"%s\" (%llu bytes, %llu lines)",
        _s_file.path,
        _s_file.size,
        _s_file.line_count
    );
}

void file_close(void) {
    if (_s_file.handle == INVALID_HANDLE_VALUE) {
        return;
    }

    UnmapViewOfFile(_s_file.text);
    CloseHandle(_s_file.mapping);
    CloseHandle(_s_file.handle);
    HeapFree(_s_file.heap, 0, _s_file.lines);

    _s_file.path = 0;
    _s_file.handle = INVALID_HANDLE_VALUE;
    _s_file.mapping = INVALID_HANDLE_VALUE;
    _s_file.size = 0;
    _s_file.lines = 0;
    _s_file.line_count = 0;
    _s_file.text = 0;
}

LPCWSTR file_get_name(void) {
    return _s_file.name;
}

uint64_t file_get_line_count(void) {
    return _s_file.line_count;
}

uint64_t file_get_line_length(uint64_t line_idx) {
    ASSERT(line_idx < _s_file.line_count);
    return (line_idx == _s_file.line_count - 1)
        ? (_s_file.size - _s_file.lines[line_idx])
        : (_s_file.lines[line_idx + 1] - _s_file.lines[line_idx]);
}

uint64_t file_get_line_span(uint64_t line_idx, uint64_t char_offset, uint64_t char_count, void *buffer) {
    uint64_t l;
    ASSERT(line_idx < _s_file.line_count);
    l = file_get_line_length(line_idx);
    if (char_offset >= l) {
        return 0;
    }
    char_count = l - char_offset;
    memcpy(buffer, _s_file.text + _s_file.lines[line_idx] + char_offset, char_count);
    return char_count;
}

/*

TODO:
- write 2 allocators:
  - one that would
    - not move previously allocated blocks
    - free deallocate everything all at once without actually freeing the memory
    it would store:
    - initial contents of the file
    - all of the insertions into the file
    - arrays of line break offsets
  - another that would growing arena allocator for storing array of file pieces
- create a struct for piece table that would initially allow
  - adding a piece
  - getting a view of a line by its number

*/
