#include "daemondll.h"

void freeHPS(EB_Resource *hpsres2) {
	if(hpsres2->ebhps->hdc) {
	} else
		WinReleasePS(hpsres2->ebhps->hps);
}

EXPENTRY void freeOldHPS(EB_Resource **anchor) {
	EB_Resource *current = *anchor;

	if(current) {
		EB_Resource *next = current->next;
		sfree(current);
		while((current = next)) {
			next = current->next;
			unChainResource(current->ebresource);
			freeHPS(current);
			freeResource(current);
		}
		sfree(current);
		*anchor = NULL;
	}
}

EXPENTRY int getDrawableHeight(Drawable d) {
	if(((EB_Resource *)d)->restype == EBWINDOW) {
		SWP swp;
		EB_Window *ebw = getResource(EBWINDOW, d);
		WinQueryWindowPos(ebw->hwnd, &swp);
		return swp.cy;
	}
	EB_Pixmap *ebp = getResource(EBPIXMAP, d);
	return ebp->pbmih->cy;
}

EXPENTRY void getDrawableGeometry(Drawable d, int *x, int *y, unsigned int *width,
		unsigned int *height, unsigned int *border_width, unsigned int *depth) {
	XRectangle position;
	EB_Window *ebw = getResource(EBWINDOW, d);
	HWND parenthwnd = getParent(ebw->hwnd);
	
	if(parenthwnd)
		getPosition(&position, ebw->hwnd, parenthwnd);
	else
		getPosition(&position, ebw->hwnd, HWND_DESKTOP);

	if(x)
		*x = position.x;
	if(y)
		*y = position.y;
	if(width)
		*width = position.width;
	if(height)
		*height = position.height;
	if(border_width)
		*border_width = ebw->border_width;
	if(depth)
		*depth = 32;
}

EB_HPS *newCachedHPS(EB_Resource *procres, Drawable d) {
	EB_HPS *ebhps = scalloc(1, sizeof(EB_HPS));
	EB_Resource *hpsres1 = createResource(EBHPS1, NULL);
	EB_Resource *hpsres2 = createResource(EBHPS2, hpsres1);

	freeOldHPS(&procres->ebprocess->hpstodelete);
	hpsres1->ebresource = hpsres2;
	hpsres1->procres = procres;
	hpsres2->ebhps = ebhps;
	// TODO initialize ebhps->current
	addResource(&procres->ebprocess->hpsres, hpsres2);

	if(((EB_Resource *)d)->restype == EBWINDOW) {
		EB_Window *ebw = getResource(EBWINDOW, d);

		ebhps->hps = WinGetPS(ebw->hwnd);
		addResource(&ebw->hpscache, hpsres1);
	} else {
	    DEVOPENSTRUC dop = { 0L, "DISPLAY" };
    	SIZEL sizl = {0, 0};
		EB_Pixmap *ebp = getResource(EBPIXMAP, d);
		HAB hab;

		Daemon_getPMHandle(procres, &hab, NULL);
		ebhps->hdc = DevOpenDC(hab, OD_MEMORY, "*", 2L, (PDEVOPENDATA)&dop, NULLHANDLE);
		ebhps->hps = GpiCreatePS(hab, ebhps->hdc, &sizl, PU_PELS | GPIT_MICRO | GPIA_ASSOC);
// TODO GpiCreateBitmap / caching
//		ebhps->hbm =
		addResource(&ebp->hpscache, hpsres1);
	}

	GpiCreateLogColorTable(ebhps->hps, 0, LCOLF_RGB, 0, 0, NULL);
	return ebhps;
}

EXPENTRY EB_HPS *getCachedHPS(EB_Resource *procres, Drawable d, GC gc) {
	EB_Resource *current;
	EB_Resource *drawable = (EB_Resource *)d;
	EB_HPS *ebhps = NULL;

	if(drawable->restype == EBWINDOW)
		current = drawable->ebwindow->hpscache;
	else
		current = drawable->ebpixmap->hpscache;
	if(current)
		while((current = current->next)) {
			if(current->procres == procres) {
				EB_Resource *current2 = getResource(EBHPS1, (XID)current);
				ebhps = current2->ebhps;
				if(gc)
					setGCValues(ebhps, &gc->values, False);
				return ebhps;
			}
		}
	ebhps = newCachedHPS(procres, d);
	if(gc)
		setGCValues(ebhps, &gc->values, True);
	else
		setGCValues(ebhps, &stdgcvalues, True);
	return ebhps;
}
