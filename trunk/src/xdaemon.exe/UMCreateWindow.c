#include "x11daemon.h"

// have to create frame window manually
// to get proper WM_SHOW messages

Window um_createWindow(UM_CreateWindow *args) {
	HPOINTER tmpicon = 0;
	HMODULE xmod = 0;
	ULONG flStyle = 0;
	PSZ pszTitle = "Xlib/PM";
	HWND hwndClient, hwndBorder;
	EB_Window *parent = getResource(EBWINDOW, args->parent);
	BOOL decorated = !args->newwind->override_redirect && parent->hwnd == HWND_DESKTOP;
	int borderwidth, borderheight, titlebar;
	SWP parentpos, border;
	PSZ pszClass;
	FRAMECDATA framectl = { sizeof(FRAMECDATA),
			FCF_TITLEBAR | FCF_SYSMENU | FCF_MINMAX | FCF_SIZEBORDER, 0, 0 };
	ULONG presmain[19] = { 36, PP_INACTIVECOLOR, 4, args->newwind->border_pixel,
		PP_ACTIVECOLOR, 4, args->newwind->border_pixel,
		PP_BORDERLIGHTCOLOR, 4, args->newwind->border_pixel };

	Window windowres;

	if(args->newwind->background_pixmap)
		monitorResource((EB_Resource *)args->newwind->background_pixmap);
	if(args->newwind->cursor)
		monitorResource((EB_Resource *)args->newwind->cursor);
	if(args->newwind->border_pixel == -1)
		args->newwind->border_pixel = 0x808080; // RGB_DARKGRAY
	if(args->newwind->save_under)
		flStyle |= WS_SAVEBITS;
	if(!args->newwind->override_redirect)
		framectl.flCreateFlags |= FCF_TASKLIST | FCF_BORDER | FCF_NOBYTEALIGN;

	if(decorated) {
		pszClass = WC_FRAME;
		borderwidth = WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);
		borderheight = WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
		titlebar = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
	} else {
		pszClass = "XPMBorder";
		borderwidth = borderheight = args->newwind->border_width;
		titlebar = 0;
	}

	WinQueryWindowPos(parent->hwnd, &parentpos);
	hwndBorder = WinCreateWindow(parent->hwnd, pszClass, pszTitle, flStyle,
			args->x - borderwidth,
			parentpos.cy - args->height - borderheight - args->y - titlebar,
			0, 0,
			NULLHANDLE, HWND_TOP, FID_BORDER,
			(pszClass == WC_FRAME ? &framectl : NULL), &presmain);
	WinSetWindowPos(hwndBorder, 0, 0, 0,
			args->width + 2 * borderwidth,
			args->height + 2 * borderheight + titlebar, SWP_SIZE);
	if(decorated) {
		WinSendMsg(hwndBorder, WM_SETBORDERSIZE, (MPARAM)args->newwind->border_width, (MPARAM)args->newwind->border_width);
		pfnFrame = WinSubclassWindow(hwndBorder, framewndproc);
		DosQueryModuleHandle("xdaemon", &xmod);
		if(xmod && (tmpicon = WinLoadPointer(HWND_DESKTOP, xmod, PMXLIB_DEFAULT_ICON)))
			WinSendMsg(hwndBorder, WM_SETICON, (MPARAM)tmpicon, 0);
	}
	WinQueryWindowPos(hwndBorder, &border);
	hwndClient = WinCreateWindow(hwndBorder, "XPMChild", pszTitle,
			flStyle | WS_VISIBLE,
			args->newwind->border_width,
			args->newwind->border_width,
			border.cx - 2 * args->newwind->border_width,
			border.cy - 2 * args->newwind->border_width - titlebar,
			NULLHANDLE,
			HWND_TOP, FID_CLIENT, args->newwind, NULL);

	windowres = getWindow(hwndClient, FALSE, NULL);

	addResource(&args->process->ebprocess->res, (EB_Resource *)windowres);
	if(args->event_mask)
		Daemon_addEventMask((XID)windowres, args->process,
				args->event_mask);

	return windowres;
}
