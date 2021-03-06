#include "X11.h"

// this module provides a function to copy an area between drawables
// XCopyArea

// see xlib.pdf 8.2

// TODO: handle invisible source areas
//       do all attributes
//       determine Expose result
// Optionally: optimize algorithm


#define X_CopyArea 62    

USHORT GXtoROPMode[16]	= {
	ROP_ZERO,	/* GXclear */
	ROP_SRCAND,	/* GXand */
	ROP_SRCERASE,	/* GXandReverse */
	ROP_SRCCOPY,	/* GXcopy */
	0x0022,		/* GXandInverted */
	0x00aa,		/* GXnoop */
	0x0066,		/* GXxor */
	ROP_SRCPAINT,	/* GXor */
	ROP_NOTSRCERASE,/* GXnor */
	0x0099,		/* GXequiv */
	ROP_DSTINVERT,	/* GXinvert */
	0x00dd,		/* GXorReverse */
	ROP_NOTSRCCOPY,	/* GXcopyInverted */
	ROP_MERGEPAINT,	/* GXorInverted */
	0x0077,		/* GXnand */
	ROP_ONE,	/* GXset */
};

int XCopyArea(Display *dpy, Drawable src_drawable, Drawable dst_drawable, GC gc,
		int src_x, int src_y, unsigned int width, unsigned int height, int dst_x, int dst_y) {
	DBUG_ENTER("XCopyArea")
	EbGetHookAccess();
	EB_HPS *src_ebhps = getCachedHPS(process, src_drawable, NULL);
	EB_HPS *dst_ebhps = getCachedHPS(process, dst_drawable, gc);
	HPS src_hps = src_ebhps->hps;
	HPS dst_hps = dst_ebhps->hps;
    int rc = GPI_OK;
	_XQEvent *newq;
	XEvent *new;
	Pixmap temp;
	HPS temphps;
	Pixmap temp2;
	HPS temp2hps;
	int dst_viewheight = getDrawableHeight(dst_drawable);
	int src_viewheight = getDrawableHeight(src_drawable);

// TODO will be optimized to XOR/MASKED AND/XOR operations (no buffers!)
	if(gc->values.clip_mask && !gc->rects) {
		EB_Pixmap *pixmask = getResource(EBPIXMAP, gc->values.clip_mask);
		HPS pixmaskhps = getCachedHPS(process, gc->values.clip_mask, NULL)->hps;

		POINTL aptl2a[3] = {
			{ 0, 0 },
			{ width, height },
			{ 0, 0 }
		};
		POINTL aptl2b[3] = {
			{ 0, 0 },
			{ width, height },
			{ dst_x, dst_viewheight - height - dst_y }
		};
		POINTL aptl2c[3] = {
			{ 0, 0 },
			{ width, height },
			{ src_x, src_viewheight + src_y - height }
		};
		POINTL aptl2d[3] = {
			{ dst_x, dst_viewheight - height - dst_y },
			{ dst_x + width, dst_viewheight - dst_y },
			{ 0, 0 }
		};
		POINTL aptl2e[3] = {
			{ 0, 0 },
			{ width, height },
			{ gc->values.clip_x_origin - dst_x, pixmask->pbmih->cy - gc->values.clip_y_origin + dst_y - height }
		};

		if(gc->values.function != GXcopy)
			fprintf(stderr, "function: %x not supported with clip mask in XCopyArea\n", gc->values.function);

		temp = XCreatePixmap(dpy, HWND_DESKTOP, width, height, 32);
		temphps = getCachedHPS(process, temp, NULL)->hps;
		GpiSetBackColor(temphps, 0xffffff);
		GpiSetColor(temphps, 0);
		GpiBitBlt(temphps, 0, 3, aptl2a, ROP_ONE, 0);
		GpiBitBlt(temphps, pixmaskhps, 3, aptl2e, ROP_SRCCOPY, 0);
		GpiBitBlt(temphps, dst_hps, 3, aptl2b, ROP_SRCAND, 0);

		temp2 = XCreatePixmap(dpy, HWND_DESKTOP, width, height, 32);
		temp2hps = getCachedHPS(process, temp2, NULL)->hps;
		GpiSetBackColor(temp2hps, 0);
		GpiSetColor(temp2hps, 0xffffff);
		GpiBitBlt(temp2hps, 0, 3, aptl2a, ROP_ONE, 0);
		GpiBitBlt(temp2hps, pixmaskhps, 3, aptl2e, ROP_SRCCOPY, 0);
		GpiBitBlt(temp2hps, src_hps, 3, aptl2c, ROP_SRCAND, 0);

		GpiBitBlt(temp2hps, temphps, 3, aptl2a, ROP_SRCPAINT, 0);
		GpiBitBlt(dst_hps, temp2hps, 3, aptl2d, ROP_SRCCOPY, 0);
		XFreePixmap(dpy, (Pixmap)temp);
		XFreePixmap(dpy, (Pixmap)temp2);
	} else {
		POINTL aptl[3] = {
			{ dst_x, dst_viewheight - dst_y - height },
			{ dst_x + width, dst_viewheight - dst_y },
			{ src_x, src_viewheight - src_y - height }
		};

		rc = GpiBitBlt(dst_hps, src_hps, 3, aptl, GXtoROPMode[gc->values.function], 0);
	}

	finishedDrawing(dst_drawable, dst_ebhps);

	EbReleaseHookAccess();


// TODO GraphicsExpose must be implemented with WinQueryVisibleRegion!!!
	if(gc->values.graphics_exposures) {
		UM_CreateEvent *EventParams = Xcalloc(1, sizeof(UM_CreateEvent));
		new = Daemon_createEvent(&newq, dst_drawable, NoExpose);
		new->xnoexpose.major_code = X_CopyArea;
		new->xnoexpose.minor_code = 0;
		EventParams->procres = process;
		EventParams->event = newq;
		Daemon_exec(process, UM_CREATEEVENT, EventParams);
	}

	if(rc != GPI_OK)
		DBUG_RETURN(False);
	DBUG_RETURN(True);
}
