#include "X11.h"

int XMoveResizeWindow(Display* display, Window w, int x, int y, unsigned int width, unsigned int height)
{
	DBUG_ENTER("XMoveResizeWindow")
	UM_SetWindowPos *SizeParams = Xcalloc(1, sizeof(UM_SetWindowPos));

//fprintf("MoveResize: %ld, %ld, %ld, %ld (%x)\n", x, y, width, height, w);

	SizeParams->window = w;
	SizeParams->x = x;
	SizeParams->y = y;
	SizeParams->width = width;
	SizeParams->height = height;

	DBUG_RETURN((int)Daemon_exec(process, UM_SETWINDOWPOS, SizeParams, NULL, 0));
}
