#include "x11daemon.h"

MRESULT EXPENTRY framewndproc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
	mutex_lock(global_lock, FALSE);
	MRESULT result = (MRESULT)0;

//	fprintf(logfile, "Processing framewnd event: %x\n", (int)msg);
//	fflush(logfile);

	switch(msg) {
	case WM_CALCVALIDRECTS: {
		// OS/2 supports sophisticated sizing of windows:
		// windows are repainted while dragging the mouse
		// X11 programs have problems for elements in
		// the center of windows due to rounding errors
		// I use an algorithm that completely eliminates those errors
		// -> ddcx, ddcy

		Window w = getWindow(WinWindowFromID(hWnd, FID_CLIENT), FALSE, NULL);
		EB_Window *ebw = getResource(EBWINDOW, w);
		RECTL *oldRect = mp1;
		RECTL *newRect = oldRect + 1;
		SWP *swp = mp2;
		int dcx = newRect->xRight - newRect->xLeft - oldRect->xRight + oldRect->xLeft;
		int ddcx = (dcx > 0 ? 1 : -1) * (1 - (swp->cx % 2));
		int dcy = newRect->yBottom - newRect->yTop - oldRect->yBottom + oldRect->yTop;
		int ddcy = (dcy > 0 ? 1 : -1) * (1 - (swp->cy % 2));

		pfnFrame(hWnd, msg, mp1, mp2);
		// only newRect is clipped on the right side - correct it
		newRect->xRight = newRect->xLeft + oldRect->xRight - oldRect->xLeft;

		if(!ebw || ebw->bit_gravity == ForgetGravity) {
			result = (MRESULT)CVR_REDRAW;
			break;
		} else {
			int dx = 0;
			int dy = 0;

			switch(ebw->bit_gravity) {
			case EastGravity:
			case SouthEastGravity:
			case NorthEastGravity:
				dx = dcx;
				break;
			case CenterGravity:
			case SouthGravity:
			case NorthGravity:
				dx = (dcx + ddcx) / 2;
				break;
			case StaticGravity:
				dx = oldRect->xLeft - newRect->xLeft;
				break;
			}

			switch(ebw->bit_gravity) {
			case SouthGravity:
			case SouthWestGravity:
			case SouthEastGravity:
				dy = dcy;
				break;
			case CenterGravity:
			case WestGravity:
			case EastGravity:
				dy = (dcy + ddcy) / 2;
				break;
			case StaticGravity:
				dy = oldRect->yTop - newRect->yTop;
				break;
			}

			WinOffsetRect(pmctls_hab, newRect, dx, dy);
			result = 0;
			break;
		}
	}
	default:
		result = pfnFrame(hWnd, msg, mp1, mp2);
	}
//	fprintf(logfile, "...finished (%x)\n", (int)WinGetLastError(pmctls_hab));
//	fflush(logfile);
	mutex_unlock(global_lock);
	return result;
}
