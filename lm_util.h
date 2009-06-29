#ifndef LM_UTIL_H
#define LM_UTIL_H

#include <stdlib.h>

extern void lm_die(const char *errstr, ...);

extern int (*fstrncmp)(const char *, const char *, size_t n);
extern char *(*fstrstr)(const char *, const char *);

#endif // LM_UTIL_H
