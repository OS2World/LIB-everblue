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
	um_openPipe(NULL);
	DosExitList(EXLST_ADD, Xlib_ExitHandler);
	_beginthread(xevent_thread, NULL, 65000, NULL);
	initializePM();
	DosWaitEventSem(xeventsem.write, SEM_INDEFINITE_WAIT);
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
	closePipe(nextpipe);
	fclose(logfile);
	return 0;
}
