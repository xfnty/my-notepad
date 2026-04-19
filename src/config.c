#include "config.h"

#include "std.h"
#include "debug.h"

static struct {
    LPCWSTR cmdline;
    LPCWSTR *argv;
    int argc;
    config_t c;
} _s_config;

void config_init(void) {
    int i;

    _s_config.cmdline = GetCommandLineW();
    _s_config.argv = (LPCWSTR*)CommandLineToArgvW(_s_config.cmdline, &_s_config.argc);
    ASSERT(_s_config.argv);
    ASSERT(_s_config.argc >= 1);

    _s_config.c.exe = _s_config.argv[0];

    if (_s_config.argc >= 2) {
        _s_config.c.filepath = _s_config.argv[1];
    }

    for (i = 2; i < _s_config.argc; i++) {
        debug_log(DEBUG_WARNING, L"argument \"%s\" ignored", _s_config.argv[i]);
    }
}

void config_cleanup(void) {
    LocalFree(_s_config.argv);
}

const config_t *config_get(void) {
    return &_s_config.c;
}
