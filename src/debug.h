#ifndef _NOTEPAD_DEBUG_H
#define _NOTEPAD_DEBUG_H

#include "std.h"

typedef enum {
    DEBUG_VERBOSE,
    DEBUG_INFO,
    DEBUG_WARNING,
    DEBUG_ERROR
} debug_log_level_t;

void debug_init(void);
void debug_cleanup(void);
void debug_log(debug_log_level_t level, LPCWSTR format, ...);
void debug_crash(LPCWSTR format, ...);

#define ASSERT(_x) do { \
        if (!(_x)) { \
            debug_crash( \
                L"Assertion failed: " L ## #_x L"\nAt %hs:%d", \
                PathFindFileNameA(__FILE__), \
                __LINE__ \
            ); \
        } \
    } while (0)

#endif /* _NOTEPAD_DEBUG_H */
