#include "X11.h"

// TODO InputOnly

int XMapWindow(Display* display, Window w) {
	DBUG_ENTER("XMapWindow")
	int ret = (int)Daemon_exec(process, UM_MAPWINDOW, (void *)w);
	DBUG_RETURN(ret);
}

int XMapSubwindows(Display *display, Window w) {
	DBUG_ENTER("XMapSubwindows")
	int ret = (int)Daemon_exec(process, UM_MAPSUBWINDOWS, (void *)w);
	DBUG_RETURN(ret);
}

int XUnmapWindow(Display* display, Window w) {
	DBUG_ENTER("XUnmapWindow")
	int ret = (int)Daemon_exec(process, UM_UNMAPWINDOW, (void *)w);
	DBUG_RETURN(ret);
}

int XLowerWindow(Display* display, Window w) {
	DBUG_ENTER("XLowerWindow")
	int ret = (int)Daemon_exec(process, UM_LOWERWINDOW, (void *)w);
	DBUG_RETURN(ret);
}

int XRaiseWindow(Display* display, Window w) {
	DBUG_ENTER("XRaiseWindow")
	int ret = (int)Daemon_exec(process, UM_RAISEWINDOW, (void *)w);
	DBUG_RETURN(ret);
}

int XMapRaised(Display* display, Window w) {
	DBUG_ENTER("XMapRaised")
	int ret = (int)Daemon_exec(process, UM_MAPRAISED, (void *)w);
	DBUG_RETURN(ret);
}
