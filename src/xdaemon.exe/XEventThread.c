#include "x11daemon.h"

void xevent_thread(void *args) {
	HAB xeventhab;

	xeventhab = WinInitialize(0);
	fprintf(logfile, "--- XEvent thread ready ---\n");
	fflush(logfile);
	while(1) {
		char winclass[32];
		BOOL border;
		HWND clienthwnd = NULLHANDLE;
		EB_Resource *newq, *newq2;
		XEvent *new, *new2;

		releaseReadAccess(&xeventsem);
		getReadAccess(&xeventsem);

if(msgxchange.msg != WM_CREATE) {
	PID pid;
	TID tid;
	char classname[100];
	WinQueryWindowProcess(msgxchange.hwnd, &pid, &tid);
	WinQueryClassName(msgxchange.hwnd, 100, classname);
	fprintf(logfile, "got event: %x (%x) - %x: %s\n", (int)msgxchange.msg, (int)msgxchange.hwnd, (int)pid, classname);
	fflush(logfile);
}
		mutex_lock(global_lock, TRUE);

		WinQueryClassName(msgxchange.hwnd, sizeof(winclass), winclass);
		if(!strcmp(winclass, "#1") || !strcmp(winclass, "XPMBorder")) {
			border = TRUE;
			clienthwnd = WinWindowFromID(msgxchange.hwnd, FID_CLIENT);
		} else
			border = FALSE;

		switch(msgxchange.msg) {
		case WM_CREATE: {
			HWND parenthwnd;
			Window parentwin, window;
			EB_Window *parentebw, *ebw;
			XRectangle position;

			if(border)
				break;
			parenthwnd = getParent(msgxchange.hwnd);
			if(!parenthwnd || !(parentwin = getWindow(parenthwnd, FALSE, NULL)))
				break;
			if(!strcmp(winclass, "XPMChild"))
				ebw = msgparameter;
			else
				ebw = NULL;
			window = getWindow(msgxchange.hwnd, TRUE, ebw);

			parentebw = getResource(EBWINDOW, parentwin);
			if(!(parentebw->event_mask & SubstructureNotifyMask))
				break;
			ebw = getResource(EBWINDOW, window);
			getPosition(&position, msgxchange.hwnd, parenthwnd);

			new = Daemon_createEvent(&newq, parentwin, CreateNotify);
			new->xcreatewindow.window = window;
			new->xcreatewindow.x = position.x;
			new->xcreatewindow.y = position.y;
			new->xcreatewindow.width = position.width;
			new->xcreatewindow.height = position.height;
			new->xcreatewindow.border_width = ebw->border_width;
			new->xcreatewindow.override_redirect = ebw->override_redirect;
			Daemon_doEvent(newq, parentebw->event_masks, SubstructureNotifyMask);
			fprintf(logfile, "CreateNotify for window %x (%x), parent %x (%x)\n",
					(int)window, (int)msgxchange.hwnd, (int)parentwin, (int)parenthwnd);
			fflush(logfile);
			break;
		}

		case WM_DESTROY: {
			removeWindow(msgxchange.hwnd);
		}
		case WM_SHOW: {
			Window parentwin, window;
			EB_Window *parentebw, *ebw;
			HWND parenthwnd = getParent(msgxchange.hwnd);

			if(!(parentwin = getWindow(parenthwnd, FALSE, NULL)))
				break;
			parentebw = getResource(EBWINDOW, parentwin);
			if(!(parentebw->event_mask & SubstructureNotifyMask))
				break;
			window = getWindow(msgxchange.hwnd, TRUE, NULL);
			ebw = getResource(EBWINDOW, window);

			new = Daemon_createEvent(&newq, window, MapNotify);
			new->xmap.window = window;
			new->xmap.override_redirect = ebw->override_redirect;
			Daemon_doEvent(newq, ebw->event_masks, StructureNotifyMask);
			new2 = Daemon_createEvent(&newq2, parentwin, MapNotify);
			new2->xmap.window = window;
			new2->xmap.override_redirect = ebw->override_redirect;
			Daemon_doEvent(newq2, parentebw->event_masks, SubstructureNotifyMask);
			break;
		}
		case WM_PAINT: {
				// TODO give back region -> xexpose.count != 0
			RECTL rectl;
			SWP swp;
			int x, y, width, height;
			HPS hps;
			Window window;
			EB_Window *ebw;

			if(border)
				break;
			window = getWindow(msgxchange.hwnd, FALSE, NULL);
			ebw = getResource(EBWINDOW, window);
			if(!ebw)
				break;

			WinQueryWindowPos(msgxchange.hwnd, &swp);

			if(!strcmp(winclass, "XPMChild")) {
				hps = WinBeginPaint(msgxchange.hwnd, NULLHANDLE, &rectl);
				if(rectl.xRight || rectl.yTop) {
					GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0, NULL);
					WinFillRect(hps, &rectl, ebw->background_pixel);
					WinEndPaint(hps);
				} else {
					WinEndPaint(hps);
					break;
				}
			} else
				if(!WinQueryUpdateRect(msgxchange.hwnd, &rectl))
					break;

			x = rectl.xLeft;
			y = swp.cy - rectl.yTop;
			width = rectl.xRight - rectl.xLeft;
			height = rectl.yTop - rectl.yBottom;

			/* else
			if (!attrib->inhiding) {
				HWND hwndframe = WinQueryWindowULong(hWnd, QWP_FRAMEHWND);
				HWND parent = WinQueryWindowULong(hWnd, QWP_PARENT);
				RECTL rectl1 = {0, 0, attrib->winattrib.width, attrib->winattrib.height};
				RECTL rectl2 = {0, 0, attrib->winattrib.width, attrib->winattrib.height};
				WinMapWindowPoints(hWnd, parent, (PPOINTL)&rectl1, 2);
				WinEnableWindowUpdate(hwndframe, FALSE);
				WinInvalidateRect(parent, &rectl1, TRUE);
				attrib->inhiding = 1;
				WinUpdateWindow(parent);
			}*/

			new = Daemon_createEvent(&newq, window, Expose);
			new->xexpose.x = x;
			new->xexpose.y = y;
			new->xexpose.width = width;
			new->xexpose.height = height;
			new->xexpose.count = 0;
			Daemon_doEvent(newq, ebw->event_masks, ExposureMask);

			break;
		}
		}
		mutex_unlock(global_lock);
	}
}
