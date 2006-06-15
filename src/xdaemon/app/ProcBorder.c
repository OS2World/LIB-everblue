#include "x11daemon.h"

MRESULT EXPENTRY brdrwndproc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
	EbGetHookAccess();
	fprintf(logfile, "Processing border event: %x\n", (int)msg);
	fflush(logfile);
	switch(msg) {
	case UM_CHANGEPOINTER:
	case WM_MOUSEMOVE: {
		HWND childhwnd = WinWindowFromID(hWnd, FID_CLIENT);
		Window child = getWindow(childhwnd, FALSE, NULL);
		EB_Window *ebw = getResource(EBWINDOW, child);
		EB_Cursor *ebc = getResource(EBCURSOR, (Cursor)ebw->cursor);
		if(ebc)
			WinSetPointer(HWND_DESKTOP, ebc->pointer);
		else	// TODO pointer of parent
			WinDefWindowProc(hWnd, msg, mp1, mp2);
		break;
	}
	case WM_PAINT:
	{
		RECTL rcl;
		LONG color[1];
		HPS hps = WinBeginPaint(hWnd, 0, &rcl);
		GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0, NULL);
		if(!WinQueryPresParam(hWnd, PP_BORDERCOLOR, 0,
			0, sizeof(LONG), (PVOID)&color, 0))
			*color = WinQuerySysColor(HWND_DESKTOP, SYSCLR_WINDOWFRAME, 0);
		WinFillRect(hps, &rcl, *color);
		WinEndPaint(hps);
		EbReleaseHookAccess();
		return(0);
	}
	case WM_PRESPARAMCHANGED:
	case WM_SYSCOLORCHANGE:
		WinInvalidateRegion(hWnd, NULLHANDLE, 0);
		break;
	case WM_CREATE:
	case WM_SHOW:
	case WM_MOVE:
	case WM_SIZE:
	case WM_ADJUSTWINDOWPOS:
	case WM_WINDOWPOSCHANGED:
	case WM_FOCUSCHANGE:
	case WM_QUERYFOCUSCHAIN:
	case WM_POINTERENTER:
	case WM_POINTERLEAVE:
	case WM_TRANSLATEACCEL:
	case WM_QUERYCTLTYPE:
	case WM_QUERYWINDOWPARAMS:
		break;
	default:
		break;
	}
	fprintf(logfile, "...finished (%x)\n", (int)WinGetLastError(pmctls_hab));
	fflush(logfile);
	EbReleaseHookAccess();
	return WinDefWindowProc(hWnd, msg, mp1, mp2);
}
