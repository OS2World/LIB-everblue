#include "x11daemon.h"

int Xlib_PMWM_Handler0(HWND* hWnd, ULONG* msg, MPARAM* mp1, MPARAM* mp2)
{
	if(msg && mp1)
		switch(*msg) {
/*		case UM_SetWindowPos:
		{
			SWP oldswp;
			HWND parent;
			Xlib_SetWindowPos *args = PVOIDFROMMP(*mp1);
			char winclass[32];
			if (!args) break;

			winclass[WinQueryClassName(args->hwnd, sizeof(winclass), winclass)] = 0;

			if (strcmp(winclass,"XPMChild") == 0)
				parent = WinQueryWindowULong(args->hwnd, QWP_PARENT);
			else
				parent = WinQueryWindowULong(WinQueryWindow(args->hwnd, QW_TOP), QWP_PARENT);

			WinQueryWindowPos(args->hwnd, &oldswp);

			if (parent == HWND_DESKTOP) {
				int cyscreen = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
				int y, cy;
				if(args->fl & SWP_SHOW)
					args->fl |= SWP_ACTIVATE;
				if (args->fl & SWP_MOVE) y = args->y; else y = oldswp.y;
				if (args->fl & SWP_SIZE) cy = args->cy; else cy = oldswp.cy;
				if (cy > cyscreen) {
					if (!(args->fl & SWP_MOVE)) {
						args->x = oldswp.x;
						args->fl |= SWP_MOVE;
					}
					args->y = y + cy - cyscreen;
					args->cy = cy = cyscreen;
					if (!(args->fl & SWP_SIZE)) {
						args->cx = oldswp.cx;
						args->fl |= SWP_SIZE;
					}
				}
				if (y + cy > cyscreen) {
					args->y = cyscreen - cy;
					if (!(args->fl & SWP_MOVE)) {
						args->x = oldswp.x;
						args->fl |= SWP_MOVE;
					}
				}
				if (y < 0) {
					args->y = 0;
					if (!(args->fl & SWP_MOVE)) {
						args->x = oldswp.x;
						args->fl |= SWP_MOVE;
					}
				}
			}

			if (args->fl & SWP_MOVE && args->x == oldswp.x && args->y == oldswp.y)
				args->fl &= ~SWP_MOVE;
			if (args->fl & SWP_SIZE && args->cx == oldswp.cx && args->cy == oldswp.cy)
				args->fl &= ~SWP_SIZE;

			break;
		}*/
	}
	return 0;
}

int Xlib_PMWM_Handler1(HWND* hWnd, ULONG* msg, MPARAM* mp1, MPARAM* mp2)
{
#if 0
    if(msg && mp1)
	switch(*msg) {
    case UM_CHANGEPROPERTY:
        {
            UM_ChangeProperty *args = PVOIDFROMMP(mp1);
            WinAttribData *attrib = WinQueryWindowPtr(args->w, QWP_WINDOW);

           if (args->property == XA_WM_ICON_NAME && args->type == XA_STRING) {
                HPOINTER tmpicon;
                HWND hwndborder = WinQueryWindow((HWND)args->w, QW_PARENT);

                if(attrib->wm_iconname)
                {
                    char *newname = smalloc(strlen(attrib->wm_iconname)+5);

                    strcpy(newname, attrib->wm_iconname);
                    strcat(newname, ".ico");
                    /* May need to search a path of some sort */
                    if((tmpicon = WinLoadFileIcon(newname, FALSE)))
                        WinSendMsg(hwndborder, WM_SETICON, (MPARAM)tmpicon, 0);
                    sfree(newname);
                }
            }
        }
        break;
    }
#endif
    return 0;
}
