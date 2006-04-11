#include "daemondll.h"

Window root_window = 0;

EXPENTRY Window Daemon_RootWindow() {
	if(!root_window) {
//		XWindowAttributes *desktopattrib;
		EB_Window *root;

		root_window = getWindow(HWND_DESKTOP, TRUE, NULL);
		root = getResource(EBWINDOW, (Window)root_window);

/*		desktopattrib->x = 0;
		desktopattrib->y = 0;
		desktopattrib->width = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
		desktopattrib->height = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
		desktopattrib->depth = 32;
		desktopattrib->root = (Window)root_window;
		desktopattrib->colormap = 1;*/

		root->class = InputOutput;
		root->bit_gravity = ForgetGravity;
		root->win_gravity = NorthWestGravity;
		root->backing_store = NotUseful;
		root->backing_planes = ~0;
		root->backing_pixel = 0;
		root->save_under = False;
		root->do_not_propagate_mask = ~0;
		root->override_redirect = True;
        root->background_pixmap = None;
        root->background_pixel = WinQuerySysColor(HWND_DESKTOP, SYSCLR_WINDOW, 0);
		// TODO below this line - mainly properties for window manager
        root->cursor = 0;
        root->delete_window_notify = False;
        root->wm_name = "WM_NAME - Desktop";
		root->wm_iconname = "WM_ICONNAME - Desktop";
		root->wm_class = "WM_CLASS - Desktop";
		root->wm_locale = "WM_LOCALE - Desktop";
        root->sizehints = 0;
        root->hints = 0;
        root->wm_client_leader = 0;
	}

	return root_window;
}

/*EXPENTRY EB_Window *Daemon_getWindowFromHWnd(HWND hwnd) {
	HAB hab;

	if(hwnd == HWND_DESKTOP)
		return root_window;
	if(!hwnd)
		return 0;
	Daemon_getPMHandle(&hab);
	if(WinIsWindow(hab, hwnd)) {
		char winclass[32];

		winclass[WinQueryClassName(hwnd, sizeof(winclass), winclass)] = 0;
		if(strcmp(winclass,"XPMChild") == 0)
			return WinQueryWindowPtr(hwnd, QWP_WINDOW);
	}
	return 0;
}*/
