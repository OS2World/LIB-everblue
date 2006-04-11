#include "x11daemon.h"
#include "Xatomtype.h"

MRESULT EXPENTRY pmhwndproc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
	MRESULT result;

	if(msg == UM_HOOKMSG) {
		getReadAccess(&xeventsem);
		hook_msg(msgxchange.hwnd, msgxchange.msg, msgxchange.mp1, msgxchange.mp2, msgxchange.time, msgxchange.ptl);
		return 0;
	}

	mutex_lock(global_lock, FALSE);
	if(msg != WM_TIMER) {
		fprintf(logfile, "Processing pmhwnd event: %x\n", (int)msg);
		fflush(logfile);
	}
	result = FALSE;
	Xlib_PMWM_Handler0(&hWnd, &msg, &mp1, &mp2);
	switch(msg) {
	case WM_TIMER:
		Daemon_doTasks(um_freeResources);
		break;
	case UM_LOADPOINTER: {
		HMODULE module;
		DosQueryModuleHandle("xdaemon", &module);
		result = (MRESULT)WinLoadPointer(HWND_DESKTOP, module, (int)mp1 + 0x1000);
		break;
	}
	case UM_OPENHANDLES:
		um_openHandles(PVOIDFROMMP(mp1));
		break;
	case UM_CREATEWINDOW:
		result = (MRESULT)um_createWindow(PVOIDFROMMP(mp1));
		sfree(PVOIDFROMMP(mp1));
		break;
	case UM_MAPWINDOW:
		result = (MRESULT)um_mapWindow((Window)mp1);
		break;
	case UM_MAPSUBWINDOWS:
		result = (MRESULT)um_mapSubwindows((Window)mp1);
		break;
	case UM_UNMAPWINDOW:
		result = (MRESULT)um_unmapWindow((Window)mp1);
		break;
	case UM_LOWERWINDOW:
		result = (MRESULT)um_lowerWindow((Window)mp1);
		break;
	case UM_RAISEWINDOW:
		result = (MRESULT)um_raiseWindow((Window)mp1);
		break;
	case UM_MAPRAISED:
		result = (MRESULT)um_mapRaised((Window)mp1);
		break;
	case UM_DESTROYWINDOW:
		result = (MRESULT)um_destroyWindow(PVOIDFROMMP(mp1));
		break;
	case UM_SETWINDOWPOS:
		result = (MRESULT)um_setWindowPos(PVOIDFROMMP(mp1));
		sfree(PVOIDFROMMP(mp1));
		break;
	case UM_FREERESOURCES:
		um_freeResources(PVOIDFROMMP(mp1));
		break;
	case UM_DELTWINDOWPOS:
		{
			SWP swp;

			WinQueryWindowPos(HWNDFROMMP(mp1), &swp);
			result = (MRESULT)WinSetWindowPos(HWNDFROMMP(mp1), 0,
								 swp.x + SHORT1FROMMP(mp2) - 32768, swp.y + SHORT2FROMMP(mp2) - 32768, 0, 0, SWP_MOVE | SWP_NOREDRAW);
		}
		break;
	case UM_REPARENTWINDOW:
		{
			UM_ReparentWindow *args = PVOIDFROMMP(mp1);
			if(WinSetParent((HWND)args->w, (HWND)args->p, FALSE)) {
				SWP swp;
				WinQueryWindowPos((HWND)args->p, &swp);
				WinSetWindowPos((HWND)args->w, 0, args->x, swp.cy - args->y - 1, 0, 0, SWP_MOVE);
				result = (MRESULT)1;
			}
			break;
		}
	case UM_CREATEEVENT: {
			UM_CreateEvent *EventParams = (UM_CreateEvent *)PVOIDFROMMP(mp1);
			Daemon_addEvent(EventParams->event, EventParams->procres, False);
			sfree(PVOIDFROMMP(mp1));
			break;
		}
#if 0
	case UM_CHANGEPROPERTY:
		{
			UM_ChangeProperty *args = PVOIDFROMMP(mp1);
			XWindowAttributes *winattrib = Daemon_getwinattrib(args->w, NULL);
			WinAttribData *attrib = (winattrib)?WinQueryWindowPtr(args->w, QWP_WINDOW):NULL;
			char *st1, *st2;
			int state=PropertyNewValue, i;
			Atom tmpatm;

			if (!mp1 || !args->property || !attrib) return (MRESULT)BadImplementation;

//printf("Changing Property %x\n", args->property);
			if (args->property == XA_WM_NAME && args->type == XA_STRING) {
				if (attrib) {
					if (attrib->wm_name) sfree(attrib->wm_name);
					attrib->wm_name = smalloc(args->nelements+1);
					strncpy(attrib->wm_name, args->data, args->nelements);
					attrib->wm_name[args->nelements] = '\0';
				}
			} else if (args->property == XA_WM_ICON_NAME && args->type == XA_STRING) {
				if (attrib && args->nelements) {
					if (attrib->wm_iconname) sfree(attrib->wm_iconname);
					attrib->wm_iconname = smalloc(args->nelements+1);
					strncpy(attrib->wm_iconname, args->data, args->nelements);
					attrib->wm_iconname[args->nelements] = '\0';
				}
			} else if (args->property == XA_WM_COMMAND && args->type == XA_STRING) {
				if (attrib) {
					st1 = smalloc(args->nelements+1);
					strncpy(st1, args->data, args->nelements);
					st1[args->nelements] = '\0';
					WinSetWindowText(mainhwnd,st1);
					sfree(st1);
				}
			} else if (args->property == XA_WM_CLASS && args->type == XA_STRING) {
				if (attrib && args->nelements) {
					if (attrib->wm_class) sfree(attrib->wm_class);
					attrib->wm_class = smalloc(args->nelements+1);
					strncpy(attrib->wm_class, args->data, args->nelements);
					attrib->wm_class[args->nelements] = '\0';
				}
			} else if (args->property == XA_WM_NORMAL_HINTS && args->type == XA_WM_SIZE_HINTS) {
				if (attrib && args->nelements) {
					if (attrib->sizehints) sfree(attrib->sizehints);
					attrib->sizehints = scalloc(1, sizeof(XSizeHints));
					memcpy(attrib->sizehints, args->data,
						   (args->format * args->nelements) / 8);
				}
			} else if (args->property == XA_WM_HINTS && args->type == XA_WM_HINTS) {
				if (attrib && args->nelements) {
					if (attrib->hints) sfree(attrib->hints);
					attrib->hints = scalloc(1, sizeof(XWMHints) );
					memcpy(attrib->hints, args->data,
						   (args->format * args->nelements) / 8);
				}
			} else if (args->property == XA_WM_CLIENT_MACHINE
				|| ((tmpatm = Daemon_xinternatom("__SWM_VROOT", True)) && tmpatm == args->property)
				|| ((tmpatm = Daemon_xinternatom("WM_LOCALE_NAME", True)) && tmpatm == args->property)
			   ){ /* not useful in Everblue */ }
			else if ((tmpatm = Daemon_xinternatom("WM_CLIENT_LEADER", True)) &&
				args->property == tmpatm && args->type == XA_WINDOW) {
				if (attrib && args->nelements) {
					attrib->wm_client_leader = ((Window *)args->data)[0];
				}
			} else if ((tmpatm = Daemon_xinternatom("WM_TRANSIENT_FOR", True)) &&
				args->property == tmpatm && args->type == XA_WINDOW) {
				if (attrib && args->nelements) {
					/* This appears to be broken - Brian */
					/*  XWindowAttributes *mattrib = Daemon_getwinattrib(((Window *)args->data)[0], NULL);
					HWND hwndframe = mattrib?
					WinQueryWindowULong(((Window *)args->data)[0],
					QWP_FRAMEHWND):((Window *)args->data)[0];
					WinSetOwner(args->w, hwndframe);
					if(mattrib)
					Xfree(mattrib);*/
				}
			} else if ((tmpatm = Daemon_xinternatom("WM_PROTOCOLS", True)) &&
				args->property == tmpatm && args->type == XA_ATOM)
				for (i=0; i<args->nelements; i++)
				{
					Atom atom = ((Atom*)(args->data))[i];
					char *protocol = Daemon_getatomname(atom);
					if (!protocol) continue;
					if (!strcmp(protocol,"WM_DELETE_WINDOW") && attrib)
						attrib->delete_window_notify = TRUE; else
							fprintf(stderr, "Unhandled WM_PROTOCOL method '%s'\n",protocol);
					sfree(protocol);
				} else {
					UserData *old;

					fprintf(stderr,"UM_ChangeProperty newly implemented!\n	property='%s'(%ld), type='%s'(%ld), format=%d, mode=%d, ",
						st1=Daemon_getatomname(args->property), args->property,
						st2=Daemon_getatomname(args->type), args->type, args->format, args->mode);
					sfree(st1); sfree(st2);
					if (!args->nelements)
						fprintf(stderr,"no args\n");
					else {
						fprintf(stderr,"{ ");
						if (args->type == XA_ATOM)
							for (i=0; i<args->nelements; i++) {
								fprintf(stderr,"'%s' ",
									st1=Daemon_getatomname(((Atom *)(args->data))[i]));
								sfree(st1);
							} else
								if (args->type == XA_STRING) {
									st1 = smalloc(args->nelements+1);
									strncpy(st1, args->data, args->nelements);
									st1[args->nelements] = '\0';
									fprintf(stderr,"'%s' ",st1);
									sfree(st1);
								} else
									fprintf(stderr,"%d elements ",args->nelements);
						fprintf(stderr,"}\n");
					}
					old = Xlib_get_data(args->w, args->property);
					if(old && args->mode != PropModeReplace &&
						(args->format != old->format || args->type != old->type))
					{
						result = (MRESULT)BadMatch;
						break;
					}
					if(old)
					{
						char *newdata;
						int newsize;

						if(args->mode == PropModeReplace)
						{
							newsize = args->nelements * args->format / 8;
							newdata = smalloc(newsize);
							memcpy(newdata, args->data, newsize);
						}
						else
						{
							int offset = 0;
							int offset2 = 0;
							int size = args->nelements * args->format / 8;

							newsize = size + old->size;
							if(args->mode == PropModePrepend)
								offset = old->size;
							else
								offset2 = size;
							newdata = smalloc(newsize);
							memcpy(newdata + offset, args->data, size);
							memcpy(newdata + offset2, old->data, old->size);
							old->format = args->format;
							old->type = args->type;
						}
						old->data = newdata;
						old->size = newsize;
					}
					else
					{
						char *data;
						int size = args->nelements * args->format / 8;
						UserData *entry = smalloc(sizeof(UserData));

						data = smalloc(size);
						memcpy(data, args->data, size);
						entry->property = args->property;
						entry->type = args->type;
						entry->format = args->format;
						entry->data = data;
						entry->size = size;
						Xlib_set_data(args->w, args->property, entry);
					}
				}
			WinPostMsg((HWND)args->w,UM_CHANGEPROPERTY,(MPARAM)args->property,(MPARAM)state);
			break;
		}
	case UM_GETWINPROPERTY:
		{
			UM_GetWinProperty *args = PVOIDFROMMP(mp1);
			XWindowAttributes *winattrib = Daemon_getwinattrib(args->w, NULL);
			WinAttribData *attrib = (winattrib)?WinQueryWindowPtr(args->w, QWP_WINDOW):NULL;
			Atom tmpatm;

			*args->prop_return = NULL;
			*args->actual_type_return = None;
			*args->bytes_after_return = 0;
			*args->actual_format_return = 0;
			*args->nitems_return = 0;

			if (((tmpatm = Daemon_xinternatom("__SWM_VROOT", True)) && tmpatm == args->property)) {
				/* not useful in Everblue */
			} else if((tmpatm = Daemon_xinternatom("WM_NORMAL_HINTS", True)) && tmpatm == args->property) {
				if(attrib->hints /*&& args->req_type == (XA_WM_HINTS || AnyPropertyType)*/)
				{
					*args->actual_type_return = XA_WM_SIZE_HINTS;
					*args->nitems_return = OldNumPropSizeElements;
					*args->actual_format_return = 32;
					*args->prop_return = (unsigned char *)smalloc(sizeof(XSizeHints));
					memcpy(*args->prop_return, attrib->sizehints, sizeof(XSizeHints));
				}
			} else if((tmpatm = Daemon_xinternatom("WM_HINTS", True)) && tmpatm == args->property) {
				if(attrib->hints /*&& args->req_type == (XA_WM_HINTS || AnyPropertyType)*/)
				{
					*args->actual_type_return = XA_WM_HINTS;
					*args->nitems_return = NumPropWMHintsElements;
					*args->actual_format_return = 32;
					*args->prop_return = (unsigned char *)smalloc(sizeof(XWMHints));
					memcpy(*args->prop_return, attrib->hints, sizeof(XWMHints));
				}
			} else if((tmpatm = Daemon_xinternatom("RESOURCE_MANAGER", True)) && tmpatm == args->property) {
				/*if(maindisplay->xdefaults)
				{
					*args->actual_type_return = XA_RESOURCE_MANAGER;
					*args->nitems_return = 1;
					*args->actual_format_return = 32;
					*args->prop_return = (unsigned char *)strdup(maindisplay->xdefaults);
				}*/
			} else {
				char *st1, *st2;
				UserData *data;
printf("hier!");
				data = Xlib_get_data(args->w, args->property);
				fprintf(stderr,"UM_GetWinProperty newly implemented! (found: %x)\n   property='%s', type='%s', \n",
						(int)data,
						st1=Daemon_getatomname(args->property),
						st2=Daemon_getatomname(args->req_type));
				sfree(st1); sfree(st2);
				if(data)
				{
					if(args->req_type != AnyPropertyType && args->req_type != data->type)
					{
						*args->actual_type_return = data->type;
						*args->actual_format_return = data->format;
						*args->bytes_after_return = data->size;
					}
					else
					{
						int N, I, T, L, A;

						*args->actual_type_return = data->type;
						*args->actual_format_return = data->format;
						N = data->size;
						I = 4 * args->long_offset;
						T = N - I;
						L = (T < 4 * args->long_length?T:4 * args->long_length);
						A = N - (I + L);
						if(L < 0)
							result = (MRESULT)BadValue;
						else
						{
							char *datareturn = smalloc(L+1);
							datareturn[L] = 0;
							*args->prop_return = datareturn;
							*args->nitems_return = L * 8 / data->format;
							*args->bytes_after_return = A;
							if(args->delete && A==0)
								XDeleteProperty(args->display, args->w, data->property);
						}
					}
				}
			}
			break;
		}
#endif
	case UM_CREATEGC:
		fprintf(stderr,"UM_CreateGC obseleted!\n");
		break;
	default:
		result = WinDefWindowProc(hWnd, msg, mp1, mp2);
	}
	Xlib_PMWM_Handler1(&hWnd, &msg, &mp1, &mp2);
	if(msg != WM_TIMER) {
		fprintf(logfile, "...finished (%x)\n", (int)WinGetLastError(pmctls_hab));
		fflush(logfile);
	}
	if(msg >= UM_FROM_X11_DLL) {
		EB_Process *process = getResource(EBPROCESS, (XID)mp2);
		process->postret = result;
		DosPostEventSem(process->postsem);
	}
	mutex_unlock(global_lock);
	return result;
}
