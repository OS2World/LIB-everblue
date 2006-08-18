#include "x11daemon.h"

// TODO InputOnly

// TODO cannot handle StaticGravity (bit_gravity and win_gravity!) in WM_SIZE

MRESULT EXPENTRY xpmwndproc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
	EbGetHookAccess();

//	BOOL weakEvent = FALSE;
	MRESULT result = (MRESULT)0;
	Window windowres = getWindow(hWnd, FALSE, NULL);
	EB_Window *ebw = getResource(EBWINDOW, windowres);

	fprintf(logfile, "Processing xpmwnd event: %x\n", (int)msg);
	fflush(logfile);

//	Xlib_PMWM_Handler0(&hWnd, &msg, &mp1, &mp2);
	switch(msg) {

	case WM_CALCVALIDRECTS: {
		RECTL *oldRect = mp1;
		RECTL *newRect = oldRect + 1;

		*oldRect = *newRect;
		result = 0;
		break;
	}
	case WM_SIZE: {
		POINTL delta, stddelta;
		HENUM children;
		HWND borderwin, childborderwin;
		SWP childborder, swp;

		borderwin = WinQueryWindow(hWnd, QW_PARENT);
		WinQueryWindowPos(hWnd, &swp);

		stddelta.x = CalcXDelta(SHORT1FROMMP(mp1), SHORT1FROMMP(mp2), ebw->bit_gravity);
		stddelta.y = CalcYDelta(SHORT2FROMMP(mp1), SHORT2FROMMP(mp2), ebw->bit_gravity);

		children = WinBeginEnumWindows(hWnd);
		while((childborderwin = WinGetNextWindow(children))) {
			HWND child = WinQueryWindow(childborderwin, QW_TOP);
			Window childres = getWindow(child, FALSE, NULL);
			EB_Window *ebwchild = getResource(EBWINDOW, childres);

			if(!ebwchild || ebwchild->win_gravity == StaticGravity)
				continue;

			int mydeltax = CalcXDelta(SHORT1FROMMP(mp1), SHORT1FROMMP(mp2), ebwchild->win_gravity) - stddelta.x;
			int mydeltay = CalcYDelta(SHORT2FROMMP(mp1), SHORT2FROMMP(mp2), ebwchild->win_gravity) - stddelta.y;

			if(mydeltax || mydeltay) {
				WinQueryWindowPos(childborderwin, &childborder);
				WinSetWindowPos(childborderwin, 0, childborder.x + mydeltax, childborder.y + mydeltay, 0, 0, SWP_MOVE | SWP_NOREDRAW);
			}

			if(ebwchild->win_gravity == UnmapGravity)
				WinShowWindow(child, FALSE);
		}
		WinEndEnumWindows(children);
		break;
	}

#if 0
	case WM_DESTROY:
	{
		Xlib_GC *xgc;

		if (!attrib)
			break;

//		x11_console_notice("DestroyNotify");
		new = Daemon_createEvent(&newq, hWnd, DestroyNotify);
		new->xdestroywindow.window = hWnd;
		Daemon_doEvent(newq, StructureNotifyMask);
		new2 = Daemon_createEvent(&newq2, WinQueryWindowULong(hWnd, QWP_PARENT), DestroyNotify);
		new2->xdestroywindow.window = hWnd;
		Daemon_doEvent(newq2, SubstructureNotifyMask);

		xgc = (Xlib_GC *)attrib->currentGC;
		Daemon_purgegc(attrib->currentGC);
		if (xgc && xgc->winattrib == attrib) {
				xgc->winattrib = NULL;
				(HPS)xgc->gid = WinGetScreenPS(HWND_DESKTOP);
		}
		attrib->currentGC = (GC)0L;
		Xlib_FreeAllAtoms((Xlib_Pixmap *)attrib);
		Xlib_UnmonitorResource((XID *)&attrib->background_pixmap);
		sfree(attrib);
		break;
	}
	case WM_CLOSE:
		if (attrib && attrib->delete_window_notify) {
			/*x11_console_notice("WM_DELETE_WINDOW");*/
			new = Daemon_createEvent(&newq, hWnd, ClientMessage);
			new->xclient.message_type = Daemon_xinternatom("WM_PROTOCOLS", False);
			new->xclient.format = 32;
			new->xclient.data.l[0] = Daemon_xinternatom("WM_DELETE_WINDOW",False);
			new->xclient.data.l[1] = 0;
			new->xclient.send_event = True;
//			if (attrib->wm_client_leader)
//				new->xany.window = attrib->wm_client_leader;
			Daemon_doEvent(newq, NoEventMask);
		} else
			WinDestroyWindow(WinQueryWindow(hWnd, QW_PARENT));
		break;
	case WM_ACTIVATE:
		break;
	case WM_MOVE:
	{
		HWND hwndparent = WinQueryWindowULong(hWnd, QWP_PARENT);
		HWND hwndborder = WinQueryWindow(hWnd, QW_PARENT);
		SWP swp, border, parent;

		if (!winattrib || !hwndparent)
			break;
		WinQueryWindowPos(hWnd, &swp);
		WinQueryWindowPos(hwndborder, &border);
		WinQueryWindowPos(hwndparent, &parent);
		if (!(swp.fl &(SWP_HIDE|SWP_MINIMIZE)) && attrib->wm_name)
			WinSetWindowText(hwndborder, attrib->wm_name);
		if ((swp.fl &(SWP_HIDE|SWP_MINIMIZE)) && attrib->wm_iconname)
			WinSetWindowText(hwndborder, attrib->wm_iconname);

		new = Daemon_createEvent(&newq, hWnd, ConfigureNotify);
		new->xconfigure.window = hWnd;
		new->xconfigure.x = winattrib->x = swp.x + border.x;
		new->xconfigure.y = winattrib->y = parent.cy - swp.y - border.y - swp.cy;
		new->xconfigure.width = winattrib->width;
		new->xconfigure.height = winattrib->height;
		new->xconfigure.border_width = winattrib->border_width;
//		if (attrib->wm_client_leader)
//			new->xany.window = attrib->wm_client_leader;
		new->xconfigure.override_redirect = winattrib->override_redirect;
		new2 = Daemon_copyEvent(&newq2, newq);

// must be included into event processing
/*		Xlib_mutex_lock(evmutex);
		if (attrib->lastconfigure)
		{
			memcpy(attrib->lastconfigure, new, sizeof(XEvent));
			Xlib_mutex_unlock(evmutex);
			sfree(newq);
		}
		else
		{
			attrib->lastconfigure = new;
			Xlib_mutex_unlock(evmutex);
*/			Daemon_doEvent(newq, StructureNotifyMask);
//		}
		new2->xconfigure.event = hwndparent;
		Daemon_doEvent(newq2, SubstructureNotifyMask);
		break;
	}
	case UM_UNMAP:
		new = Daemon_createEvent(&newq, hWnd, UnmapNotify);
		new->xunmap.window = hWnd;
		new->xunmap.from_configure = mp2?TRUE:FALSE;
		Daemon_doEvent(newq, StructureNotifyMask);
		new2 = Daemon_createEvent(&newq2, WinQueryWindowULong(hWnd, QWP_PARENT), UnmapNotify);
		new2->xunmap.window = hWnd;
		new2->xunmap.from_configure = mp2?TRUE:FALSE;
		Daemon_doEvent(newq2, SubstructureNotifyMask);
		break;
	case WM_SHOW:
	{
		int type = SHORT1FROMMP(mp1)?MapNotify:UnmapNotify;

		new = Daemon_createEvent(&newq, hWnd, type);
		new->xunmap.window = HWNDFROMMP(mp1);
		new->xunmap.from_configure = FALSE;
		Daemon_doEvent(newq, StructureNotifyMask);
		new2 = Daemon_createEvent(&newq2, WinQueryWindowULong(hWnd, QWP_PARENT), type);
		new2->xunmap.window = HWNDFROMMP(mp1);
		new2->xunmap.from_configure = FALSE;
		Daemon_doEvent(newq2, SubstructureNotifyMask);
		break;
	}
	case WM_CHAR:
	{
		int type, keycode;
		int state;
		BOOL repeat = FALSE;

		if(!winattrib)
			break;

// type detection
		if (SHORT1FROMMP(mp1) & KC_KEYUP) {
			type = KeyRelease;
		} else {
			type = KeyPress;
			if (SHORT1FROMMP(mp1) & KC_PREVDOWN) {
				/* Key repeat! */
				if(autorepeat)
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

		if (keycode < min_keycode || keycode > max_keycode)
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

		new = Daemon_createEvent(&newq, hWnd, type);
		new->xkey.window = hWnd;
		new->xkey.subwindow = None;
		new->xkey.time = WinQueryMsgTime(pmctls_hab);
		_XQueryPointer(hWnd, &new->xkey.root, NULL, &new->xkey.x_root,
			&new->xkey.y_root, &new->xkey.x, &new->xkey.y, NULL, TRUE);
		new->xkey.state = state;
		new->xkey.keycode = keycode;
		new->xkey.same_screen = TRUE;
		if(repeat)
		{
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
#endif
	case UM_CHANGEPOINTER:
	case WM_MOUSEMOVE: {
		EB_Cursor *ebc = getResource(EBCURSOR, (Cursor)ebw->cursor);
		if(ebc)
			WinSetPointer(HWND_DESKTOP, ebc->pointer);
		else // TODO pointer of parent
			WinDefWindowProc(hWnd, msg, mp1, mp2);
		break;
	}
#if 0
	case WM_BUTTON1MOTIONSTART:
	case WM_BUTTON2MOTIONSTART:
	case WM_BUTTON3MOTIONSTART:
	case WM_BUTTON1MOTIONEND:
	case WM_BUTTON2MOTIONEND:
	case WM_BUTTON3MOTIONEND:
	case WM_SEM4:
	{
		POINTL ptls = {SHORT1FROMMP(mp1), SHORT2FROMMP(mp1)};
		int btnmask = PointerMotionMask;

		if (WinGetKeyState(HWND_DESKTOP,VK_BUTTON1) & 0x8000)
			btnmask |= Button1MotionMask;
		if (WinGetKeyState(HWND_DESKTOP,VK_BUTTON2) & 0x8000)
			btnmask |= Button3MotionMask;
		if (WinGetKeyState(HWND_DESKTOP,VK_BUTTON3) & 0x8000)
			btnmask |= Button2MotionMask;
		if(btnmask & (Button1MotionMask | Button2MotionMask | Button3MotionMask))
			btnmask |= ButtonMotionMask;

		new = Daemon_createEvent(&newq, hWnd, MotionNotify);
		new->xmotion.state = Xlib_State(-1);
		new->xmotion.root = HWND_DESKTOP;
		new->xmotion.subwindow = None;
		new->xmotion.time = WinQueryMsgTime(pmctls_hab);
		new->xmotion.x = ptls.x;
		new->xmotion.y = winattrib->height - ptls.y - 1;
		WinMapWindowPoints(hWnd, HWND_DESKTOP, &ptls, 1);
		new->xmotion.x_root = ptls.x;
		new->xmotion.y_root = WinQuerySysValue(HWND_DESKTOP,SV_CYSCREEN) - ptls.y - 1;
		new->xmotion.is_hint = NotifyNormal;
		new->xmotion.same_screen = TRUE;
		Daemon_propagateEvent(newq, btnmask, weakEvent);
		break;
	}
	case WM_BUTTON1DOWN:
	case WM_BUTTON1DBLCLK:
	case WM_BUTTON2DOWN:
	case WM_BUTTON2DBLCLK:
	case WM_BUTTON3DOWN:
	case WM_BUTTON3DBLCLK:
	case WM_BUTTON1UP:
	case WM_BUTTON2UP:
	case WM_BUTTON3UP:
	{
		int type = 0;
		int button = 0;
		POINTL ptls = {SHORT1FROMMP(mp1), SHORT2FROMMP(mp1)};

		if (!winattrib)
			break;

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
		}

		if (type == ButtonPress && !(WinQueryFocus(HWND_DESKTOP) == hWnd))
				WinSetFocus(HWND_DESKTOP, hWnd);

		new = Daemon_createEvent(&newq, hWnd, type);
		new->xbutton.root = HWND_DESKTOP;
		new->xbutton.subwindow = None;
		new->xbutton.time = WinQueryMsgTime(pmctls_hab);
		new->xbutton.x = ptls.x;
		new->xbutton.y = winattrib->height - ptls.y - 1;
		WinMapWindowPoints(hWnd, HWND_DESKTOP, &ptls, 1);
		new->xbutton.x_root = ptls.x;
		new->xbutton.y_root = WinQuerySysValue(HWND_DESKTOP,SV_CYSCREEN) - ptls.y - 1;
		new->xbutton.state = Xlib_State(SHORT1FROMMP(mp1));
		new->xbutton.button = button;
		new->xbutton.same_screen = True;
		Daemon_propagateEvent(newq, (type==ButtonPress)?ButtonPressMask:ButtonReleaseMask, 0);
		break;
	}
	case WM_PAINT: {
		RECTL rectl;
		SWP swp;
		int x, y, width, height;
		HPS hps;

		WinQueryWindowPos(hWnd, &swp);

		hps = WinBeginPaint(hWnd, NULLHANDLE, &rectl);
		x = rectl.xLeft;
		y = swp.cy - rectl.yTop;
		width = rectl.xRight - rectl.xLeft + 1;
		height = rectl.yTop - rectl.yBottom + 1;
		GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0, NULL);
		WinFillRect(hps, &rectl, ebw->background_pixel);
		WinEndPaint(hps);

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
	case WM_POINTERENTER:
	case WM_POINTERLEAVE:
	{
		POINTL ptl;
		HWND other = LONGFROMMP(mp2);
		HWND ancestor = hWnd;
		HWND parent = WinQueryWindowULong(hWnd, QWP_PARENT);

		if(LONGFROMMP(mp1) != hWnd)
			break;
		
		if (!winattrib)
			break;

		while(!WinIsChild(other, ancestor = WinQueryWindowULong(ancestor, QWP_PARENT)) && ancestor != HWND_DESKTOP);
		WinQueryMsgPos(pmctls_hab, &ptl);

		new = Daemon_createEvent(&newq, hWnd, (msg == 0x041e)?EnterNotify:LeaveNotify);
		new->xcrossing.root = HWND_DESKTOP;
		new->xcrossing.subwindow = None;
		new->xcrossing.time = WinQueryMsgTime(pmctls_hab);
		new->xcrossing.x_root = ptl.x;
		new->xcrossing.y_root = WinQuerySysValue(HWND_DESKTOP,SV_CYSCREEN) - ptl.y - 1;
		WinMapWindowPoints(HWND_DESKTOP, hWnd, &ptl, 1);
		new->xcrossing.x = ptl.x;
		new->xcrossing.y = winattrib->height - ptl.y - 1;
		new->xcrossing.mode = NotifyNormal;
		new->xcrossing.detail = NotifyNonlinear;
		new->xcrossing.same_screen = 1;
		new->xcrossing.focus = WinIsChild(hWnd,WinQueryFocus(HWND_DESKTOP));
		new->xcrossing.state = Xlib_State(-1);
		if(parent != ancestor);
		{
			new2 = Daemon_copyEvent(&newq2, newq);
			new2->xany.window = parent;
			new2->xfocus.detail = NotifyNonlinearVirtual;
			Daemon_recurseEvent(newq2, new2,
				(msg==WM_POINTERENTER)?EnterWindowMask:LeaveWindowMask, ancestor);
		}
		Daemon_doEvent(newq, (msg==WM_POINTERENTER)?EnterWindowMask:LeaveWindowMask);

		break;
	}
	case UM_ChangeProperty:
	{
		HWND hwndborder = WinQueryWindowULong(hWnd, QWP_PARENT);
		Atom atom = (Atom)mp1;

		switch (atom) {
		case XA_WM_NAME:
		{
			SWP swp;

			WinQueryWindowPos(hwndborder, &swp);
			if (!(swp.fl &(SWP_HIDE|SWP_MINIMIZE)))
				WinSetWindowText(hwndborder, attrib->wm_name);
			break;
		}
		case XA_WM_ICON_NAME:
		{
			SWP swp;

			WinQueryWindowPos(hwndborder,&swp);
			if ((swp.fl &(SWP_HIDE|SWP_MINIMIZE)))
				WinSetWindowText(hwndborder, attrib->wm_iconname);
			}
		}
//		if (attrib->wm_client_leader)
//			new->xany.window = attrib->wm_client_leader;

		new = Daemon_createEvent(&newq, hWnd, PropertyNotify);
		new->xproperty.atom = atom;
		new->xproperty.time = WinGetCurrentTime(pmctls_hab);
		new->xproperty.state = (int)mp2;
		Daemon_doEvent(newq, PropertyChangeMask);

		break;
	}
	case UM_Reparent:
	{
// old parent is missing

		SWP swp, pswp, border;

		new = Daemon_createEvent(&newq, hWnd, ReparentNotify);
		new->xreparent.parent = WinQueryWindowULong(hWnd, QWP_PARENT);
		WinQueryWindowPos(hWnd, &swp);
		WinQueryWindowPos(WinQueryWindow(hWnd, QW_PARENT), &border);
		WinQueryWindowPos(new->xreparent.parent, &pswp);
		new->xreparent.window = hWnd;
		new->xreparent.parent = WinQueryWindowULong(new->xany.window, QWP_PARENT);
		new->xreparent.x = swp.x;
		new->xreparent.y = pswp.cy - swp.y - border.y - 1;
		new->xreparent.override_redirect = attrib->winattrib.override_redirect;
//		if (attrib->wm_client_leader)
//			new->xany.window = attrib->wm_client_leader;
		new2 = Daemon_copyEvent(&newq2, newq);
		new3 = Daemon_copyEvent(&newq3, newq);
		Daemon_doEvent(newq, StructureNotifyMask);
		new2->xany.window = new2->xreparent.parent;
		Daemon_doEvent(newq2, SubstructureNotifyMask);
//		new3->xany.window = ;
//		Daemon_doEvent(newq3, SubstructureNotifyMask);

		break;
	}
#endif
	default:
		result = WinDefWindowProc(hWnd, msg, mp1, mp2);
	}
//	Xlib_PMWM_Handler1(&hWnd, &msg, &mp1, &mp2);
	fprintf(logfile, "...finished (%x)\n", (int)WinGetLastError(pmctls_hab));
	fflush(logfile);
	EbReleaseHookAccess();
	return result;
}
