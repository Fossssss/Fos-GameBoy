#include <windows.h>
#include <stdio.h>
#include "emu.h"
#include "cart.h"
#include "cpu.h"
#include "ui.h"

static emu_context ctx;

emu_context* emu_get_context() {
    return &ctx;
}

DWORD WINAPI cpu_run(LPVOID p) {
    cpu_init();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    while (ctx.running) {
        if (ctx.paused) {
            delay(10);
            continue;
        }

        if (!cpu_step()) {
            printf("CPU Stopped\n");
            return 0;
        }

        ctx.ticks++;
    }
    return 0;
}

int emu_run(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: emu <rom_file>\n");
        return -1;
    }

    if (!cart_load(argv[1])) {
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    }

    printf("Cart loaded..\n");

    ui_init();

    HANDLE t1 = CreateThread(
        NULL, 0, cpu_run, NULL, 0, NULL
    );

    if (t1 == NULL) {
        fprintf(stderr, "FAILED TO START MAIN CPU THREAD!\n");
        return -1;
    }
    while (!ctx.die) {
        Sleep(1); 
        ui_handle_events();
    }
    CloseHandle(t1);

    return 0;
}

void emu_cycles(int cpu_cycles) {
    // TODO...
}
