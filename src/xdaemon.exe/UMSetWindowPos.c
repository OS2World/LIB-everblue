#include "x11daemon.h"

Bool um_setWindowPos(UM_SetWindowPos *args) {
	EB_Window *ebw = getResource(EBWINDOW, args->window);
	HWND hwndborder = WinQueryWindow(ebw->hwnd, QW_PARENT);
	HWND parenthwnd = getParent(ebw->hwnd);
	SWP parent, border;
	int borderwidth, borderheight, titlebar;

	// TODO compare with #1, direct border size
	if(!ebw->override_redirect && parenthwnd == HWND_DESKTOP) {
		borderwidth = WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
		borderheight = WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
		titlebar = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
	}
	else {
		borderwidth = borderheight = ebw->border_width;
		titlebar = 0;
	}

	WinQueryWindowPos(parenthwnd, &parent);
	WinSetWindowPos(hwndborder, NULLHANDLE,
			args->x - borderwidth,
			parent.cy - args->height - borderheight - args->y - titlebar,
			args->width + 2 * borderwidth,
			args->height + 2 * borderheight + titlebar,
			SWP_MOVE | SWP_SIZE);
	WinQueryWindowPos(hwndborder, &border);
	WinSetWindowPos(ebw->hwnd, NULLHANDLE,
			borderwidth,
			borderheight,
			border.cx - 2 * borderwidth,
			border.cy - 2 * borderheight - titlebar,
			SWP_MOVE | SWP_SIZE);

	return TRUE;
}
