#ifndef LLIBC_H
#define LLIBC_H

#include <stdint.h>
typedef uint32_t size_t;

#define NULL ((void*)0)

/* ─────────────── 시스템 콜 번호 (Luxon 커널 규격) ─────────────── */
#define SYSCALL_WRITE 0   /* fd, buf, len */
#define SYSCALL_EXIT  1   /* status */

/* ─────────────── stdio ─────────────── */
int   putchar(int c);
int   puts(const char *s);
int   printf(const char *fmt, ...);

/* ─────────────── 문자열/메모리 ─────────────── */
size_t strlen (const char *s);
int    strcmp (const char *a, const char *b);
void  *memcpy (void *dst, const void *src, size_t n);
void  *memset (void *dst, int v, size_t n);

/* ─────────────── 유틸 ─────────────── */
void   exit(int status) __attribute__((noreturn));

#endif /* LLIBC_H */
