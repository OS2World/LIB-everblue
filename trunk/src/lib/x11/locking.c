#include "X11.h"

Status XInitThreads() {
    return 1;
}

void XLockDisplay(register Display* dpy){
	LockDisplay(dpy);
}

void XUnlockDisplay(register Display* dpy) {
	UnlockDisplay(dpy);
}
