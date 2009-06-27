VERSION = 0.0

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -I.
LIBS =

# flags
CPPFLAGS = -D_BSD_SOURCE -DVERSION=\"${VERSION}\"
CFLAGS   = -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = -s ${LIBS}

# packages
PACKAGES = xft xext xrandr xrender xinerama

CFLAGS  += $(foreach n,${PACKAGES}, $(shell pkg-config --exists ${n} && echo -DHAVE_${n} | tr 'a-z' 'A-Z'))
CFLAGS  += $(foreach n,${PACKAGES}, $(shell pkg-config --cflags ${n}))
LDFLAGS += $(foreach n,${PACKAGES}, $(shell pkg-config --libs ${n}))

# Solaris
#CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}

# compiler and linker
CC = cc
