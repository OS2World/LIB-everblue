#include "X11.h"

Pixmap XCreatePixmap(Display *dpy, Drawable d, unsigned int width,
		unsigned int height, unsigned int depth) {
    DBUG_ENTER("XCreatePixmap")
    EB_Pixmap *pixmap;

    if(!depth)
		depth = 24;
    if(!(pixmap = Xcalloc(1, sizeof(EB_Pixmap))))
		DBUG_RETURN((Pixmap)0);
    if(!(pixmap->pbmih = Xcalloc(1, sizeof(BITMAPINFOHEADER))))
		DBUG_RETURN((Pixmap)0);
    if(!(pixmap->data = Xcalloc(1, ((width * height * depth + 31)>>5)*4)))
		DBUG_RETURN((Pixmap)0);

    pixmap->pbmih->cbFix = sizeof(BITMAPINFOHEADER2);
    pixmap->pbmih->cx = width;
	pixmap->pbmih->cy = height;
    pixmap->pbmih->cPlanes = 1;
    pixmap->pbmih->cBitCount = depth;

    DBUG_RETURN((Pixmap)createResource(EBPIXMAP, pixmap));
}
