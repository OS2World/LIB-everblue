#include "X11.h"

// TODO revert_to
// TODO window handle

int XGetInputFocus(Display *display, Window *focus_return, int *revert_to_return)
{
	DBUG_ENTER("XGetInputFocus");
	*focus_return = WinQueryFocus(HWND_DESKTOP);
	*revert_to_return = RevertToNone;
	DBUG_RETURN(True);
}

/*int XSetInputFocus(Display *display, Window focus, int revert_to, Time time)
{
	DBUG_ENTER("XSetInputFocus");
	WinSetFocus(HWND_DESKTOP, focus);
	DBUG_RETURN(True);
}*/
