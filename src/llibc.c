#include "llibc.h"
#include <stdarg.h>

/* ─────────────── 저수준 시스템 콜 래퍼 ─────────────── */
static inline int _syscall3(int num, int a, int b, int c)
{
    int ret;
    __asm__ __volatile__ (
        "int $0x80"
        : "=a"(ret)
        : "a"(num), "b"(a), "c"(b), "d"(c)
        : "memory");
    return ret;
}

static inline void _syscall1_no_return(int num, int a)
{
    __asm__ __volatile__ (
        "int $0x80"
        :
        : "a"(num), "b"(a)
        : "memory");
    __builtin_unreachable();
}

/* ─────────────── 메모리/문자열 ─────────────── */
void *memcpy(void *dst, const void *src, size_t n)
{
    uint8_t *d = dst;
    const uint8_t *s = src;
    while (n--) *d++ = *s++;
    return dst;
}

void *memset(void *dst, int v, size_t n)
{
    uint8_t *d = dst;
    while (n--) *d++ = (uint8_t)v;
    return dst;
}

size_t strlen(const char *s)
{
    size_t i = 0;
    while (s[i]) ++i;
    return i;
}

int strcmp(const char *a, const char *b)
{
    while (*a && (*a == *b)) { ++a; ++b; }
    return (uint8_t)(*a) - (uint8_t)(*b);
}

/* ─────────────── 표준 입출력 ─────────────── */
int putchar(int c)
{
    char ch = (char)c;
    return _syscall3(SYSCALL_WRITE, 1, (int)&ch, 1);
}

int puts(const char *s)
{
    size_t len = strlen(s);
    int r = _syscall3(SYSCALL_WRITE, 1, (int)s, (int)len);
    putchar('\n');
    return r;
}

/* 아주 가벼운 %d %s %% 만 지원하는 printf */
static void print_dec(int v)
{
    char buf[12]; /* 32-bit 정수 최대 10자리 + 부호 + \0 */
    int i = 0, neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    do { buf[i++] = '0' + (v % 10); v /= 10; } while (v);
    if (neg) buf[i++] = '-';
    while (i--) putchar(buf[i]);
}

int printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    for (; *fmt; ++fmt) {
        if (*fmt != '%') { putchar(*fmt); continue; }
        ++fmt;
        switch (*fmt) {
            case 's': {
                const char *s = va_arg(ap, const char*);
                puts(s);
                break;
            }
            case 'd': {
                int v = va_arg(ap, int);
                print_dec(v);
                break;
            }
            case '%':
                putchar('%');
                break;
            default:
                putchar('%'); putchar(*fmt);
        }
    }
    va_end(ap);
    return 0;
}

/* ─────────────── 종료 ─────────────── */
void exit(int status)
{
    _syscall1_no_return(SYSCALL_EXIT, status);
}
