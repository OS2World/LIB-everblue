#include "X11.h"

#ifdef OS2I18N
#define UCSCPSIZE 16
static UniChar ucsCodepage[UCSCPSIZE];

int Xlib_XlatISO8859_1(char *tgt, int tgt_size, char *src, int src_size) {
	UconvObject xlat1 = NULL, xlat2 = NULL;
	int result = 0;
	size_t src_pos = src_size, tgt_pos = tgt_size, dup;
	size_t tmpsize = src_size + 1, tmppos = tmpsize;
	UniChar *tmpbuf = alloca(tmpsize * sizeof(UniChar)), *tmp = tmpbuf;
	if(UniCreateUconvObject( ISO8859_1, &xlat1 ) ||
		    UniCreateUconvObject( ucsCodepage, &xlat2 )) {
		result = (tgt_size>src_size)?src_size:tgt_size;
		strncpy(tgt, src, result);
	} else {
		UniUconvToUcs(xlat1, (void **)&src, &src_pos, &tmp, &tmppos, &dup);
		tmp = tmpbuf; tmppos = tmpsize - tmppos;
		UniUconvFromUcs(xlat2, &tmp, &tmppos, (void **)&tgt, &tgt_pos, &dup);
		result = tgt_size - tgt_pos;
	}
	if(xlat1)
		UniFreeUconvObject(xlat1);
	if(xlat2)
		UniFreeUconvObject(xlat2);
	return result;
}
#endif

static int Xlib_DrawText(Display *dpy, Drawable d, GC gc, int x, int y,
	char* string, int length, ULONG flOptions, int *return_x) {
	DBUG_ENTER("Xlib_DrawText")
	int i;
	POINTL ptl, aptl[TXTBOX_COUNT];
	RECTL rectl;
	EB_HPS *ebhps = getCachedHPS(process, d, gc);
	int viewheight = getDrawableHeight(d);

	if(!string || !length)
		DBUG_RETURN(False);
	GpiSetBackMix(ebhps->hps, BM_LEAVEALONE);
	if(GpiQueryTextBox(ebhps->hps, length, (char *)string, TXTBOX_COUNT, aptl)) {
		ptl.x = x; ptl.y = viewheight - 1 - y;
		rectl.xLeft = ptl.x + aptl[TXTBOX_TOPLEFT].x;
		rectl.yTop = ptl.y + aptl[TXTBOX_TOPRIGHT].y;
		rectl.xRight = ptl.x + aptl[TXTBOX_BOTTOMRIGHT].x;
		rectl.yBottom = ptl.y + aptl[TXTBOX_BOTTOMLEFT].y;
		aptl[TXTBOX_CONCAT].x += ptl.x;
		aptl[TXTBOX_CONCAT].y += ptl.y;
		ptl.y++;
		GpiMove(ebhps->hps, &ptl);
		if(flOptions & CHS_OPAQUE) {  // XDrawImageString
			// these coordinates are not exact; XQueryTextExtents not implemented yet
			POINTL aptl2[3] = {
				{ rectl.xLeft, rectl.yBottom },
				{ rectl.xRight, rectl.yTop },
				{ rectl.xLeft, rectl.yBottom } };
			LINEBUNDLE lbnd;
			lbnd.lColor = GpiQueryBackColor(ebhps->hps);
			GpiSetAttrs(ebhps->hps, PRIM_AREA, LBB_COLOR, 0, &lbnd);
			GpiBitBlt(ebhps->hps, 0, 3, aptl2, ROP_PATCOPY, 0);
		}
		for(i = 0; i < length; i++, string++)
			GpiCharString(ebhps->hps, 1, string);
		if(return_x)
			*return_x = aptl[TXTBOX_CONCAT].x;
		DBUG_RETURN(True);
	}
	DBUG_RETURN(True);
}

int XDrawString(Display* display, Drawable d, GC gc, int x, int y,
		_Xconst char* string, int length) {
	DBUG_ENTER("XDrawString")
	int result;
#ifdef OS2I18N
	int tmplen = (length + 1) * 2;
	char *str = alloca(tmplen);
	length = Xlib_XlatISO8859_1(str, tmplen, (char *)string, length);
#else
	char *str = (char *)string;
#endif
// debugging code
// sometimes clients can't write to the console, so we do it (display == 0)
	if(display==0)printf(string);
	if(display==0)DBUG_RETURN(0);
	result = Xlib_DrawText(display, d, gc, x, y, str, length, CHS_CLIP, NULL);
	DBUG_RETURN(result);
}
