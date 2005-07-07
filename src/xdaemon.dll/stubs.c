#include "daemondll.h"

int auto_repeat = 0;

char *Daemon_getatomname(Atom atom) {
	return "";
}
Atom Daemon_xinternatom(_Xconst char* atom_name, Bool only_if_exists) {
	return (Atom)0;
}
void Daemon_purgegc(GC gc) {
}
int Xlib_State(int hint) {
	return 0;
}
Bool _XQueryPointer(Window w, Window* root_return, Window* child_return, 
		int* root_x_return, int* root_y_return, int* win_x_return, int* win_y_return,
		unsigned int* mask_return, BOOL msgpointer) {
	return FALSE;
}
void Xlib_FreeAllAtoms(EB_Pixmap *client) {
}
void Xlib_UnmonitorResource(XID *resource) {
}
void Xlib_set_data(HWND window, Atom dataname, UserData *data) {
}
UserData *Xlib_get_data(HWND window, Atom dataname) {
	return (UserData *)0;
}
int XDeleteProperty(Display* dpy, Window window, Atom atom) {
	return 0;
}
EXPENTRY Status XGetWindowAttributes2(Display* display, Window w, XWindowAttributes* window_attributes_return) {
	return (Status)0;
}
EXPENTRY int XGetWindowProperty(Display* display, Window w, Atom property,
	long long_offset, long long_length, Bool delete, Atom req_type, Atom*
	actual_type_return,	int* actual_format_return, unsigned long* nitems_return,
	unsigned long* bytes_after_return, unsigned char** prop_return) {
	return 0;
}

EXPENTRY XWindowAttributes *Daemon_getwinattrib(Drawable w, HPS *hps) {
	return NULL;
}
