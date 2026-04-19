#include "tga.h"

void tga_save(LPCWSTR path, int w, int h, uint32_t *pixels) {
    HANDLE file;
    HANDLE fmap;
    LPVOID ptr;

    file = CreateFileW(
        path,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        0,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING,
        0
    );
    if (file == INVALID_HANDLE_VALUE) {
        debug_log(DEBUG_ERROR, L"%d", GetLastError());
    }
    ASSERT(file != INVALID_HANDLE_VALUE);

    fmap = CreateFileMappingW(
        file,
        0,
        PAGE_READWRITE,
        0,
        w * h * 4 + 18,
        0
    );
    ASSERT(fmap);

    ptr = MapViewOfFile(fmap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    ASSERT(ptr);

    ((uint8_t*)ptr)[2] = 2;
    ((uint16_t*)((uint8_t*)ptr + 12))[0] = (uint16_t)w;
    ((uint16_t*)((uint8_t*)ptr + 12))[1] = (uint16_t)h;
    ((uint8_t*)ptr)[16] = 32;
    ((uint8_t*)ptr)[17] = 32;

    memcpy((uint8_t*)ptr + 18, pixels, w * h * 4);

    UnmapViewOfFile(ptr);
    CloseHandle(fmap);
    CloseHandle(file);

    debug_log(DEBUG_VERBOSE, L"saved \"%s\"", path);
}
