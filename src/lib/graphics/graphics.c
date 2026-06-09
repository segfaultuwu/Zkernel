#include "charlesnicholson/nanoprintf.h"
#include "zk/arch/x86_64/pit.h"
#include <zk/alloc.h>
#include <stdarg.h>
#include <zk/graphics.h>
#include <zk/boot/limine.h>
#include <zk/mem.h>
#include <zk/string.h>

#include <stdint.h>
#include <stddef.h>

#include <flanterm.h>
#include <flanterm_backends/fb.h>

static struct flanterm_context *CTX = NULL;

void graphics_init(void) {
    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
        return;
    }

    struct limine_framebuffer *fb =
        framebuffer_request.response->framebuffers[0];

    CTX = flanterm_fb_init(
        kalloc,
        kfree,

        (uint32_t *)fb->address,
        fb->width,
        fb->height,
        fb->pitch,

        fb->red_mask_size,
        fb->red_mask_shift,
        fb->green_mask_size,
        fb->green_mask_shift,
        fb->blue_mask_size,
        fb->blue_mask_shift,

        NULL, // canvas
        NULL, // ansi_colours
        NULL, // ansi_bright_colours
        NULL, // default_bg
        NULL, // default_fg
        NULL, // default_bg_bright
        NULL, // default_fg_bright
        NULL, // font

        0, // font_width
        0, // font_height
        0, // font_spacing
        1, // font_scale_x
        1, // font_scale_y
        0, // margin
        0  // rotation
    );

    logfmt(LOG_INFO,"Framebuffer: %ux%u pitch=%u\n", fb->width, fb->height, fb->pitch);
    logfmt(LOG_INFO,"Address: 0x%lx\n", (uint64_t)fb->address);
    log(LOG_INFO,"Framebuffer console initialized\n");
}

void print(const char *text) {
    if (CTX == NULL || text == NULL) {
        return;
    }

    for (size_t i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            flanterm_write(CTX, "\r\n", 2);
        } else {
            flanterm_write(CTX, &text[i], 1);
        }
    }
}

void kprintf(const char *restrict format, ...) {
    if (CTX == NULL || format == NULL) {
        return;
    }

    va_list args;
    va_start(args, format);

    char buffer[1024];
    int len = npf_vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    if (len <= 0) {
        return;
    }

    buffer[sizeof(buffer) - 1] = '\0';
    print(buffer);
}

void println(const char *text) {
    print(text);
    print("\n");
}

static void log_prefix(loglevel level, const char *time) {
    print(ANSI_GRAY);
    print("[");
    print(time);
    print("] ");
    print(ANSI_RESET);

    switch (level) {
        case LOG_INFO:
            print(ANSI_GREEN "[INFO] " ANSI_RESET);
            break;

        case LOG_WARN:
            print(ANSI_YELLOW "[WARN] " ANSI_RESET);
            break;

        case LOG_ERROR:
            print(ANSI_RED "[ERROR] " ANSI_RESET);
            break;

        case LOG_DEBUG:
            print(ANSI_CYAN "[DEBUG] " ANSI_RESET);
            break;

        default:
            print(ANSI_MAGENTA "[UNKNOWN] " ANSI_RESET);
            break;
    }
}

void log(loglevel level, const char *text) {
    if (text == NULL) {
        return;
    }

    char uptime[64];
    pit_format_uptime(uptime, sizeof(uptime));

    log_prefix(level, uptime);
    println(text);
}

void logfmt(loglevel level, const char *format, ...) {
    if (format == NULL) {
        return;
    }

    char uptime[64];
    pit_format_uptime(uptime, sizeof(uptime));

    va_list args;
    va_start(args, format);

    char buffer[1024];
    int len = npf_vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    if (len <= 0) {
        return;
    }

    buffer[sizeof(buffer) - 1] = '\0';

    log_prefix(level, uptime);
    println(buffer);
}
