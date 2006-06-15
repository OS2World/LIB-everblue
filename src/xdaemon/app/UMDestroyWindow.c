#include "x11daemon.h"

Bool um_destroyWindow(EB_Resource *windowres) {
	EB_Window *ebw = getResource(EBWINDOW, (XID)windowres);

	if(ebw->hwnd == HWND_DESKTOP)
		return False;

//	um_destroySubwindows(windowres); // TODO
	WinDestroyWindow(WinQueryWindow(ebw->hwnd, QW_PARENT));
										// TODO hpscache -> hpstodelete
	Daemon_freeEventMask(&ebw->event_masks);

	if(ebw->background_pixmap)
		unMonitorResource(ebw->background_pixmap);
										// TODO hints?
	freeResource(windowres);

	return True;
}
