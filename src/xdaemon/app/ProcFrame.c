#include "x11daemon.h"

MRESULT EXPENTRY framewndproc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
	EbGetHookAccess();
	MRESULT result = (MRESULT)0;

//	fprintf(logfile, "Processing framewnd event: %x\n", (int)msg);
//	fflush(logfile);

	switch(msg) {
	case WM_CALCVALIDRECTS: {
		Window w = getWindow(WinWindowFromID(hWnd, FID_CLIENT), FALSE, NULL);
		EB_Window *ebw = getResource(EBWINDOW, w);

		if(!ebw || ebw->bit_gravity == ForgetGravity)
			result = (MRESULT)CVR_REDRAW;
		else {
			RECTL *oldRect = mp1;
			RECTL *newRect = oldRect + 1;

			*newRect = *oldRect;
			result = 0;
			if(ebw->bit_gravity = StaticGravity)
				break;

			SWP *swp = mp2;
			int dx = swp->x - oldRect->xLeft;
			int dy = swp->y - oldRect->yBottom;

			dx += CalcXDelta(oldRect->xRight - oldRect->xLeft, swp->cx, ebw->bit_gravity);
			dy += CalcYDelta(oldRect->yTop - oldRect->yBottom, swp->cy, ebw->bit_gravity);

			if(dx || dy)
				WinOffsetRect(pmctls_hab, newRect, dx, dy);
		}
		break;
	}
	default:
		result = pfnFrame(hWnd, msg, mp1, mp2);
	}
//	fprintf(logfile, "...finished (%x)\n", (int)WinGetLastError(pmctls_hab));
//	fflush(logfile);
	EbReleaseHookAccess();
	return result;
}
