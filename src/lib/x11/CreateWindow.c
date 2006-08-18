#include "X11.h"

Window XCreateWindow(Display *display, Window parent, int x, int y,
		unsigned int width, unsigned int height,
		unsigned int border_width, int depth, unsigned int class,
		Visual *visual, unsigned long valuemask,
		XSetWindowAttributes *attributes) {
	DBUG_ENTER("XCreateWindow")
	UM_CreateWindow *CreateParams = Xcalloc(1, sizeof(UM_CreateWindow));
	EB_Window *ebw = Xcalloc(1, sizeof(EB_Window));

fprintf(stderr, "Create: %ld, %ld, %ld, %ld (%x->)\n", x, y, width, height, parent);

	ebw->class = class;
	ebw->xpm = True;
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

	Window win = (Window)Daemon_exec(process, UM_CREATEWINDOW, CreateParams);
	DBUG_RETURN(win);
}

int XDestroyWindow(Display *display, Window window) {
	DBUG_ENTER("XDestroyWindow")
	int ret = (int)Daemon_exec(process, UM_DESTROYWINDOW, (EB_Resource *)window);
	DBUG_RETURN(ret);
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
	Window win = XCreateWindow(display, parent, x, y, width, height,
			border_width, 32, ebw->class, DefaultVisual(display, 1),
			CWBackPixel | CWBorderPixel, &attrib);
	DBUG_RETURN(win);
}

int XDestroySubwindows(Display *display, Window window)
{
	DBUG_ENTER("XDestroySubwindows")
	EB_Window *ebw = getResource(EBWINDOW, window);
	HENUM henum = WinBeginEnumWindows(ebw->hwnd);
	HWND child;

	if (!henum)
		DBUG_RETURN(0);

	while((child = WinQueryWindow(WinGetNextWindow(henum), QW_TOP))) {
		Window w = (EB_Resource *)getWindow(child, TRUE, NULL);
		XDestroySubwindows(display, w);
		Daemon_exec(process, UM_DESTROYWINDOW, w);
	}

	WinEndEnumWindows(henum);
	DBUG_RETURN(True);
}
