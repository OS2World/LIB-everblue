#include "X11.h"

// Everblue is synchronized -> we need no flushing

int XFlush(Display* display)
{
	DBUG_ENTER("XFlush")
	DBUG_RETURN(1);
}

void _XFlush(Display *dpy) {
}

void _XFlushGCCache(Display *dpy, GC gc) {
}

int XNoOp(Display* display) {
	DosSleep(1);
	return TRUE;
}

int XSync(dpy, discard)
    register Display *dpy;
    Bool discard;
{
	DBUG_ENTER("XSync")
	// TODO: handle (discard == True)
	DBUG_RETURN(1);
}
