#include "x11daemon.h"

VOID APIENTRY Xlib_ExitHandler(ULONG ulTerm) {
	Daemon_shutdown(pmctls_hab);
	DosExitList(EXLST_EXIT, (PFNEXITLIST)NULL);
}


int main(void)
{
	QMSG qmsg;

	logfile = fopen("xdaemon.log", "w");
	fprintf(logfile, "--- xdaemon.exe log ---\n");
	fflush(logfile);

	DosSetPriority(PRTYS_PROCESS, PRTYC_TIMECRITICAL, 0, 0);
	if(!Daemon_register()) {
		fprintf(logfile, "Error registering with xdaemon.dll\n");
		exit(1);
	}
	um_openHandles(NULL);
	DosExitList(EXLST_ADD, Xlib_ExitHandler);
	initializePM();
	EbReleaseHookDataAccess();
	Daemon_xinitialized(pmctls_hab, mainhwnd);

	while(WinGetMsg(pmctls_hab, &qmsg, 0, 0, 0)) {
		if(qmsg.msg != WM_TIMER) {
			fprintf(logfile, "posted message %x\n", (int)qmsg.msg);
			fflush(logfile);
		}
		WinDispatchMsg(pmctls_hab, &qmsg);
	}
	Daemon_shutdown(pmctls_hab);
	DosExitList(EXLST_REMOVE, (PFNEXITLIST)Xlib_ExitHandler);
	shutDownPM();
	closeHandles(nextpipe, NULLHANDLE);
	fclose(logfile);
//	EbLogTerminate();
	return 0;
}

