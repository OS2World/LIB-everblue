#include "X11.h"

#define X_CopyArea 62    
extern USHORT GXtoROPMode[16];

int XCopyPlane(register Display *dpy, Drawable src_drawable,
		Drawable dst_drawable, GC gc, int src_x, int src_y, unsigned int width,
		unsigned int height, int dst_x, int dst_y, unsigned long bit_plane) {       
	DBUG_ENTER("XCopyPlane")
	HPS src_hps, from, dst_hps, planehps;
	int src_viewheight, depth;
	LINEBUNDLE lbnd;
	Pixmap plane = NULLHANDLE;
	_XQEvent *newq;
	XEvent *new;

	EbGetHookAccess();
	EB_HPS *src_ebhps = getCachedHPS(process, src_drawable, NULL);
	EB_HPS *dst_ebhps = getCachedHPS(process, dst_drawable, gc);
	src_hps = src_ebhps->hps;
	dst_hps = dst_ebhps->hps;
	getDrawableGeometry(src_drawable, NULL, NULL, NULL, &src_viewheight, NULL, &depth);
	int dst_viewheight = getDrawableHeight(dst_drawable);

// coordinates for copy operation: aptl1 or aptl2b
	POINTL *aptl;

// coordinates for monochrome source
	POINTL aptl1[3] = {
		{ dst_x, dst_viewheight - dst_y - height },
		{ dst_x + width, dst_viewheight - dst_y },
		{ src_x, src_viewheight - src_y - height }
	};

// coordinates for plane selection
	POINTL aptl2a[3] = {
		{ 0, 0},
		{ width, height },
		{ src_x, src_viewheight - src_y - height }
	};

// coordinates for plane display
	POINTL aptl2b[3] = {
		{ dst_x, dst_viewheight - dst_y - height },
		{ dst_x + width, dst_viewheight - dst_y },
		{ 0, 0 }
	};

	if (!src_viewheight || !dst_viewheight || !gc) DBUG_RETURN(False);

if(depth > 1) {
// extract the selected plane into a temporary bitmap
// very slow - but only solution I can think of
		plane = XCreatePixmap(dpy, HWND_DESKTOP, width, height, 1);
		planehps = getCachedHPS(process, plane, NULL)->hps;
// AND operation with bit_plane (-> area/pattern color)
		lbnd.lColor = bit_plane & 0xffffff;
		GpiSetAttrs(planehps, PRIM_AREA, LBB_COLOR, 0, &lbnd);
		LockDisplay(dpy);
		GpiBitBlt(planehps, src_hps, 3, aptl2a, ROP_SRCCOPY, 0);
		aptl = aptl2b;
		from = planehps;
	}
	else {
		LockDisplay(dpy);
		aptl = aptl1;
		from = src_hps;
	}

// only change selected plane mask(s) in destination
	lbnd.lColor = gc->values.plane_mask & 0x00ffffff;
	GpiSetAttrs(dst_hps, PRIM_AREA, LBB_COLOR, 0, &lbnd);
// complex operation: function and plane_mask from GC
	GpiBitBlt(dst_hps, from, 3, aptl, (GXtoROPMode[gc->values.function] & 0xf0) | 0x0A, 0);
	UnlockDisplay(dpy);

	finishedDrawing(dst_drawable, dst_ebhps);
	EbReleaseHookAccess();

// GraphicsExpose must be implemented with WinQueryVisibleRegion!!!
	if(gc->values.graphics_exposures) {
		UM_CreateEvent *EventParams = Xcalloc(1, sizeof(UM_CreateEvent));
		new = Daemon_createEvent(&newq, dst_drawable, NoExpose);
		new->xnoexpose.major_code = X_CopyArea;
		new->xnoexpose.minor_code = 0;
		EventParams->procres = process;
		EventParams->event = newq;
		Daemon_exec(process, UM_CREATEEVENT, EventParams);
	}

	if(depth > 1)
		XFreePixmap(dpy, (Pixmap)plane);
	SyncHandle();
	DBUG_RETURN(1);
}
