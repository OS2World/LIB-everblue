#include "daemondll.h"

// tests top-level window
Bool foreignHWND(HWND hwnd) {
	EB_Window *ebw = getResource(EBWINDOW, getWindow(hwnd, FALSE, NULL));

	if(ebw && ebw->xpm)
		return False;
	HWND child = WinWindowFromID(hwnd, FID_CLIENT);
	ebw = getResource(EBWINDOW, getWindow(child, FALSE, NULL));
	return !ebw->xpm;
}

Bool outside(HWND hwnd, PPOINTL pptl) {
	SWP swp;
	POINTL local = { pptl->x, pptl->y };

	if(!WinQueryWindowPos(hwnd, &swp))
		return True;
	WinMapWindowPoints(HWND_DESKTOP, hwnd, &local, 1);
	if(local.x < swp.x || local.x >= swp.x + swp.cx)
		return True;
	if(local.y < swp.y || local.y >= swp.y + swp.cy)
		return True;
	return False;
}

Bool mapped(HWND hwnd) {
	if(WinIsWindowVisible(hwnd))
		return True;

	EB_Window *ebw = getResource(EBWINDOW, getWindow(hwnd, FALSE, NULL));

	return(ebw && ebw->xpm && ebw->class == InputOnly && ebw->mapped);
}

Window getValidDown(HWND hwnd) {
	Window w = getWindow(hwnd, FALSE, NULL);
	EB_Window *ebw = getResource(EBWINDOW, w);

	if(ebw)
		return w;
	HWND child = WinWindowFromID(hwnd, FID_CLIENT);
	w = getWindow(child, FALSE, NULL);
	ebw = getResource(EBWINDOW, w);
	if(ebw)
		return w;
	else
		abort();
}

Window getWindowFromPoint(PPOINTL pptl) {
	HWND found, prev = HWND_DESKTOP;

	do {
		HWND hwndNext;
		HENUM henum = WinBeginEnumWindows(prev);

		do
			hwndNext = WinGetNextWindow(henum);
		while(hwndNext && (!mapped(hwndNext) || outside(hwndNext, pptl)));

		if(!hwndNext)
			found = prev;
		else if(prev == HWND_DESKTOP && foreignHWND(hwndNext))
			found = hwndNext;
		else
			prev = hwndNext;
	} while(!found);

	return getValidDown(found);
}
