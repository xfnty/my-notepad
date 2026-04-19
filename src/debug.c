#include "debug.h"

#include "util.h"

typedef enum {
    _DEBUG_STDOUT  = 0x1,
    _DEBUG_DEBUGEE = 0x2,
    _DEBUG_COLOR   = 0x4
} _debug_flags_t;

static LPCWSTR _s_debug_format[] = {
    L"%04X:%04X ",
    L"%04X:%04X ",
    L"%04X:%04X warning: ",
    L"%04X:%04X error: ",
};
static LPCWSTR _s_debug_colored_format[] = {
    L"\x1b[1;30m%04X:%04X %s\x1b[0m\n",
    L"\x1b[1;30m%04X:%04X\x1b[0m %s\x1b[0m\n",
    L"\x1b[1;30m%04X:%04X \x1b[1;93mwarning\x1b[0m: %s\x1b[0m\n",
    L"\x1b[1;30m%04X:%04X \x1b[1;91merror\x1b[0m: %s\x1b[0m\n",
};

static struct {
    HANDLE stdout;
    CRITICAL_SECTION outputLock;
    _debug_flags_t flags;
    DWORD pid;
} _s_debug;

void debug_init(void) {
    DWORD conMode;

    InitializeCriticalSection(&_s_debug.outputLock);

    AttachConsole(ATTACH_PARENT_PROCESS);
    _s_debug.stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (_s_debug.stdout != INVALID_HANDLE_VALUE) {
        _s_debug.flags |= _DEBUG_STDOUT;
        GetConsoleMode(_s_debug.stdout, &conMode);
        if (SetConsoleMode(_s_debug.stdout, conMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
            _s_debug.flags |= _DEBUG_COLOR;
        }
    }

    if (IsDebuggerPresent()) {
        _s_debug.flags |= _DEBUG_DEBUGEE;
    }

    _s_debug.pid = GetCurrentProcessId();
}

void debug_cleanup(void) {
    DeleteCriticalSection(&_s_debug.outputLock);
}

void debug_log(debug_log_level_t level, LPCWSTR format, ...) {
    va_list args;
    WCHAR formatted_text[500], final_text[524];
    int formatted_text_len, final_text_len;
    DWORD tid;

    if (!(_s_debug.flags & (_DEBUG_DEBUGEE | _DEBUG_STDOUT))) {
        return;
    }

    tid = GetCurrentThreadId();

    va_start(args, format);
    formatted_text_len = wvnsprintfW(formatted_text, COUNTOF(formatted_text) - 1, format, args);
    formatted_text[formatted_text_len] = L'\0';

    EnterCriticalSection(&_s_debug.outputLock);
    if (_s_debug.flags & _DEBUG_DEBUGEE) {
        final_text_len = wnsprintfW(
            final_text,
            COUNTOF(final_text),
            _s_debug_format[level],
            _s_debug.pid,
            tid,
            formatted_text
        );
        OutputDebugStringW(final_text);
    }
    if (_s_debug.flags & _DEBUG_STDOUT) {
        final_text_len = wnsprintfW(
            final_text,
            COUNTOF(final_text),
            ((_s_debug.flags & _DEBUG_COLOR) ? (_s_debug_colored_format) : (_s_debug_format))[level],
            _s_debug.pid,
            tid,
            formatted_text
        );
        WriteConsoleW(_s_debug.stdout, final_text, final_text_len, NULL, NULL);
    }
    LeaveCriticalSection(&_s_debug.outputLock);
}

void debug_crash(LPCWSTR format, ...) {
    va_list args;
    WCHAR text[1024];
    int text_len;

    if (_s_debug.flags & _DEBUG_DEBUGEE) {
        #if defined(_MSC_VER)
            __debugbreak();
        #else
            __builtin_trap();
        #endif

        return;
    }

    va_start(args, format);
    text_len = wvnsprintfW(text, COUNTOF(text) - 2, format, args);
    text[text_len] = L'\0';

    MessageBoxW(NULL, text, L"MyNotepad has crashed.", MB_OK | MB_ICONERROR);

    ExitProcess(1);
}
