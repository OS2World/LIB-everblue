#include "X11.h"

// this module provides functions for clearing whole windows or areas in them
// XClearWindow, XClearArea

// see xlib.pdf 8.1

// TODO: use GpiBox instead of WinFillRect?
//       fill with window background instead of simply erasing


int XClearWindow(Display* display, Window w) {
	DBUG_ENTER("XClearWindow")
	EB_HPS *ebhps = getCachedHPS(process, w, NULL);

	GpiErase(ebhps->hps);
	DBUG_RETURN(True);
}

int XClearArea(Display* display, Window w, int x, int y,
		unsigned int width, unsigned int height, Bool exposures) {
	DBUG_ENTER("XClearArea")
	EB_HPS *ebhps = getCachedHPS(process, w, NULL);
	EB_Window *ebw = getResource(EBWINDOW, w);
	RECTL rectl;
	int realwidth, realheight;

	getDrawableGeometry(w, NULL, NULL, &realwidth, &realheight, NULL, NULL);

	if(!width)
		width = realwidth - x;
	if(!height)
		height = realheight - y;
	rectl.xLeft = x;
	rectl.xRight = x + width;
	rectl.yTop = y;
	rectl.yBottom = y + height;
	WinFillRect(ebhps->hps, &rectl, ebw->background_pixel);
	if(exposures)
		WinInvalidateRect(ebw->hwnd, &rectl, False);
	DBUG_RETURN(True);
}
