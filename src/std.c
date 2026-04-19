#include "std.h"

int _fltused=1;

void *memset(void *dst, int c, size_t count) {
    #ifdef _MSC_VER
        __stosb(dst, (uint8_t)c, count);
    #else
        size_t i;
        for (i = 0; i < count; i++) {
            ((uint8_t*)dst)[i] = (uint8_t)c;
        }
    #endif
    return dst;
}

void *memcpy(void *dst, const void *src, size_t count) {
    #ifdef _MSC_VER
        __movsb(dst, src, count);
    #else
        size_t i;
        for (i = 0; i < count; i++) {
            *((uint8_t*)dst + i) = *((uint8_t*)src + i);
        }
    #endif
    return dst;
}

uint16_t byteswap_u16(uint16_t v) {
    #ifdef _MSC_VER
        return (v >> 8) | (v << 8);
    #else
        return __builtin_bswap16(v);
    #endif
}

uint32_t byteswap_u32(uint32_t v) {
    #ifdef _MSC_VER
        return (v >> 24) | ((v & 0xFF00) << 8) | ((v & 0xFF0000) >> 8) | (v << 24);
    #else
        return __builtin_bswap32(v);
    #endif
}
