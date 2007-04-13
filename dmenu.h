/* © 2006-2007 Anselm R. Garbe <garbeam at gmail dot com>
 * © 2006-2007 Sander van Dijk <a dot h dot vandijk at gmail dot com>
 * See LICENSE file for license details. */
#include <X11/Xlib.h>

#define FONT			"-*-fixed-medium-r-normal-*-13-*-*-*-*-*-*-*"
#define NORMBGCOLOR		"#eeeeee"
#define NORMFGCOLOR		"#222222"
#define SELBGCOLOR		"#006699"
#define SELFGCOLOR		"#ffffff"
#define SPACE			30 /* px */

/* color */
enum { ColFG, ColBG, ColLast };

typedef struct {
	int x, y, w, h;
	unsigned long norm[ColLast];
	unsigned long sel[ColLast];
	Drawable drawable;
	GC gc;
	struct {
		XFontStruct *xfont;
		XFontSet set;
		int ascent;
		int descent;
		int height;
	} font;
} DC; /* draw context */

int screen;
Display *dpy;
DC dc;			/* global drawing context */

/* draw.c */
void drawtext(const char *text, unsigned long col[ColLast]);
unsigned int textw(const char *text);
unsigned int textnw(const char *text, unsigned int len);

/* util.c */
void *emalloc(unsigned int size);		/* allocates memory, exits on error */
void eprint(const char *errstr, ...);		/* prints errstr and exits with 1 */
char *estrdup(const char *str);			/* duplicates str, exits on allocation error */
