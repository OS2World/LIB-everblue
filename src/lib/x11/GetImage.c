#include "X11.h"

// TODO borders can be read with this function

extern int _XSetImage(XImage *srcimg, register XImage *dstimg, register int x, register int y);

#define ROUNDUP(nbytes, pad) (((((nbytes) - 1) + (pad)) / (pad)) * (pad))

Visual *_XVIDtoVisual (Display*, VisualID);
int _XGetScanlinePad(Display*, int);

static unsigned int Ones(mask)					 /* HACKMEM 169 */
	unsigned long mask;
{
	register unsigned long y;

	y = (mask >> 1) &033333333333;
	y = mask - y - ((y >>1) & 033333333333);
	return ((unsigned int) (((y + (y >> 3)) & 030707070707) % 077));
}

XImage *XGetImage (dpy, d, x, y, width, height, plane_mask, format)
		register Display *dpy;
		Drawable d;
		int x, y;
		unsigned int width, height;
		unsigned long plane_mask;
		int format;	/* either XYPixmap or ZPixmap */
{
	DBUG_ENTER("XGetImage")
	char *data;
	XImage *image = NULL;
	DEVOPENSTRUC dop = { 0L, "DISPLAY", NULL, 0L, 0L, 0L, 0L, 0L, 0L };
	BITMAPINFOHEADER2 bmih;
	SIZEL sizl = {0, 0};
	HAB hab;
	Daemon_getPMHandle(process, &hab);
	HDC ihdc = DevOpenDC(hab, OD_MEMORY, "*", 5L, (PDEVOPENDATA)&dop, NULLHANDLE);
	HPS ihps = GpiCreatePS(hab, ihdc, &sizl, PU_PELS | GPIT_MICRO | GPIA_ASSOC );
	int dheight = getDrawableHeight(d);
	EB_HPS *ebhps = getCachedHPS(process, d, NULL);
	POINTL aptl[] = { {0, 0}, {width, height}, {x,dheight-y-height} };
	HBITMAP ihbm;

	if(!dheight || !ihps)
		DBUG_RETURN(NULL);

	LockDisplay(dpy);

	memset(&bmih, 0, sizeof(BITMAPINFOHEADER2));
	bmih.cbFix = sizeof(BITMAPINFOHEADER2);
	bmih.cx = width;
	bmih.cy = height;
	bmih.cPlanes = 1;
	bmih.cBitCount = 32;
	ihbm = GpiCreateBitmap(ihps, &bmih, 0L, NULL, NULL);

	GpiSetBitmap(ihps, ihbm);
	GpiCreateLogColorTable(ihps, 0, LCOLF_RGB, 0, 0, NULL);

	int bytesperline = (((bmih.cBitCount * bmih.cx)+31)/32) * 4;
	data = Xmalloc(bytesperline * bmih.cy);
	if(data) {
		GpiBitBlt(ihps,ebhps->hps,3,aptl,ROP_SRCCOPY,BBO_IGNORE);
		GpiQueryBitmapBits(ihps, 0L, height, data, (PBITMAPINFO2)&bmih);

		char *temp = Xcalloc(bytesperline, height);
		int z;
		for(z = 0; z < height; z++)
			memcpy(&temp[z*bytesperline], &data[(height-z-1)*bytesperline], bytesperline);
		Xfree(data);
		data = temp;

		if(format == XYPixmap)
			image = XCreateImage(dpy, _XVIDtoVisual(dpy, 0),
					Ones (plane_mask &
					(((unsigned long)0xFFFFFFFF) >> (32 - bmih.cBitCount))),
					format, 0, data, width, height, dpy->bitmap_pad, 0);
		else /* format == ZPixmap */
			image = XCreateImage (dpy, _XVIDtoVisual(dpy, 0),
					bmih.cBitCount, ZPixmap, 0, data, width, height,
					_XGetScanlinePad(dpy, (int) bmih.cBitCount), 0);
	}

	if(!image)
		 Xfree(data);
	if(!ihbm)
		GpiDeleteBitmap(ihbm);
	if(!ihps)
		GpiDestroyPS(ihps);
	if(!ihdc)
		DevCloseDC(ihdc);
	UnlockDisplay(dpy);
	SyncHandle();
	DBUG_RETURN(image);
}

XImage *XGetSubImage(register Display *dpy, Drawable d, int x, int y,
		unsigned int width, unsigned int height, unsigned long plane_mask,
		int format, XImage *dest_image, int dest_x, int dest_y) {
	DBUG_ENTER("XGetSubImage")
	XImage *temp_image;
	temp_image = XGetImage(dpy, d, x, y, width, height, 
				plane_mask, format);
	if(!temp_image)
		DBUG_RETURN((XImage *)NULL);
	_XSetImage(temp_image, dest_image, dest_x, dest_y);
	XDestroyImage(temp_image);
	DBUG_RETURN(dest_image);
}
