#include "x11daemon.h"

// TODO InputOnly

// have to create frame window manually
// to get proper WM_SHOW messages

Window um_createWindow(UM_CreateWindow *args) {
	HPOINTER tmpicon = 0;
	HMODULE xmod = 0;
	ULONG flStyle = 0;
	ULONG id;
	PVOID pCtlData;
	PSZ pszTitle = "Xlib/PM";
	HWND hwndClient, hwndBorder;
	EB_Window *parent = getResource(EBWINDOW, args->parent);
	BOOL decorated = !args->newwind->override_redirect && parent->hwnd == HWND_DESKTOP;
	int titlebar;
	SWP parentpos;
	PSZ pszClass;
	FRAMECDATA framectl = { sizeof(FRAMECDATA),
			FCF_TITLEBAR | FCF_SYSMENU | FCF_MINMAX | FCF_SIZEBORDER, 0, 0 };
	ULONG presmain[19] = { 36, PP_INACTIVECOLOR, 4, args->newwind->border_pixel,
		PP_ACTIVECOLOR, 4, args->newwind->border_pixel,
		PP_BORDERLIGHTCOLOR, 4, args->newwind->border_pixel };

	Window windowres;

fprintf(logfile, "Creating window (%x x %x), %x\n", args->width, args->height, args->newwind->override_redirect);
fflush(logfile);
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

	if(args->newwind->class == InputOnly) {
		pszClass = "XPMInput";
		titlebar = 0;
		id = FID_INPUT;
		pCtlData = args->newwind;
	} else if(decorated) {
		pszClass = WC_FRAME;
		titlebar = WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);
		id = FID_FRAME;
		pCtlData = &framectl;
	} else {
		pszClass = "XPMBorder";
		titlebar = 0;
		id = FID_BORDER;
		pCtlData = NULL;
	}

	WinQueryWindowPos(parent->hwnd, &parentpos);
fprintf(logfile, "Parent (%x, %x, %x, %x): %x\n", parentpos.x, parentpos.y, parentpos.cx, parentpos.cy, parent->hwnd);
fprintf(logfile, "create with size: %x (%x), position: %x\n", args->height + 2 * args->newwind->border_width + titlebar, args->height, parentpos.cy - args->height - args->newwind->border_width - args->y - titlebar);
fprintf(logfile, "position calc: %x - %x - %x - %x - %x\n", parentpos.cy, args->height, args->newwind->border_width, args->y, titlebar);
fflush(logfile);
	hwndBorder = WinCreateWindow(parent->hwnd, pszClass, pszTitle, flStyle,
			0, 0, 0, 0, NULLHANDLE, HWND_TOP, id, pCtlData, &presmain);
	if(decorated) {
		if(!args->newwind->border_width)
			args->newwind->border_width = 1;
		WinSendMsg(hwndBorder, WM_SETBORDERSIZE, (MPARAM)args->newwind->border_width, (MPARAM)args->newwind->border_width);
		pfnFrame = WinSubclassWindow(hwndBorder, framewndproc);
		DosQueryModuleHandle("xdaemon", &xmod);
		if(xmod && (tmpicon = WinLoadPointer(HWND_DESKTOP, xmod, PMXLIB_DEFAULT_ICON)))
			WinSendMsg(hwndBorder, WM_SETICON, (MPARAM)tmpicon, 0);
	}
	WinSetWindowPos(hwndBorder, 0,
			args->x - args->newwind->border_width,
			parentpos.cy - args->height - args->newwind->border_width - args->y - titlebar,
			args->width + 2 * args->newwind->border_width,
			args->height + 2 * args->newwind->border_width + titlebar, SWP_SIZE | SWP_MOVE);

	if(args->newwind->class == InputOnly)
		windowres = getWindow(hwndBorder, TRUE, NULL);
	else {
		hwndClient = WinCreateWindow(hwndBorder, "XPMChild", pszTitle,
				flStyle | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
				args->newwind->border_width,
				args->newwind->border_width,
				args->width,
				args->height,
				NULLHANDLE,
				HWND_TOP, FID_CLIENT, args->newwind, NULL);
		windowres = getWindow(hwndClient, TRUE, NULL);
	}

	addResource(&args->process->ebprocess->res, (EB_Resource *)windowres);
	if(args->event_mask)
		Daemon_addEventMask((XID)windowres, args->process,
				args->event_mask);

	return windowres;
}
