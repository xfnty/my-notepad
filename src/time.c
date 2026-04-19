#include "time.h"

#include "std.h"

static struct {
    LARGE_INTEGER start;
    LARGE_INTEGER freq;
} _s_time;

void time_init(void) {
    QueryPerformanceCounter(&_s_time.start);
    QueryPerformanceFrequency(&_s_time.freq);
}

double time_get(void) {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return (t.QuadPart - _s_time.start.QuadPart) / (double)_s_time.freq.QuadPart;
}
