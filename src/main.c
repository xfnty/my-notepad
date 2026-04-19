#include "ui.h"
#include "std.h"
#include "file.h"
#include "time.h"
#include "debug.h"
#include "config.h"

static void init(void);
static void cleanup(void);

NORETURN void _start(void) {
    init();
    ui_run();
    cleanup();
    ExitProcess(0);
}

void init(void) {
    double init_begin_time, init_end_time;

    time_init();
    init_begin_time = time_get();

    debug_init();
    config_init();

    file_init();
    if (config_get()->filepath) {
        file_open(config_get()->filepath);
    }

    ui_init();

    init_end_time = time_get();
    debug_log(
        DEBUG_VERBOSE,
        L"startup took %llu ms",
        (uint64_t)((init_end_time - init_begin_time) * 1000)
    );
}

void cleanup(void) {
    ui_cleanup();
    config_cleanup();
    debug_cleanup();
}
