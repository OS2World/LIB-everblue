#include "X11.h"

#define FID_BORDER 0x8009

Status XQueryTree (register Display *dpy, Window w, Window *root, Window *parent, Window **children, unsigned int *nchildren) {
	DBUG_ENTER("XQueryTree")
	EB_Window *ebw = getResource(EBWINDOW, w);
	if(!ebw) {
		DBUG_RETURN(False);
	} else {
		HWND current, current2, client;
		HENUM henum = WinBeginEnumWindows(ebw->hwnd);
		Window *copy = henum?alloca(256 * sizeof(HWND)):NULL, *temp;
		int i;

		if(!henum || !copy)
			DBUG_RETURN(0);

		*nchildren = 0;

		if(ebw->hwnd == HWND_DESKTOP)
			while((current = WinGetNextWindow(henum))) {
				if((WinQueryWindowUShort(current, QWS_ID) == FI_FRAME ||
						WinQueryWindowUShort(current, QWS_ID) == FID_BORDER) &&
						(client = WinWindowFromID(current, FID_CLIENT))) {
					char winclass[32];
					WinQueryClassName(client, sizeof(winclass), winclass);
					if(!strcmp(winclass, "XPMChild")) {
						HENUM henum2 = WinBeginEnumWindows(client);
						while((current2 = WinGetNextWindow(henum))) {
							*(copy++) = getWindow(current2, TRUE, NULL);
							(*nchildren)++;
						}
						WinEndEnumWindows(henum2);
						continue;
					}
				}
				*(copy++) = getWindow(current, TRUE, NULL);
				(*nchildren)++;
			} else
				while((current = WinGetNextWindow(henum))) {
					current2 = WinQueryWindow(current, QW_TOP);
					*(copy++) = getWindow(current2, TRUE, NULL);
					(*nchildren)++;
				}

		WinEndEnumWindows(henum);
		temp = *children = *nchildren?Xmalloc(*nchildren * sizeof(Window)):NULL;
		for(i = *nchildren; i; i--)
			*(temp++) = *(--copy);
		*parent = getWindow(getParent(ebw->hwnd), True, NULL);
		*root = DefaultRootWindow(dpy);
		DBUG_RETURN(1);
	}
}
