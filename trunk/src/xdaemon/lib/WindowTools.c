#include "daemondll.h"

EXPENTRY HWND getParent(HWND child) {
	if(child == HWND_DESKTOP)
		return NULL;

	HWND parenthwnd = WinQueryWindow(child, QW_PARENT);
	if(parenthwnd == realdesktop)
		return HWND_DESKTOP;

	parenthwnd = WinQueryWindow(parenthwnd, QW_PARENT);
	if(parenthwnd == realdesktop)
		return HWND_DESKTOP;
	else
		return parenthwnd;
}

EXPENTRY HWND getValidWindow(HWND child) {

	if(child == HWND_DESKTOP)
		return HWND_DESKTOP;

	HWND prev = NULLHANDLE;
	HWND iterator = child;
	while(iterator) {
		HWND parent;
		char winclass[32];
		WinQueryClassName(iterator, sizeof(winclass), winclass);
		if(!strcmp(winclass, "XPMChild"))
			return iterator;
		if(!strcmp(winclass, "XPMBorder"))
			return HWND_DESKTOP;
		if(iterator == realdesktop)
			if(prev)
				return prev;
			else
				return HWND_DESKTOP;
		parent = WinQueryWindow(iterator, QW_PARENT);
		if(WinQueryWindowUShort(iterator, QWS_ID) == FI_FRAME && parent == realdesktop) {
			HWND client = WinWindowFromID(iterator, FID_CLIENT);
			WinQueryClassName(client, sizeof(winclass), winclass);
			if(!strcmp(winclass, "XPMChild"))
				if(prev)
					return prev;
				else
					return HWND_DESKTOP;
			else
				return iterator;
		}
		prev = iterator;
		iterator = parent;
	}
	return NULLHANDLE;
}

EXPENTRY void getPosition(XRectangle *result, HWND child, HWND relativeto) {
	SWP childpos;
	SWP otherpos;

	WinQueryWindowPos(child, &childpos);
	WinQueryWindowPos(relativeto, &otherpos);
	POINTL point = { 0, childpos.cy };
	WinMapWindowPoints(child, relativeto, &point, 1);
	result->x = point.x;
	result->y = otherpos.cy - point.y;
	result->width = childpos.cx;
	result->height = childpos.cy;
}
