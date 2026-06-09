#ifndef ZK_GRAPHICS_H
#define ZK_GRAPHICS_H

#define ANSI_RESET   "\033[0m"
#define ANSI_DIM     "\033[2m"

#define ANSI_RED     "\033[31m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN    "\033[36m"
#define ANSI_WHITE   "\033[37m"
#define ANSI_GRAY    "\033[90m"

typedef enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DEBUG
} loglevel;

void graphics_init(void);

void print(const char *text);
void println(const char *text);
void kprintf(const char *restrict format, ...);

void log(loglevel level, const char *text);
void logfmt(loglevel level, const char *restrict format, ...);

#endif
