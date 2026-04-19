#include "ui.h"

#include "gfx.h"
#include "std.h"
#include "file.h"
#include "font.h"
#include "time.h"
#include "util.h"
#include "debug.h"
#include "config.h"
#include "resources.h"

#define _UI_WNDCLASS_NAME          L"xfnty.mynotepad"
#define _UI_DRWNDCLASS_NAME        L"xfnty.mynotepad.drwnd"
#define _UI_TITLE_PLACEHOLDER      L"Untitled - My Notepad"

static struct {
    HINSTANCE hinst;
    WNDCLASSEXW wndclass;
    HWND wnd;
    POINT wndsize;
    HMENU menu;
    HWND statusbar;
    bool statusbar_visible;
    int statusbar_height;
    WNDCLASSEXW drwndclass;
    HWND drwnd;
    int64_t line;
    SCROLLINFO vscrollbar_info;
} _s_ui;

static void _ui_init_wnd(void);
static void _ui_init_drwnd(void);
static LRESULT _ui_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
static LRESULT _ui_drwndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
static void _ui_handle_menu_command(int id);
static void _ui_show_statusbar(bool show);
static void _ui_relayout(void);
static void _ui_start_new_instance(void);
static void _ui_repaint_drwnd(void);
static void _ui_dump_textures(void);

void ui_init(void) {
    uint32_t black_texture[4];

    _s_ui.hinst = (HINSTANCE)GetModuleHandleW(NULL);

    _ui_init_wnd();
    _ui_init_drwnd();

    gfx_init(_s_ui.drwnd);
    font_init();

    gfx_update_texture(0, 0, FONT_WIDTH, FONT_HEIGHT, font_get_bitmap());
    black_texture[0] = 0xFF000000;
    black_texture[1] = 0xFF000000;
    black_texture[2] = 0xFF000000;
    black_texture[3] = 0xFF000000;
    gfx_update_texture(FONT_WIDTH + 1, 0, 2, 2, black_texture);
    gfx_dump_texture_tga(L"vram.tga");

    ShowWindow(_s_ui.wnd, SW_SHOW);
}

