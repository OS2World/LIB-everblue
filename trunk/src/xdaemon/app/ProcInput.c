#include "x11daemon.h"

MRESULT EXPENTRY inputwndproc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
	EbGetHookAccess();
	MRESULT result = (MRESULT)0;

//	fprintf(logfile, "Processing inputwnd event: %x\n", (int)msg);
//	fflush(logfile);

	switch(msg) {
	default:
		result = WinDefWindowProc(hWnd, msg, mp1, mp2);
	}
//	fprintf(logfile, "...finished (%x)\n", (int)WinGetLastError(pmctls_hab));
//	fflush(logfile);
	EbReleaseHookAccess();
	return result;
}
