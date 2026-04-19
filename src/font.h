#ifndef _NOTEPAD_FONT_H
#define _NOTEPAD_FONT_H

#include "std.h"

#define FONT_WIDTH 128
#define FONT_HEIGHT 82
#define FONT_ADVANCE 9
#define FONT_LINE_HEIGHT 11

typedef struct {
    int ox, oy, tx, ty, w, h;
} font_glyph_t;

void font_init(void);
void font_cleanup(void);

const uint32_t *font_get_bitmap(void);
bool font_get_glyph(char c, font_glyph_t *g);

#endif /* _NOTEPAD_FONT_H */
