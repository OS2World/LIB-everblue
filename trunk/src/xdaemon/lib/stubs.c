#include "daemondll.h"

int auto_repeat = 0;
int _XmaxKeyCode, _XminKeyCode;

EXPENTRY Window Daemon_RootWindow();

int Xlib_State(int hint) {
	int state = 0;

	if(hint < 0) {
		if (WinGetKeyState(HWND_DESKTOP,VK_SHIFT) & 0x8000) state |= ShiftMask;
		if (WinGetKeyState(HWND_DESKTOP,VK_CTRL) & 0x8000) state |= ControlMask;
		if (WinGetKeyState(HWND_DESKTOP,VK_ALT) & 0x8000) state |= Mod2Mask;
	} else {
		if (hint & KC_SHIFT) state |= ShiftMask;
		if (hint & KC_CTRL) state |= ControlMask;
		if (hint & KC_ALT) state |= Mod2Mask;
	}
	if (WinGetKeyState(HWND_DESKTOP, VK_CAPSLOCK)&0x0001) state |= LockMask;
	if (WinGetKeyState(HWND_DESKTOP, VK_BUTTON1)&0x8000) state |= Button1Mask;
	if (WinGetKeyState(HWND_DESKTOP, VK_BUTTON2)&0x8000) state |= Button2Mask;
	if (WinGetKeyState(HWND_DESKTOP, VK_BUTTON3)&0x8000) state |= Button3Mask;

	return(state);
}
Bool _XQueryPointer(Window w, Window* root_return, Window* child_return, 
		int* root_x_return, int* root_y_return, int* win_x_return, int* win_y_return,
		unsigned int* mask_return, PPOINTL pptl) {
	POINTL ptl;
	Window child;
	XWindowAttributes *attrib;

	if(!pptl) {
		pptl = &ptl;
		WinQueryPointerPos(HWND_DESKTOP, pptl);
	}
	EB_Window *ebw = getResource(EBWINDOW, w);
	HWND childhwnd = WinWindowFromPoint(ebw->hwnd, &ptl, TRUE);
	child = getWindow(childhwnd, FALSE, NULL);
	if(!child)
		child = Daemon_RootWindow();
	if(root_x_return)
		*root_x_return = ptl.x;
	if(root_y_return)
		*root_y_return = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - ptl.y - 1;
	if(root_return)
		*root_return = Daemon_RootWindow();
	if(child_return)
		*child_return = child;
	if(w && (win_x_return || win_y_return)) {
		RECTL rectl;
		WinQueryWindowRect(w, &rectl);
		WinMapWindowPoints(HWND_DESKTOP, ebw->hwnd, &ptl, 1);
		if(win_x_return)
			*win_x_return = ptl.x;
		if(win_y_return)
			*win_y_return = rectl.yTop - ptl.y;
	}
	if(mask_return)
		*mask_return = Xlib_State(-1);
	return TRUE;
}
