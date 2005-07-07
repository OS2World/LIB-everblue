#include "daemondll.h"

EXPENTRY HWND getParent(HWND child) {
	HWND parenthwnd = WinQueryWindow(child, QW_PARENT);
	char winclass[32];

	WinQueryClassName(parenthwnd, sizeof(winclass), winclass);
	if(!strcmp(winclass, "#1") || !strcmp(winclass, "XPMBorder")) {
		if(WinQueryWindowUShort(child, QWS_ID) != FID_CLIENT)
			return 0;
		else
			return WinQueryWindow(parenthwnd, QW_PARENT);
	}
	return parenthwnd;
}

EXPENTRY void getPosition(XRectangle *result, HWND child, HWND relativeto) {
	SWP childpos;
	SWP otherpos;

	WinQueryWindowPos(child, &childpos);
	WinQueryWindowPos(relativeto, &otherpos);
	result->x = 0;
	result->y = childpos.cy - 1;
	WinMapWindowPoints(child, relativeto, (PPOINTL)&result->x, 1);
	result->y = otherpos.cy - result->y - 1;
	result->width = childpos.cx;
	result->height = childpos.cy;
}
