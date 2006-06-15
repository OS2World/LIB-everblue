#include "X11.h"

Pixmap XCreatePixmap(Display *dpy, Drawable d, unsigned int width,
		unsigned int height, unsigned int depth) {
    DBUG_ENTER("XCreatePixmap")
    EB_Pixmap *pixmap;

    if(!depth)
		depth = 32;
    if(!(pixmap = Xcalloc(1, sizeof(EB_Pixmap))))
		DBUG_RETURN((Pixmap)0);
    if(!(pixmap->pbmih = Xcalloc(20, sizeof(BITMAPINFO))))
		DBUG_RETURN((Pixmap)0);
    if(!(pixmap->data = Xcalloc(1, height * ((width * depth + 31)>>5)*4)))
		DBUG_RETURN((Pixmap)0);

// we use pbmih as BITMAPINFO and as BITMAPINFOHEADER
// => allocated size is sizeof(BITMAPINFO), cbFix is sizeof(BITMAPINFOHEADER)!

    pixmap->pbmih->cbFix = sizeof(BITMAPINFOHEADER);
    pixmap->pbmih->cx = width;
	pixmap->pbmih->cy = height;
    pixmap->pbmih->cPlanes = 1;
    pixmap->pbmih->cBitCount = depth;

	Pixmap created = (Pixmap)createResource(EBPIXMAP, pixmap);
    DBUG_RETURN(created);
}


// TODO monitoring - resource management
int XFreePixmap(Display* display, Pixmap _pixmap) {
	DBUG_ENTER("XFreePixmap")
	EB_Pixmap *pixmap = getResource(EBPIXMAP, _pixmap);
	if(!pixmap)
		DBUG_RETURN(FALSE);
	Xfree(pixmap->pbmih);
	Xfree(pixmap->data);
	freeResource((EB_Resource *)_pixmap);
	DBUG_RETURN(TRUE);
}
