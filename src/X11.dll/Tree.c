#include "X11.h"

Status XQueryTree (register Display *dpy, Window w, Window *root, Window *parent, Window **children, unsigned int *nchildren) {
	DBUG_ENTER("XQueryTree")
	EB_Window *ebw = getResource(EBWINDOW, w);
	if(!ebw)
		DBUG_RETURN(False)
	else {
		HWND current, current2;
		HENUM henum = WinBeginEnumWindows(ebw->hwnd);
		Window *copy = henum?alloca(256 * sizeof(HWND)):NULL, *temp;
		int i;

		if(!henum || !copy)
			DBUG_RETURN(0);

		*nchildren = 0;
		while((current = WinGetNextWindow(henum))) {
			if((current2 = WinWindowFromID(current, FID_CLIENT)))
				current = current2;
			*(copy++) = getWindow(current, TRUE, NULL);
			(*nchildren)++;
		}

		WinEndEnumWindows(henum);
		temp = *children = *nchildren?Xmalloc(*nchildren * sizeof(Window)):NULL;
		for(i = *nchildren; i; i--)
			*(temp++) = *(--copy);
		*parent = getWindow(getParent(ebw->hwnd), True, NULL);
		*root = DefaultRootWindow(dpy);
		DBUG_RETURN(1)
	}
}
