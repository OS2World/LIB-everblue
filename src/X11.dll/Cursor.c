#include "X11.h"

Cursor XCreateFontCursor (Display *display, unsigned int shape) {
	EB_Resource *cursorres;
	EB_Cursor *ebc = Xcalloc(1, sizeof(EB_Cursor));

	ebc->pointer = (HPOINTER)Daemon_exec(process, UM_LOADPOINTER, (void *)shape, NULL, 0);
	cursorres = createResource(EBCURSOR, ebc);
	addResource(&process->ebprocess->res, cursorres);
	return (Cursor)cursorres;
}

int XDefineCursor(Display *display, Window w, Cursor cursor) {
	DBUG_ENTER("XDefineCursor")
	EB_Window *ebw = getResource(EBWINDOW, w);

	if(ebw->xpmchild) {
		char winclass[32];
		POINTL ptl;
		HWND hwnd;

		ebw->cursor = cursor;
		monitorResource((EB_Resource *)cursor);

		WinQueryPointerPos(HWND_DESKTOP, &ptl);
		hwnd = WinWindowFromPoint(HWND_DESKTOP, &ptl, TRUE);
		WinQueryClassName(hwnd, sizeof(winclass), winclass);
		if(!strcmp(winclass, "XPMChild") || !strcmp(winclass, "XPMBorder"))
			WinSendMsg(hwnd, UM_CHANGEPOINTER, 0, 0);
	}
	DBUG_RETURN(0);
}
