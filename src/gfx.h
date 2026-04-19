#ifndef _NOTEPAD_GFX_H
#define _NOTEPAD_GFX_H

#include "std.h"
#include "util.h"

#define GFX_TEXTURE_SIZE 512

typedef struct {
    float x, y, u, v;
} gfx_vertex_t;

void gfx_init(HWND hwnd);
void gfx_cleanup(void);

void gfx_set_size(int w, int h);

void gfx_clear(void);
void gfx_draw(const gfx_vertex_t *vertices, uint32_t count);
void gfx_present(void);

void gfx_update_texture(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t *pixels);
void gfx_dump_texture_tga(LPCWSTR path);

#endif /* _NOTEPAD_GFX_H */
