#include "X11.h"

int mapWindow(HWND hwnd) {
	DBUG_ENTER("mapWindow")
	char winclass[32];
	HWND parenthwnd = WinQueryWindow(hwnd, QW_PARENT);

	WinQueryClassName(parenthwnd, sizeof(winclass), winclass);
	if(!strcmp(winclass, "#1") || !strcmp(winclass, "XPMBorder"))
		WinSetWindowPos(parenthwnd, NULLHANDLE, 0, 0, 0, 0, SWP_SHOW | SWP_ACTIVATE);
	else
		WinSetWindowPos(hwnd, NULLHANDLE, 0, 0, 0, 0, SWP_SHOW);

	DBUG_RETURN(TRUE);
}

int XMapWindow(Display* display, Window w) {
	DBUG_ENTER("XMapWindow")
	EB_Window *ebw = getResource(EBWINDOW, w);
	mapWindow(ebw->hwnd);

	DBUG_RETURN(TRUE);
}

int XMapSubwindows(Display *display, Window w) {
	DBUG_ENTER("XMapSubwindows")
	HENUM henum;
	HWND child;
	EB_Window *ebw = getResource(EBWINDOW, w);

	henum = WinBeginEnumWindows(ebw->hwnd);
	while((child = WinGetNextWindow(henum)) != NULLHANDLE)
		mapWindow(WinQueryWindow(child, QW_TOP));
	DBUG_RETURN(WinEndEnumWindows(henum));
}

int XLowerWindow(Display* display, Window w) {
	DBUG_ENTER("XLowerWindow")
	EB_Window *ebw = getResource(EBWINDOW, w);
	HWND parenthwnd = WinQueryWindow(ebw->hwnd, QW_PARENT);

	WinSetWindowPos(parenthwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_ZORDER);
	DBUG_RETURN(TRUE);
}
