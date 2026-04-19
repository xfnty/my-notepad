#ifndef _NOTEPAD_CONFIG_H
#define _NOTEPAD_CONFIG_H

#include "std.h"

typedef struct {
    LPCWSTR exe;
    LPCWSTR filepath;
} config_t;

void config_init(void);
void config_cleanup(void);
const config_t *config_get(void);

#endif /* _NOTEPAD_CONFIG_H */
