#include "x11daemon.h"

int positionWindow(HWND hwnd, HWND position, ULONG flags) {
	char winclass[32];
	HWND parenthwnd = WinQueryWindow(hwnd, QW_PARENT);

	WinQueryClassName(parenthwnd, sizeof(winclass), winclass);
	if(!strcmp(winclass, "#1") || !strcmp(winclass, "XPMBorder"))
		WinSetWindowPos(parenthwnd, position, 0, 0, 0, 0, flags);
	else
		WinSetWindowPos(hwnd, position, 0, 0, 0, 0, flags);

	return TRUE;
}

int um_mapWindow(Window w) {
	EB_Window *ebw = getResource(EBWINDOW, w);
	positionWindow(ebw->hwnd, NULLHANDLE, SWP_SHOW | SWP_ACTIVATE);

	return TRUE;
}

int um_mapSubwindows(Window w) {
	HENUM henum;
	HWND child;
	EB_Window *ebw = getResource(EBWINDOW, w);

	char winclass[32];
	HWND parenthwnd = WinQueryWindow(ebw->hwnd, QW_PARENT);
	WinQueryClassName(parenthwnd, sizeof(winclass), winclass);
	if(!strcmp(winclass, "#1") || !strcmp(winclass, "XPMBorder"))
		return TRUE;

	henum = WinBeginEnumWindows(ebw->hwnd);
	while((child = WinGetNextWindow(henum)) != NULLHANDLE)
		positionWindow(WinQueryWindow(child, QW_TOP), NULLHANDLE, SWP_SHOW | SWP_ACTIVATE);
	return WinEndEnumWindows(henum);
}

int um_unmapWindow(Window w) {
	EB_Window *ebw = getResource(EBWINDOW, w);
	positionWindow(ebw->hwnd, NULLHANDLE, SWP_HIDE);

	return TRUE;
}

int um_lowerWindow(Window w) {
	EB_Window *ebw = getResource(EBWINDOW, w);
	positionWindow(ebw->hwnd, HWND_BOTTOM, SWP_ZORDER);

	return TRUE;
}

int um_raiseWindow(Window w) {
	EB_Window *ebw = getResource(EBWINDOW, w);
	positionWindow(ebw->hwnd, HWND_TOP, SWP_ZORDER | SWP_ACTIVATE);

	return TRUE;
}

int um_mapRaised(Window w) {
	EB_Window *ebw = getResource(EBWINDOW, w);
	positionWindow(ebw->hwnd, HWND_TOP, SWP_SHOW | SWP_ZORDER | SWP_ACTIVATE);

	return TRUE;
}
