#include "x11daemon.h"

HPOINTER um_createPointer(UM_CreatePointer *args) {
	POINTERINFO pptriPointerInfo;
	memset(&pptriPointerInfo, 0, sizeof(POINTERINFO));
	pptriPointerInfo.fPointer = TRUE;
	pptriPointerInfo.xHotspot = args->x;
	pptriPointerInfo.yHotspot = args->y;

	HPS hps = WinGetScreenPS(HWND_DESKTOP);
	EB_Pixmap *pebp = getResource(EBPIXMAP, args->pointer);
	EB_Pixmap *cebp = getResource(EBPIXMAP, args->color);
	HBITMAP pointerhbm = GpiCreateBitmap(hps, (PBITMAPINFOHEADER2)pebp->pbmih, CBM_INIT, pebp->data, (PBITMAPINFO2)pebp->pbmih);
	HBITMAP colorhbm = GpiCreateBitmap(hps, (PBITMAPINFOHEADER2)cebp->pbmih, CBM_INIT, cebp->data, (PBITMAPINFO2)cebp->pbmih);
	WinReleasePS(hps);

	pptriPointerInfo.hbmPointer = pointerhbm;
	pptriPointerInfo.hbmColor = colorhbm;
	HPOINTER hpointer = WinCreatePointerIndirect(HWND_DESKTOP, &pptriPointerInfo);
	GpiDeleteBitmap(pointerhbm);
	GpiDeleteBitmap(colorhbm);
	return hpointer;
}
