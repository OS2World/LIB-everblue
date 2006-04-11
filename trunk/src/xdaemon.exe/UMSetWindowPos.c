#include "x11daemon.h"

Bool um_setWindowPos(UM_SetWindowPos *args) {
	EB_Window *ebw = getResource(EBWINDOW, args->window);
	HWND hwndborder = WinQueryWindow(ebw->hwnd, QW_PARENT);
	HWND parenthwnd = getParent(ebw->hwnd);
	SWP parent, border;
	int titlebar;

	// TODO compare with #1, direct border size
	if(!ebw->override_redirect && parenthwnd == HWND_DESKTOP)
		titlebar = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
	else
		titlebar = 0;

	WinQueryWindowPos(parenthwnd, &parent);

	WinQueryWindowPos(hwndborder, &border);
fprintf(logfile, "move with size: %x -> %x (%x), position: %x -> %x\n", border.cy, args->height + 2 * ebw->border_width + titlebar, args->height, border.y, parent.cy - args->height - ebw->border_width - args->y - titlebar);
fprintf(logfile, "position calc: %x - %x - %x - %x - %x\n", parent.cy, args->height, ebw->border_width, args->y, titlebar);
fflush(logfile);
	WinSetWindowPos(hwndborder, NULLHANDLE,
			args->x - ebw->border_width,
			parent.cy - args->height - ebw->border_width - args->y - titlebar,
			args->width + 2 * ebw->border_width,
			args->height + 2 * ebw->border_width + titlebar,
			SWP_MOVE | SWP_SIZE);
	WinQueryWindowPos(hwndborder, &border);
	WinSetWindowPos(ebw->hwnd, NULLHANDLE,
			ebw->border_width,
			ebw->border_width,
			border.cx - 2 * ebw->border_width,
			border.cy - 2 * ebw->border_width - titlebar,
			SWP_MOVE | SWP_SIZE);

	return TRUE;
}
