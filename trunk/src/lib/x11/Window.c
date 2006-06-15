#include "X11.h"

Status XGetGeometry(register Display *dpy, Drawable d, Window *root, int *x,
		int *y, unsigned int *width, unsigned int *height,
		unsigned int *borderWidth, unsigned int *depth) {
	DBUG_ENTER("XGetGeometry")
	getDrawableGeometry(d, x, y, width, height, borderWidth, depth);
	if(root)
		*root = DefaultRootWindow(dpy);
	DBUG_RETURN(True);
}

Status XGetWindowAttributes(register Display *dpy, Window w, XWindowAttributes *attr) {
	DBUG_ENTER("XGetWindowAttributes")
	HWND parenthwnd;
	EB_Window *ebw = getResource(EBWINDOW, w);
	if(ebw->hwnd == HWND_DESKTOP) {
		parenthwnd = ebw->hwnd;
		attr->map_state = IsViewable;
	} else {
		parenthwnd = WinQueryWindow(ebw->hwnd, QW_PARENT);
		if(WinIsWindowVisible(parenthwnd))
			attr->map_state = IsViewable;
		else
			if(WinIsWindowEnabled(parenthwnd))
				attr->map_state = IsUnviewable;
			else
				attr->map_state = IsUnmapped;
	}
	EB_Resource *eventmaskres;
	getDrawableGeometry(w, &attr->x, &attr->y, &attr->width, &attr->height,
			&attr->border_width, &attr->depth);
	attr->visual = DefaultVisual(dpy, 0);
	attr->root = DefaultRootWindow(dpy);
	attr->class = ebw->class;
	attr->bit_gravity = ebw->bit_gravity;
	attr->win_gravity = ebw->win_gravity;
	attr->backing_store = ebw->backing_store;
	attr->backing_planes = ebw->backing_planes;
	attr->backing_pixel = ebw->backing_pixel;
	attr->save_under = ebw->save_under;
	attr->colormap = COLORMAP;
	attr->map_installed = True;
	attr->all_event_masks = Daemon_orEventMasks(ebw->event_masks);
	eventmaskres = Daemon_findEventMask(ebw->event_masks, process);
	if(eventmaskres)
		attr->your_event_mask = eventmaskres->event_mask;
	else
		attr->your_event_mask = 0;
	attr->do_not_propagate_mask = ebw->do_not_propagate_mask;
	attr->override_redirect = ebw->override_redirect;
	attr->screen = DefaultScreenOfDisplay(dpy);
	DBUG_RETURN(True);
}

Bool XTranslateCoordinates(register Display *dpy, Window src_win,
		Window dest_win, int src_x, int src_y, int *dst_x, int *dst_y,
		Window *child) {
	DBUG_ENTER("XTranslateCoordinates")
	SWP swp1, swp2;
	POINTL temp;
	HWND childhwnd;
	EB_Window *ebw1 = getResource(EBWINDOW, src_win);
	EB_Window *ebw2 = getResource(EBWINDOW, dest_win);

	WinQueryWindowPos(ebw1->hwnd, &swp1);
	WinQueryWindowPos(ebw2->hwnd, &swp2);
	temp.x = src_x;
	temp.y = swp1.cy - src_y;
	WinMapWindowPoints(ebw1->hwnd, ebw2->hwnd, &temp, 1);
	*dst_x = temp.x;
	*dst_y = swp2.cy - temp.y;
	childhwnd = WinWindowFromPoint(ebw2->hwnd, &temp, FALSE);
	if(childhwnd && childhwnd != ebw2->hwnd)
		*child = getWindow(ebw2->hwnd, TRUE, NULL);
	else
		*child = None;
	DBUG_RETURN(True);
}

int XSetWindowBackground(register Display *dpy, Window w, unsigned long pixel) {
    DBUG_ENTER("XSetWindowBackground")
	EB_Window *ebw = getResource(EBWINDOW, w);
	ebw->background_pixel = pixel;
    DBUG_RETURN(1);
}
