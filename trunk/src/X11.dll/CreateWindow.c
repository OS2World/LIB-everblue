#include "X11.h"

Window XCreateWindow(Display *display, Window parent, int x, int y,
		unsigned int width, unsigned int height,
		unsigned int border_width, int depth, unsigned int class,
		Visual *visual, unsigned long valuemask,
		XSetWindowAttributes *attributes) {
	DBUG_ENTER("XCreateWindow")
	UM_CreateWindow *CreateParams = Xcalloc(1, sizeof(UM_CreateWindow));
	EB_Window *ebw = Xcalloc(1, sizeof(EB_Window));

//printf("Create: %ld, %ld, %ld, %ld (%x->)\n", x, y, width, height, parent);

	ebw->xpmchild = True;
	ebw->border_width = border_width;
	ebw->bit_gravity = ForgetGravity;
	ebw->win_gravity = NorthWestGravity;
	ebw->background_pixel = WhitePixel(display, 0);
	ebw->background_pixmap = None;
	ebw->border_pixel = -1;

	if(attributes && valuemask) {
		if(valuemask & CWOverrideRedirect)
			ebw->override_redirect = attributes->override_redirect;
		if(valuemask & CWSaveUnder)
			ebw->save_under = attributes->save_under;
		if(valuemask & CWDontPropagate)
			ebw->do_not_propagate_mask = attributes->do_not_propagate_mask;
		if(valuemask & CWBitGravity)
			ebw->bit_gravity = attributes->bit_gravity;
		if(valuemask & CWWinGravity)
			ebw->win_gravity = attributes->win_gravity;
		if(valuemask & CWBackPixel)
			ebw->background_pixel = attributes->background_pixel;
		if(valuemask & CWBackPixmap)
			ebw->background_pixmap = (EB_Resource *)attributes->background_pixmap;
		if(valuemask & CWCursor)
			ebw->cursor = attributes->cursor;
		if(valuemask & CWBorderPixel)
			ebw->border_pixel = attributes->border_pixel;

		if(valuemask & CWEventMask)
			CreateParams->event_mask = attributes->event_mask;
	}
	CreateParams->x = x;
	CreateParams->y = y;
	CreateParams->width = width;
	CreateParams->height = height;
	CreateParams->parent = parent;
	CreateParams->process = process;
	CreateParams->newwind = ebw;

	DBUG_RETURN((int)Daemon_exec(process, UM_CREATEWINDOW, CreateParams, NULL, 0));
}

int XDestroyWindow(Display *display, Window window) {
	DBUG_ENTER("XDestroyWindow")
	DBUG_RETURN((int)Daemon_exec(process, UM_DESTROYWINDOW, (EB_Resource *)window, NULL, 0));
}

Window XCreateSimpleWindow(Display* display, Window parent, int x, int y,
		unsigned int width, unsigned int height,
		unsigned int border_width, unsigned long border,
		unsigned long background) {
	DBUG_ENTER("XCreateSimpleWindow")
	XSetWindowAttributes attrib;
	EB_Window *ebw = getResource(EBWINDOW, parent);

	attrib.background_pixel = background;
	attrib.border_pixel = border;
	DBUG_RETURN(XCreateWindow(display, parent, x, y, width, height,
			border_width, 32, ebw->class, DefaultVisual(display, 1),
			CWBackPixel | CWBorderPixel, &attrib));
}

#if 0
void _XPurgeGC(Display* display, GC gc);

int XDestroySubwindows(Display *display, Window window)
{
	DBUG_ENTER("XDestroySubwindows")
	HENUM henum = WinBeginEnumWindows((HWND)window);
	HWND w;

	if (!henum)
		DBUG_RETURN(0);

	while ((w = WinQueryWindow(WinGetNextWindow(henum), QW_TOP)))
	{
		XDestroySubwindows(display, w);
		WinSendMsg(mainhwnd, UM_DestroyWindow,
			(MPARAM)WinQueryWindow(w, QW_PARENT), 0);
	}

	WinEndEnumWindows(henum);
	DBUG_RETURN(True);
}
#endif
