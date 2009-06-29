#include "lm_util.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int (*fstrncmp)(const char *, const char *, size_t n) = strncmp;
char *(*fstrstr)(const char *, const char *) = strstr;

void
lm_die(const char *errstr, ...) {
	va_list ap;

	va_start(ap, errstr);
	vfprintf(stderr, errstr, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