void ui_run(void) {
    MSG msg;
    while (GetMessageW(&msg, 0, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

void ui_cleanup(void) {
    font_cleanup();
    gfx_cleanup();
    ASSERT(DestroyWindow(_s_ui.wnd));
    ASSERT(UnregisterClassW(_s_ui.drwndclass.lpszClassName, _s_ui.wndclass.hInstance));
    ASSERT(UnregisterClassW(_s_ui.wndclass.lpszClassName, _s_ui.wndclass.hInstance));
}

void _ui_init_wnd(void) {
    WCHAR title[512];
    int status_widths[1];
    RECT statusbar_rect;

    InitCommonControls();

    _s_ui.wndclass.cbSize = sizeof(_s_ui.wndclass);
    _s_ui.wndclass.hInstance = _s_ui.hinst;
    _s_ui.wndclass.lpszClassName = _UI_WNDCLASS_NAME;
    _s_ui.wndclass.lpfnWndProc = _ui_wndproc;
    _s_ui.wndclass.hIcon = LoadIconW(_s_ui.wndclass.hInstance, (LPCWSTR)NOTEPAD_IDI_TASKBAR);
    _s_ui.wndclass.hIconSm = LoadIconW(_s_ui.wndclass.hInstance, (LPCWSTR)NOTEPAD_IDI_TITLEBAR);
    _s_ui.wndclass.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
    ASSERT(RegisterClassExW(&_s_ui.wndclass));

    _s_ui.menu = LoadMenuW(_s_ui.hinst, (LPCWSTR)MAKEINTRESOURCE(NOTEPAD_IDR_MENU));
    ASSERT(_s_ui.menu);

    wnsprintfW(title, COUNTOF(title), L"%s - My Notepad", file_get_name());

    _s_ui.wnd = CreateWindowExW(
        0,
        _s_ui.wndclass.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0,
        _s_ui.menu,
        _s_ui.wndclass.hInstance,
        0
    );
    ASSERT(_s_ui.wnd);

    _s_ui.statusbar = CreateWindowExW(
        0,
        STATUSCLASSNAMEW,
        0,
        WS_CHILD | SBARS_SIZEGRIP | SBARS_TOOLTIPS,
        0, 0,
        0, 0,
        _s_ui.wnd,
        0,
        _s_ui.wndclass.hInstance,
        0
    );
    ASSERT(_s_ui.statusbar);

    status_widths[0] = -1;
    SendMessageW(_s_ui.statusbar, SB_SETPARTS, COUNTOF(status_widths), (LPARAM)status_widths);
    SendMessageW(_s_ui.statusbar, SB_SETTEXTW, 0 | SBT_NOBORDERS, (LPARAM)L"Status bar");
    GetClientRect(_s_ui.statusbar, &statusbar_rect);
    _s_ui.statusbar_height = statusbar_rect.bottom;
    _ui_show_statusbar(true);
}

void _ui_init_drwnd(void) {
    _s_ui.drwndclass.cbSize = sizeof(_s_ui.drwndclass);
    _s_ui.drwndclass.style = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    _s_ui.drwndclass.hInstance = _s_ui.hinst;
    _s_ui.drwndclass.lpszClassName = _UI_DRWNDCLASS_NAME;
    _s_ui.drwndclass.lpfnWndProc = _ui_drwndproc;
    _s_ui.drwndclass.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_IBEAM);
    ASSERT(RegisterClassExW(&_s_ui.drwndclass));

    _s_ui.drwnd = CreateWindowExW(
        0,
        _s_ui.drwndclass.lpszClassName,
        0,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL,
        0, 0,
        0, 0,
        _s_ui.wnd,
        0,
        _s_ui.wndclass.hInstance,
        0
    );
    ASSERT(_s_ui.drwnd);

    _s_ui.vscrollbar_info.cbSize = sizeof(_s_ui.vscrollbar_info);
    _s_ui.vscrollbar_info.fMask = SIF_RANGE;
    _s_ui.vscrollbar_info.nMin = 0;
    _s_ui.vscrollbar_info.nMax = 0x7FFF;
    SetScrollInfo(_s_ui.drwnd, SB_VERT, &_s_ui.vscrollbar_info, true);
}

LRESULT _ui_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (hwnd != _s_ui.wnd)
        return DefWindowProcW(hwnd, msg, wp, lp);

    switch (msg) {
    case WM_CLOSE:
        ShowWindow(hwnd, SW_HIDE);
        PostQuitMessage(0);
        return TRUE;

    case WM_SIZE:
        _s_ui.wndsize.x = LOWORD(lp);
        _s_ui.wndsize.y = HIWORD(lp);
        _ui_relayout();
        break;

    case WM_COMMAND:
        _ui_handle_menu_command((int)wp);
        break;

    case WM_MOUSEWHEEL:
        if (!file_get_line_count()) {
            break;
        }

        _s_ui.line = CLIP(
            _s_ui.line - GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA * 3,
            0,
            (int64_t)file_get_line_count()
        );

        _s_ui.vscrollbar_info.fMask = SIF_POS;
        _s_ui.vscrollbar_info.nPos = (int)((float)_s_ui.line / file_get_line_count() * 0x7FFF);
        SetScrollInfo(_s_ui.drwnd, SB_VERT, &_s_ui.vscrollbar_info, true);

        _ui_repaint_drwnd();
        break;
    }

    return DefWindowProcW(hwnd, msg, wp, lp);
}

LRESULT _ui_drwndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (hwnd != _s_ui.drwnd)
        return DefWindowProcW(hwnd, msg, wp, lp);

    switch (msg) {
    case WM_SIZE:
        gfx_set_size(LOWORD(lp), HIWORD(lp));
        break;

    case WM_PAINT:
        _ui_repaint_drwnd();
        break;

    case WM_VSCROLL:
        if (LOWORD(wp) == SB_THUMBTRACK) {
            _s_ui.vscrollbar_info.fMask = SIF_POS;
            _s_ui.vscrollbar_info.nPos = HIWORD(wp);
            SetScrollInfo(_s_ui.drwnd, SB_VERT, &_s_ui.vscrollbar_info, true);

            _s_ui.line = (int64_t)((float)HIWORD(wp) / 0x7FFF * file_get_line_count());

            _ui_repaint_drwnd();
        }
        break;
    }

    return DefWindowProcW(hwnd, msg, wp, lp);
}

void _ui_handle_menu_command(int id) {
    switch (id) {
    case NOTEPAD_MENUITEM_TOGGLE_STATUSBAR:
        _ui_show_statusbar(!_s_ui.statusbar_visible);
        break;
    case NOTEPAD_MENUITEM_EXIT:
        PostQuitMessage(0);
        break;
    case NOTEPAD_MENUITEM_DUMP_TEXTURES:
        _ui_dump_textures();
        break;
    }
}

