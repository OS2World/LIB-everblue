#include "X11.h"

HMTX *_Xglobal_lock = &global_lock;
void (*_XCreateMutex_fn)(HMTX *) = NULL;
void (*_XFreeMutex_fn)(HMTX *) = NULL;
void (*_XLockMutex_fn)(HMTX *) = NULL;
void (*_XUnlockMutex_fn)(HMTX *) = NULL;

/*
 * This routine can be used to (cheaply) get some memory within a single
 * Xlib routine for scratch space.  A single buffer is reused each time
 * if possible.  To be MT safe, you can only call this between a call to
 * GetReq* and a call to Data* or _XSend*, or in a context when the thread
 * is guaranteed to not unlock the display.
 */
char *_XAllocScratch(
	register Display *dpy,
	unsigned long nbytes)
{
	if (nbytes > dpy->scratch_length) {
	    if (dpy->scratch_buffer) Xfree (dpy->scratch_buffer);
	    if ((dpy->scratch_buffer = Xmalloc((unsigned) nbytes)))
		dpy->scratch_length = nbytes;
	    else dpy->scratch_length = 0;
	}
	return (dpy->scratch_buffer);
}

/*
 * Scratch space allocator you can call any time, multiple times, and be
 * MT safe, but you must hand the buffer back with _XFreeTemp.
 */
char *_XAllocTemp(
    register Display *dpy,
    unsigned long nbytes)
{
    char *buf;

    buf = _XAllocScratch(dpy, nbytes);
    dpy->scratch_buffer = NULL;
    dpy->scratch_length = 0;
    return buf;
}

void _XFreeTemp(
    register Display *dpy,
    char *buf,
    unsigned long nbytes)
{
    if (dpy->scratch_buffer)
	Xfree(dpy->scratch_buffer);
    dpy->scratch_buffer = buf;
    dpy->scratch_length = nbytes;
}
