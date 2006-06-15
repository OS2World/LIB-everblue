#include "X11.h"
#include <unistd.h>

char *XResourceManagerString(Display *dpy) {
    return dpy->xdefaults;
}

char *Xstrdup(const char *string) {
	char *p;
	size_t n;

	n = strlen (string) + 1;
	p = Xmalloc (n);
	if (p != NULL)
		memcpy (p, string, n);
	return p;
}

/*
 * _XGetHostname - similar to gethostname but allows special processing.
 */
int _XGetHostname (
    char *buf,
    int maxlen)
{
    int len;

#ifdef NEED_UTSNAME
    struct utsname name;

    if (maxlen <= 0 || buf == NULL)
	return 0;

    uname (&name);
    len = strlen (name.nodename);
    if (len >= maxlen) len = maxlen - 1;
    strncpy (buf, name.nodename, len);
    buf[len] = '\0';
#else
    if (maxlen <= 0 || buf == NULL)
	return 0;

    buf[0] = '\0';
    (void) gethostname (buf, maxlen);
    buf [maxlen - 1] = '\0';
    len = strlen(buf);
#endif /* NEED_UTSNAME */
    return len;
}

int
XDisplayKeycodes(dpy, min_keycode_return, max_keycode_return)
    Display *dpy;
    int *min_keycode_return, *max_keycode_return;
{
    *min_keycode_return = dpy->min_keycode;
    *max_keycode_return = dpy->max_keycode;
    return 1;
}

unsigned long XDisplayMotionBufferSize(dpy)
    Display *dpy;
{
    return dpy->motion_buffer;
}

VisualID XVisualIDFromVisual(visual)
    Visual *visual;
{
    return visual->visualid;
}