void _ui_show_statusbar(bool show) {
    if (_s_ui.statusbar_visible != show) {
        _s_ui.statusbar_visible = show;
        ModifyMenuW(
            _s_ui.menu,
            NOTEPAD_MENUITEM_TOGGLE_STATUSBAR,
            MF_BYCOMMAND | MFT_STRING | ((_s_ui.statusbar_visible) ? (MFS_CHECKED) : (MFS_UNCHECKED)),
            NOTEPAD_MENUITEM_TOGGLE_STATUSBAR,
            L"Status Bar"
        );
        ShowWindow(_s_ui.statusbar, ((_s_ui.statusbar_visible) ? (SW_SHOW) : (SW_HIDE)));
        _ui_relayout();
    }
}

void _ui_relayout(void) {
    SendMessageW(_s_ui.statusbar, WM_SIZE, 0, 0);
    MoveWindow(
        _s_ui.drwnd,
        0,
        0,
        _s_ui.wndsize.x,
        _s_ui.wndsize.y - ((_s_ui.statusbar_visible) ? (_s_ui.statusbar_height) : (0)),
        true
    );
}

void _ui_repaint_drwnd(void) {
    double s, e;
    char line[1024];
    uint64_t line_length;
    uint64_t line_idx;
    uint64_t char_idx;
    gfx_vertex_t vs[6];
    font_glyph_t g;
    font_glyph_t invalid_glyph;
    int x, y;
    WCHAR text[64];

    s = time_get();

    line_idx = _s_ui.line;
    x = y = 3;

    ASSERT(font_get_glyph('#', &invalid_glyph));

    gfx_clear();

    for (; y < _s_ui.wndsize.y && line_idx < file_get_line_count(); ++line_idx) {
        line_length = file_get_line_length(line_idx);
        if (!file_get_line_span(line_idx, 0, COUNTOF(line), line)) {
            break;
        }

        for (char_idx = 0; char_idx < line_length && x < _s_ui.wndsize.x; ++char_idx) {
            if (line[char_idx] == '\r' || line[char_idx] == '\n') {
                continue;
            }

            if (!font_get_glyph(line[char_idx], &g)) {
                g = invalid_glyph;
            }

            vs[0].x = (float)(x + g.ox);
            vs[0].y = (float)(y + g.oy);
            vs[0].u = (float)g.tx / GFX_TEXTURE_SIZE;
            vs[0].v = (float)g.ty / GFX_TEXTURE_SIZE;

            vs[1].x = (float)(x + g.ox + (float)g.w);
            vs[1].y = (float)(y + g.oy);
            vs[1].u = (float)(g.tx + g.w) / GFX_TEXTURE_SIZE;
            vs[1].v = (float)g.ty / GFX_TEXTURE_SIZE;

            vs[2].x = (float)(x + g.ox + (float)g.w);
            vs[2].y = (float)(y + g.oy + (float)g.h);
            vs[2].u = (float)(g.tx + g.w) / GFX_TEXTURE_SIZE;
            vs[2].v = (float)(g.ty + g.h) / GFX_TEXTURE_SIZE;

            vs[3].x = (float)(x + g.ox);
            vs[3].y = (float)(y + g.oy);
            vs[3].u = (float)g.tx / GFX_TEXTURE_SIZE;
            vs[3].v = (float)g.ty / GFX_TEXTURE_SIZE;

            vs[4].x = (float)(x + g.ox + (float)g.w);
            vs[4].y = (float)(y + g.oy + (float)g.h);
            vs[4].u = (float)(g.tx + g.w) / GFX_TEXTURE_SIZE;
            vs[4].v = (float)(g.ty + g.h) / GFX_TEXTURE_SIZE;

            vs[5].x = (float)(x + g.ox);
            vs[5].y = (float)(y + g.oy + (float)g.h);
            vs[5].u = (float)(g.tx) / GFX_TEXTURE_SIZE;
            vs[5].v = (float)(g.ty + g.h) / GFX_TEXTURE_SIZE;

            gfx_draw(vs, COUNTOF(vs));

            x += FONT_ADVANCE;
        }

        x = 3;
        y += FONT_LINE_HEIGHT + 3;
    }

    gfx_present();

    e = time_get();

    wnsprintfW(text, COUNTOF(text), L"%u ms, Ln %lld", (uint32_t)((e - s) * 1000), _s_ui.line + 1);
    SendMessageW(_s_ui.statusbar, SB_SETTEXTW, 0 | SBT_NOBORDERS, (LPARAM)text);
}

void _ui_dump_textures(void) {
    OPENFILENAMEW ofn = {0};
    WCHAR path[1024] = L"textures.tga";

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = _s_ui.wnd;
    ofn.lpstrFilter = L"TGA Images (*.tga)\0*.tga\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = COUNTOF(path);
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"tga";

    if (GetSaveFileNameW(&ofn)) {
        gfx_dump_texture_tga(ofn.lpstrFile);
    }
}
