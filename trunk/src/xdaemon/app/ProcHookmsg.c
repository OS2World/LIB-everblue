#include "x11daemon.h"

extern int _XminKeyCode, _XmaxKeyCode;

void hook_msg(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2, ULONG time, POINTL ptl) {
		char winclass[32];
		BOOL border;
		HWND clienthwnd = NULLHANDLE;
		_XQEvent *newq, *newq2;
		XEvent *new, *new2;

if(msg != WM_CREATE && msg != WM_MOUSEMOVE) {
	PID pid;
	TID tid;
	char classname[100];
	WinQueryWindowProcess(hwnd, &pid, &tid);
	WinQueryClassName(hwnd, 100, classname);
	fprintf(logfile, "got event: %x (%x) - %x: %s\n", (int)msg, (int)hwnd, (int)pid, classname);
	fflush(logfile);
}
		EbWaitForHookAccess();
		EbReleaseHookDataAccess();

		WinQueryClassName(hwnd, sizeof(winclass), winclass);
		if(!strcmp(winclass, "#1") || !strcmp(winclass, "XPMBorder")) {
			border = TRUE;
			clienthwnd = WinWindowFromID(hwnd, FID_CLIENT);
		} else
			border = FALSE;

		switch(msg) {
		case WM_CHAR: {
			int type, keycode;
			int state;
			BOOL repeat = FALSE;
			Window win = getWindow(hwnd, FALSE, NULL);
			if(!win)
				break;

			// type detection
			if (SHORT1FROMMP(mp1) & KC_KEYUP) {
				type = KeyRelease;
			} else {
				type = KeyPress;
				if (SHORT1FROMMP(mp1) & KC_PREVDOWN) {
					/* Key repeat! */
					if(auto_repeat)
						repeat = TRUE;
					else
						break;
				}
			}
			/* There will probably need to be lots of keycode translations */
			/* Keycode translations happen in XKeycodeToKeysym() funcs */

			// state detection
			state = Xlib_State(SHORT1FROMMP(mp1));

			// keycode (+state) detection
			if (SHORT1FROMMP(mp1) & KC_CHAR && !(SHORT1FROMMP(mp2) & 0xff80))
				keycode = SHORT1FROMMP(mp2);
			else
				if (SHORT1FROMMP(mp1) & KC_CHAR && !(SHORT1FROMMP(mp2) & 0xff00)) {
					keycode = SHORT1FROMMP(mp2);
					state |= Mod1Mask;
				} else
					if (SHORT1FROMMP(mp1) & KC_SCANCODE && CHAR4FROMMP(mp1))
						keycode = CHAR4FROMMP(mp1) | 0x80;
					else
						break;

			if (keycode < _XminKeyCode || keycode > _XmaxKeyCode)
				break;

			if (SHORT1FROMMP(mp1) & KC_SCANCODE)
				switch (CHAR4FROMMP(mp1)) {
				case 0x2a:      /* Shift_L */
				case 0x36:      /* Shift_R */
				case 0x1d:      /* Control_L */
				case 0x5b:      /* Control_R */
				case 0x38:      /* Alt_L */
				case 0x5e:      /* Alt_R */
				case 0x3a:      /* Caps Lock */
				case 0x39:      /* Spacebar */
				case 0x5c:      /* KP_Divide */
				case 0x37:      /* KP_Multiply */
				case 0x4a:      /* KP_Subtract */
				case 0x4e:      /* KP_Add */
				case 0x5a:      /* KP_Enter */
					keycode = CHAR4FROMMP(mp1) | 0x80;
					state &= ~Mod1Mask;
					break;
				case 0x53:      /* KP_Decimal */
				case 0x52:      /* KP_0 */
				case 0x4f:      /* KP_1 */
				case 0x50:      /* KP_2 */
				case 0x51:      /* KP_3 */
				case 0x4b:      /* KP_4 */
				case 0x4c:      /* KP_5 */
				case 0x4d:      /* KP_6 */
				case 0x47:      /* KP_7 */
				case 0x48:      /* KP_8 */
				case 0x49:      /* KP_9 */
					keycode = CHAR4FROMMP(mp1) | 0x80;
					if (((WinGetKeyState(HWND_DESKTOP, VK_NUMLOCK)&0x0001)==1) ==
						((state & ShiftMask) == ShiftMask)) {
						keycode &= 0x7f;
						state |= Mod1Mask;
					}
					break;
				}

			new = Daemon_createEvent(&newq, win, type);
			new->xkey.window = win;
			new->xkey.subwindow = None;
			new->xkey.time = WinQueryMsgTime(pmctls_hab);
			_XQueryPointer(win, &new->xkey.root, NULL, &new->xkey.x_root,
				&new->xkey.y_root, &new->xkey.x, &new->xkey.y, NULL, &ptl);
			new->xkey.state = state;
			new->xkey.keycode = keycode;
			new->xkey.same_screen = TRUE;

			if(repeat) {
				new2 = Daemon_copyEvent(&newq2, newq);
				new2->type = KeyRelease;
				Daemon_propagateEvent(newq2, KeyReleaseMask, 0);
			}
			if(new->type == KeyRelease)
				Daemon_propagateEvent(newq, KeyReleaseMask, 0);
			else
				Daemon_propagateEvent(newq, KeyPressMask, 0);

			break;
		}
		case WM_POINTERENTER:
		case WM_POINTERLEAVE:
		{
			if(LONGFROMMP(mp1) != hwnd)
				break;
		
			HWND ancestor = hwnd = getValidWindow(hwnd);
			Window win = getWindow(hwnd, FALSE, NULL);
			HWND other = getValidWindow(LONGFROMMP(mp2));
			if(!win || !other)
				break;

			while(ancestor && !WinIsChild(other, ancestor))
				ancestor = WinQueryWindow(ancestor, QW_PARENT);
			ancestor = getValidWindow(ancestor);

			POINTL ptl;
			WinQueryMsgPos(pmctls_hab, &ptl);

			new = Daemon_createEvent(&newq, win, (msg == WM_POINTERENTER)?EnterNotify:LeaveNotify);
			new->xcrossing.root = Daemon_RootWindow();
			new->xcrossing.subwindow = None;
			new->xcrossing.time = WinQueryMsgTime(pmctls_hab);
			new->xcrossing.x_root = ptl.x;
			new->xcrossing.y_root = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - ptl.y - 1;
			WinMapWindowPoints(HWND_DESKTOP, hwnd, &ptl, 1);
			SWP swp;
			WinQueryWindowPos(hwnd, &swp);
			new->xcrossing.x = ptl.x;
			new->xcrossing.y = swp.cy - ptl.y - 1;
			new->xcrossing.mode = NotifyNormal;
			new->xcrossing.detail = NotifyNonlinear;
			new->xcrossing.same_screen = 1;
			new->xcrossing.focus = WinIsChild(hwnd, WinQueryFocus(HWND_DESKTOP));
			new->xcrossing.state = Xlib_State(-1);
			HWND parent = hwnd;
			Window subwindow = win;
			while(parent != HWND_DESKTOP && (parent = getParent(parent)) != ancestor) {
				new2 = Daemon_copyEvent(&newq2, newq);
				Window parentwin = getWindow(parent, TRUE, NULL);
				new2->xany.window = parentwin;
				new2->xcrossing.subwindow = subwindow;
				subwindow = NULLHANDLE;
				WinQueryMsgPos(pmctls_hab, &ptl);
				WinMapWindowPoints(HWND_DESKTOP, parent, &ptl, 1);
				WinQueryWindowPos(parent, &swp);
				new2->xcrossing.x = ptl.x;
				new2->xcrossing.y = swp.cy - ptl.y - 1;
				new2->xcrossing.focus = WinIsChild(parent, WinQueryFocus(HWND_DESKTOP));
				new2->xcrossing.detail = NotifyNonlinearVirtual;
				EB_Window *ebw = getResource(EBWINDOW, parentwin);
				Daemon_doEvent(newq2, ebw->event_masks, (msg==WM_POINTERENTER)?EnterWindowMask:LeaveWindowMask);
			}
			EB_Window *ebw = getResource(EBWINDOW, win);
			Daemon_doEvent(newq, ebw->event_masks, (msg==WM_POINTERENTER)?EnterWindowMask:LeaveWindowMask);

			break;
		}
		case WM_MOUSEMOVE:
		case WM_BUTTON1MOTIONSTART:
		case WM_BUTTON2MOTIONSTART:
		case WM_BUTTON3MOTIONSTART:
		case WM_BUTTON1MOTIONEND:
		case WM_BUTTON2MOTIONEND:
		case WM_BUTTON3MOTIONEND:
		case WM_SEM4:
		{
			static POINTL old = { -1, -1 };
//			BOOL weakEvent = (msg == WM_MOUSEMOVE);
			BOOL weakEvent = FALSE;
			POINTL ptls = {SHORT1FROMMP(mp1), SHORT2FROMMP(mp1)};
			if(ptls.x == old.x && ptls.y == old.y)
				break;
			else
				old = ptls;
			Window win = getWindow(hwnd, FALSE, NULL);
			if(!win)
				break;

			int btnmask = PointerMotionMask;
			if(WinGetKeyState(HWND_DESKTOP, VK_BUTTON1) & 0x8000)
				btnmask |= Button1MotionMask | ButtonMotionMask;
			if(WinGetKeyState(HWND_DESKTOP, VK_BUTTON2) & 0x8000)
				btnmask |= Button3MotionMask | ButtonMotionMask;
			if(WinGetKeyState(HWND_DESKTOP, VK_BUTTON3) & 0x8000)
				btnmask |= Button2MotionMask | ButtonMotionMask;

			new = Daemon_createEvent(&newq, win, MotionNotify);
			new->xmotion.state = Xlib_State(-1);
			new->xmotion.root = Daemon_RootWindow();
			new->xmotion.subwindow = None;
			new->xmotion.time = WinQueryMsgTime(pmctls_hab);
			new->xmotion.x = ptls.x;
			SWP swp;
			WinQueryWindowPos(hwnd, &swp);
			new->xmotion.y = swp.cy - ptls.y - 1;
			WinMapWindowPoints(hwnd, HWND_DESKTOP, &ptls, 1);
			new->xmotion.x_root = ptls.x;
			new->xmotion.y_root = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - ptls.y - 1;
			new->xmotion.is_hint = NotifyNormal;
			new->xmotion.same_screen = TRUE;
			Daemon_propagateEvent(newq, btnmask, weakEvent);
			break;
		}
		case WM_BUTTON1UP:
		case WM_BUTTON2UP:
		case WM_BUTTON3UP:
		case WM_BUTTON1DOWN:
		case WM_BUTTON1DBLCLK:
		case WM_BUTTON2DOWN:
		case WM_BUTTON2DBLCLK:
		case WM_BUTTON3DOWN:
		case WM_BUTTON3DBLCLK:
		{
			int type, button;
			Window win = getWindow(hwnd, FALSE, NULL);
			if(!win)
				break;
			POINTL ptls = {SHORT1FROMMP(mp1), SHORT2FROMMP(mp1)};

			switch(msg) {
			case WM_BUTTON1UP:
				type = ButtonRelease;
				button = 1;
				break;
			case WM_BUTTON2UP:
				type = ButtonRelease;
				button = 3;
				break;
			case WM_BUTTON3UP:
				type = ButtonRelease;
				button = 2;
				break;
			case WM_BUTTON1DOWN:
			case WM_BUTTON1DBLCLK:
				type = ButtonPress;
				button = 1;
				break;
			case WM_BUTTON2DOWN:
			case WM_BUTTON2DBLCLK:
				type = ButtonPress;
				button = 3;
				break;
			case WM_BUTTON3DOWN:
			case WM_BUTTON3DBLCLK:
				type = ButtonPress;
				button = 2;
				break;
			default:
				type = button = 0;
			}

	fprintf(logfile, "type: %x, hwnd: %x, Focus: %x\n", type == ButtonPress, hwnd, WinQueryFocus(HWND_DESKTOP));
	fflush(logfile);
//			if(type == ButtonPress && !(WinQueryFocus(HWND_DESKTOP) == hwnd))
//					WinSetFocus(HWND_DESKTOP, hwnd);

			new = Daemon_createEvent(&newq, win, type);
			new->xbutton.root = (Window)Daemon_RootWindow();
			new->xbutton.subwindow = None;
			new->xbutton.time = WinQueryMsgTime(pmctls_hab);
			new->xbutton.x = ptls.x;
			SWP swp;
			WinQueryWindowPos(hwnd, &swp);
			new->xbutton.y = swp.cy - ptls.y - 1;
			WinMapWindowPoints(hwnd, HWND_DESKTOP, &ptls, 1);
			new->xbutton.x_root = ptls.x;
			new->xbutton.y_root = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - ptls.y - 1;
			new->xbutton.state = Xlib_State(SHORT1FROMMP(mp1));
			new->xbutton.button = button;
			new->xbutton.same_screen = True;
			Daemon_propagateEvent(newq, (type==ButtonPress)?ButtonPressMask:ButtonReleaseMask, 0);
			break;
		}
		case WM_CREATE: {
			HWND parenthwnd;
			Window parentwin, window;
			EB_Window *parentebw, *ebw;
			XRectangle position;

			if(border)
				break;
			parenthwnd = getParent(hwnd);
			if(!parenthwnd || !(parentwin = getWindow(parenthwnd, FALSE, NULL)))
				break;
			if(!strcmp(winclass, "XPMChild"))
				ebw = msgparameter;
			else
				ebw = NULL;
			window = getWindow(hwnd, TRUE, ebw);

			parentebw = getResource(EBWINDOW, parentwin);
			if(!(parentebw->event_mask & SubstructureNotifyMask))
				break;
			ebw = getResource(EBWINDOW, window);
			getPosition(&position, hwnd, parenthwnd);

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
					(int)window, (int)hwnd, (int)parentwin, (int)parenthwnd);
			fflush(logfile);
			break;
		}

		case WM_DESTROY: {
			removeWindow(hwnd);
			break;
		}
		case WM_SHOW: {
			Window parentwin, window;
			EB_Window *parentebw, *ebw;
			HWND parenthwnd = getParent(hwnd);

			if(!(parentwin = getWindow(parenthwnd, FALSE, NULL)))
				break;
			parentebw = getResource(EBWINDOW, parentwin);
			if(!(parentebw->event_mask & SubstructureNotifyMask))
				break;
			window = getWindow(hwnd, TRUE, NULL);
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
		case WM_SETFOCUS:
		{
			int type;
//			HWND pointerWindow;
//			POINTL ptl;
//			HWND other = LONGFROMMP(mp1);
//			HWND ancestor = hwnd;
//			HWND parent = getParent(hwnd);
			Window window;
			EB_Window *ebw;

			if(border)
				break;
			window = getWindow(hwnd, FALSE, NULL);
			ebw = getResource(EBWINDOW, window);
			if(!ebw)
				break;

//			while(!WinIsChild(other, ancestor = getParent(ancestor)) && ancestor != HWND_DESKTOP);
			if (SHORT1FROMMP(mp2)==TRUE) {
				type = FocusIn;
			} else {
				type = FocusOut;
			}
			new = Daemon_createEvent(&newq, window, type);
			new->xfocus.mode = NotifyNormal;
			new->xfocus.detail = NotifyNonlinear;
			Daemon_doEvent(newq, ebw->event_masks, FocusChangeMask);
#if 0
			if(parent != ancestor)
			{
				new2 = Daemon_createEvent(&newq2, parent, type);
				new2->xfocus.mode = NotifyNormal;
				new2->xfocus.detail = NotifyNonlinearVirtual;
				Daemon_recurseEvent(newq2, new2, FocusChangeMask, ancestor);
			}
			WinQueryMsgPos(pmctls_hab, &ptl);
			pointerWindow = WinWindowFromPoint(hwnd, &ptl, TRUE);
			if(pointerWindow && pointerWindow != hwnd)
			{
				new3 = Daemon_createEvent(&newq3, pointerWindow, type);
				new3->xfocus.mode = NotifyNormal;
				new3->xfocus.detail = NotifyPointer;
				Daemon_recurseEvent(newq3, new3, FocusChangeMask, hwnd);
			}
#endif
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
			window = getWindow(hwnd, FALSE, NULL);
			ebw = getResource(EBWINDOW, window);
			if(!ebw)
				break;

			WinQueryWindowPos(hwnd, &swp);

			if(!strcmp(winclass, "XPMChild")) {
				hps = WinBeginPaint(hwnd, NULLHANDLE, &rectl);
				if(rectl.xRight || rectl.yTop) {
					GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0, NULL);
					WinFillRect(hps, &rectl, ebw->background_pixel);
					WinEndPaint(hps);
				} else {
					WinEndPaint(hps);
					break;
				}
			} else
				if(!WinQueryUpdateRect(hwnd, &rectl))
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


		// quick hack for rdesktop
		// later use WinSetVisibleRegionNotify()

		if(msg == WM_PAINT || msg == WM_SHOW) {
			Window window = getWindow(hwnd, FALSE, NULL);
			EB_Window *ebw = getResource(EBWINDOW, window);
			if(window) {	
				new = Daemon_createEvent(&newq, window, VisibilityNotify);
				new->xvisibility.state = VisibilityPartiallyObscured;
				Daemon_doEvent(newq, ebw->event_masks, VisibilityChangeMask);
			}
		}

		EbReleaseHookAccess();
}
