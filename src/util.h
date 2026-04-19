#ifndef _NOTEPAD_UTIL_H
#define _NOTEPAD_UTIL_H

#include "std.h"

#define MIN(_a, _b) (((_a) < (_b)) ? (_a) : (_b))
#define MAX(_a, _b) (((_a) > (_b)) ? (_a) : (_b))
#define CLIP(_v, _min, _max) (MAX(MIN((_v), (_max)), (_min)))
#define COUNTOF(_a) (sizeof(_a)/sizeof((_a)[0]))

typedef struct {
    float x, y;
} vec2_t;

#define span_t(_T) struct { _T *ptr; uint64_t size; }

#endif /* _NOTEPAD_UTIL_H */
