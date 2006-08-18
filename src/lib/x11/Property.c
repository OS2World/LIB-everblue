#define XMD_H 1
#include "X11.h"
#include "Xatomtype.h"
#define FID_FRAME 0x0001

/* Functions for managing the user data lists that are associated with
 * a given window handle.  Used in dw_window_set_data() and
 * dw_window_get_data().
 */
UserData *find_userdata(UserData **root, Atom varname) {
	UserData *tmp = *root;

	while(tmp) {
		if(tmp->property == varname) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

int new_userdata(UserData **root, Atom varname, UserData *data) {
	UserData *new = find_userdata(root, varname);

	if(new) {
		memcpy(&new->property, &data->property, sizeof(UserData) - sizeof(data->next));
		Xfree(data);
		return TRUE;
	}
	else {
		data->next = NULL;

		if (!*root)
			*root = data;
		else {
			UserData *prev = NULL, *tmp = *root;
			while(tmp) {
				prev = tmp;
				tmp = tmp->next;
			}
			if(prev)
				prev->next = data;
		}
		return TRUE;
	}
	return FALSE;
}

int remove_userdata(UserData **root, Atom varname, int all) {
	UserData *prev = NULL, *tmp = *root;

	while(tmp) {
		if(all || tmp->property == varname) {
			if(!prev) {
				*root = tmp->next;
				Xfree(tmp);
				return 0;
			}
			else {
				prev->next = tmp->next;
				Xfree(tmp);
				return 0;
			}
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return 0;
}

/*
 * Add a named user data item to a window handle.
 * Parameters:
 *       window: Window handle of signal to be called back.
 *       dataname: A string pointer identifying which signal to be hooked.
 *       data: User data to be passed to the handler function.
 */
void Xlib_set_data(EB_Window *ebw, Atom dataname, UserData *data) {
	if(data)
		new_userdata(&ebw->userdata, dataname, data);
	else {
		if(dataname)
			remove_userdata(&ebw->userdata, dataname, FALSE);
		else
			remove_userdata(&ebw->userdata, 0, TRUE);
	}
}

/*
 * Gets a named user data item to a window handle.
 * Parameters:
 *       window: Window handle of signal to be called back.
 *       dataname: A string pointer identifying which signal to be hooked.
 *       data: User data to be passed to the handler function.
 */
UserData *Xlib_get_data(EB_Window *ebw, Atom dataname) {
	if(ebw->userdata && dataname)
		return find_userdata(&ebw->userdata, dataname);
	return NULL;
}

int XGetWindowProperty(Display* display, Window w, Atom property, long long_offset, 
	long long_length, Bool delete, Atom req_type, Atom* actual_type_return,
	int* actual_format_return, unsigned long* nitems_return,
	unsigned long* bytes_after_return, unsigned char** prop_return) {

	DBUG_ENTER("XGetWindowProperty")
	EB_Window *ebw = getResource(EBWINDOW, w);
	Atom tmpatm;

	*prop_return = NULL;
	*actual_type_return = None;
	*bytes_after_return = 0;
	*actual_format_return = 0;
	*nitems_return = 0;

	if(((tmpatm = XInternAtom(display, "__SWM_VROOT", True)) && tmpatm == property)) {
		if(ebw->hwnd != HWND_DESKTOP && !ebw->xpm && WinQueryWindowUShort(ebw->hwnd, QWS_ID) == FID_FRAME) {
			if(!WinWindowFromID(ebw->hwnd, 0x0056))
				DBUG_RETURN(Success);
			Window wps = getWindow(ebw->hwnd, TRUE, NULL);
			*prop_return = Xmalloc(sizeof(Window));
			memcpy(*prop_return, &wps, sizeof(Window));
			*actual_type_return = XA_WINDOW;
			*actual_format_return = sizeof(Window) * 8;
			*nitems_return = 1;
		}
	} else if((tmpatm = XInternAtom(display, "WM_NORMAL_HINTS", True)) && tmpatm == property) {
		if(ebw->hints /*&& req_type == (XA_WM_HINTS || AnyPropertyType)*/) {
			*actual_type_return = XA_WM_SIZE_HINTS;
			*nitems_return = OldNumPropSizeElements;
			*actual_format_return = 32;
			*prop_return = (unsigned char *)Xmalloc(sizeof(XSizeHints));
			memcpy(*prop_return, ebw->sizehints, sizeof(XSizeHints));
		}
	} else if((tmpatm = XInternAtom(display, "WM_HINTS", True)) && tmpatm == property) {
		if(ebw->hints /*&& req_type == (XA_WM_HINTS || AnyPropertyType)*/) {
			*actual_type_return = XA_WM_HINTS;
			*nitems_return = NumPropWMHintsElements;
			*actual_format_return = 32;
			*prop_return = (unsigned char *)Xmalloc(sizeof(XWMHints));
			memcpy(*prop_return, ebw->hints, sizeof(XWMHints));
		}
	} else if((tmpatm = XInternAtom(display, "RESOURCE_MANAGER", True)) && tmpatm == property) {
		if(display->xdefaults) {
			*actual_type_return = XA_RESOURCE_MANAGER;
			*nitems_return = 1;
			*actual_format_return = 32;
			*prop_return = (unsigned char *)strdup(display->xdefaults);
		}
	} else {
		UserData *data;

		data = Xlib_get_data(ebw, property);
		if(data) {
			if(req_type != AnyPropertyType && req_type != data->type) {
				*actual_type_return = data->type;
				*actual_format_return = data->format;
				*bytes_after_return = data->size;
			} else {
				int N, I, T, L, A;

				*actual_type_return = data->type;
				*actual_format_return = data->format;
				N = data->size;
				I = 4 * long_offset;
				T = N - I;
				L = (T < 4 * long_length?T:4 * long_length);
				A = N - (I + L);
				if(L < 0) {
					DBUG_RETURN(BadValue);
				} else {
					char *datareturn = Xmalloc(L+1);
					datareturn[L] = 0;
					*prop_return = datareturn;
					*nitems_return = L * 8 / data->format;
					*bytes_after_return = A;
					if(delete && A==0)
						XDeleteProperty(display, w, data->property);
				}
			}
		}
	}
	DBUG_RETURN(Success);
}
