#include "X11.h"

// TODO InputOnly

HMODULE module = 0;

int XDefineCursor(Display *display, Window w, Cursor cursor) {

	DBUG_ENTER("XDefineCursor")
	if(cursor == None) {
		XUndefineCursor(display, w);
		DBUG_RETURN(0);
	}

	EB_Window *ebw = getResource(EBWINDOW, w);

	if(ebw->xpm) {
		char winclass[32];
		POINTL ptl;
		HWND hwnd;

		ebw->cursor = cursor;
		monitorResource((EB_Resource *)cursor);

		WinQueryPointerPos(HWND_DESKTOP, &ptl);
		hwnd = WinWindowFromPoint(HWND_DESKTOP, &ptl, TRUE);
		WinQueryClassName(hwnd, sizeof(winclass), winclass);
		if(!strcmp(winclass, "XPMChild") || !strcmp(winclass, "XPMBorder") || !strcmp(winclass, "XPMInput"))
			WinSendMsg(hwnd, UM_CHANGEPOINTER, 0, 0);
	}
	DBUG_RETURN(0);
}

Cursor XCreateGlyphCursor(register Display *dpy, Font source_font,
		Font mask_font, unsigned int source_char, unsigned int mask_char,
		XColor _Xconst *foreground, XColor _Xconst *background) {

	DBUG_ENTER("XCreateGlyphCursor")
	if(source_font == dpy->cursor_font && mask_font == dpy->cursor_font &&
			source_char + 1 == mask_char) {
		EB_Resource *cursorres;
		EB_Cursor *ebc = Xcalloc(1, sizeof(EB_Cursor));

		if(!module)
			DosQueryModuleHandle("XDAEMON.DLL", &module);
		// TODO set color!
		ebc->pointer = (HPOINTER)Daemon_exec(process, UM_LOADPOINTER, (void *)source_char);
		cursorres = createResource(EBCURSOR, ebc);
		addResource(&process->ebprocess->res, cursorres);
		DBUG_RETURN((Cursor)cursorres);
	}

	fprintf(stderr, "XCreateGlyphCursor not implemented for arbitrary fonts!\n");
	DBUG_RETURN(None);
}

Cursor XCreatePixmapCursor(Display *display, Pixmap source, Pixmap mask,
		XColor *foreground_color, XColor *background_color,
		unsigned int x, unsigned int y) {

	DBUG_ENTER("XCreatePixmapCursor")

	int width, height;
	getDrawableGeometry(source, NULL, NULL, &width,	&height, NULL, NULL);
	Pixmap pointer = XCreatePixmap(display, DefaultRootWindow(display), width, height * 2, 1);
	Pixmap color = XCreatePixmap(display, DefaultRootWindow(display), width, height, 24);
	GC gc = XCreateGC(display, pointer, 0, NULL);
	XSetFunction(display, gc, GXcopyInverted);
	XCopyArea(display, mask, pointer, gc, 0, 0, width, height, 0, 0);
	XFreeGC(display, gc);
	gc = XCreateGC(display, color, 0, NULL);
	unsigned long fg = (foreground_color->red & 0xFF00) * 256 + (foreground_color->green & 0xFF00) + ((foreground_color->blue & 0xFF00) >> 8);
	unsigned long bg = (background_color->red & 0xFF00) * 256 + (background_color->green & 0xFF00) + ((background_color->blue & 0xFF00) >> 8);
	XSetForeground(display, gc, fg);
	XSetBackground(display, gc, bg);
	XCopyPlane(display, source, color, gc, 0, 0, width, height, 0, 0, 1);
	XSetForeground(display, gc, 0xffffff);
	XSetBackground(display, gc, 0);
	XSetFunction(display, gc, GXand);
	XCopyPlane(display, mask, color, gc, 0, 0, width, height, 0, 0, 1);
	XFreeGC(display, gc);

	UM_CreatePointer *EventParams = Xcalloc(1, sizeof(UM_CreatePointer));
	EventParams->x = x;
	EventParams->y = height - y - 1;
	EventParams->pointer = pointer;
	EventParams->color = color;
	HPOINTER hpointer = (HPOINTER)Daemon_exec(process, UM_CREATEPOINTER, EventParams);

	XFreePixmap(display, pointer);
	XFreePixmap(display, color);

	if(!hpointer)
		DBUG_RETURN((Cursor)XCreateFontCursor(display, 0));

	EB_Cursor *ebc = Xcalloc(1, sizeof(EB_Cursor));
	ebc->pointer = hpointer;
	EB_Resource *cursorres = createResource(EBCURSOR, ebc);
	addResource(&process->ebprocess->res, cursorres);
	DBUG_RETURN((Cursor)cursorres);
}
