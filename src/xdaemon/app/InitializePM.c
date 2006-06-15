#include "x11daemon.h"

/* Our replacement accellerator table */
#define NACCEL 15
struct {
	ACCELTABLE atable;
	ACCEL accel[NACCEL-1];
} acctable = {
	{ NACCEL, 437,
	{ { AF_SYSCOMMAND | AF_ALT | AF_VIRTUALKEY, VK_F4, SC_CLOSE } } }, {
	{ AF_SYSCOMMAND | AF_ALT | AF_VIRTUALKEY, VK_ENTER, SC_RESTORE },
	{ AF_SYSCOMMAND | AF_ALT | AF_VIRTUALKEY, VK_NEWLINE, SC_RESTORE },
	{ AF_SYSCOMMAND | AF_ALT | AF_VIRTUALKEY, VK_F5, SC_RESTORE },
	{ AF_SYSCOMMAND | AF_ALT | AF_VIRTUALKEY, VK_F6, SC_NEXTFRAME },
	{ AF_SYSCOMMAND | AF_ALT | AF_VIRTUALKEY, VK_F7, SC_MOVE },
	{ AF_SYSCOMMAND | AF_ALT | AF_VIRTUALKEY, VK_F8, SC_SIZE },
	{ AF_SYSCOMMAND | AF_ALT | AF_VIRTUALKEY, VK_F9, SC_MINIMIZE },
	{ AF_SYSCOMMAND | AF_ALT | AF_VIRTUALKEY, VK_F10, SC_MAXIMIZE },
	{ AF_SYSCOMMAND | AF_VIRTUALKEY, VK_F10, SC_APPMENU },
	{ AF_SYSCOMMAND | AF_LONEKEY | AF_VIRTUALKEY, VK_ALT, SC_APPMENU },
	{ AF_SYSCOMMAND | AF_LONEKEY | AF_VIRTUALKEY, VK_ALTGRAF, SC_APPMENU },
	{ AF_SYSCOMMAND | AF_ALT | AF_VIRTUALKEY, VK_SPACE, SC_SYSMENU },
	{ AF_SYSCOMMAND | AF_SHIFT | AF_VIRTUALKEY, VK_ESC, SC_SYSMENU },
	{ AF_SYSCOMMAND | AF_CONTROL | AF_VIRTUALKEY, VK_ESC, SC_TASKMANAGER }
} };

void initializePM() {
	HACCEL holdaccel, haccel;

	pmctls_hab = WinInitialize(0);
	hmq = WinCreateMsgQueue(pmctls_hab, 0);
	WinGetLastError(pmctls_hab); // PMERR_INV_MICROPS_FUNCTION???
	if ((!pmctls_hab) || (!hmq)) {
		fprintf(logfile, "Error init PM instance\n");
		exit(1);
	}

	WinRegisterClass(pmctls_hab, "ObjClass", pmhwndproc, 0, 0);
	WinGetLastError(pmctls_hab); // PMERR_ATOM_NAME_NOT_FOUND???
	WinRegisterClass(pmctls_hab, "XPMChild", xpmwndproc, CS_MOVENOTIFY, 0);
	WinGetLastError(pmctls_hab); // PMERR_ATOM_NAME_NOT_FOUND???
	WinRegisterClass(pmctls_hab, "XPMBorder", brdrwndproc, CS_MOVENOTIFY, 0);
	WinGetLastError(pmctls_hab); // PMERR_ATOM_NAME_NOT_FOUND???
	mainhwnd = WinCreateWindow(HWND_OBJECT, "ObjClass", "Everblue Daemon",
			0, 0, 0, 0, 0, NULLHANDLE, HWND_TOP, FID_OBJECT, NULL, NULL);
	if (mainhwnd == NULLHANDLE) {
		printf("Error creating PM instance\n");
		exit(1);
	}

	holdaccel = WinQueryAccelTable(pmctls_hab, NULLHANDLE);
		// Clear all accellerators
	WinSetAccelTable(pmctls_hab, 0, NULLHANDLE);
		// Delete the old accellerator
	WinDestroyAccelTable(holdaccel);
		// Create the new accellerator
	haccel = WinCreateAccelTable(pmctls_hab, &acctable.atable);
		// Set it as the default for the queue
	WinSetAccelTable(pmctls_hab, haccel, NULLHANDLE);

	// check for work every 100 ms
	WinStartTimer(pmctls_hab, mainhwnd, 1, 100);
}
