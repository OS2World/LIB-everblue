#include "daemondll.h"
#include "common.h"

// phase codes: 0=init value, 1=started, 2=registered, 3=ready, 4=finished

int phase = 0;
int daemonthread = -1;

EXPENTRY BOOL Daemon_register() {
	if (!EbGetHookAccess())
		return FALSE;
	if(daemonthread != -1 || phase > 1) {
		EbReleaseHookAccess();
		return FALSE;
	}
	daemonthread = *_threadid;
	phase = 2;
	EbReleaseHookAccess();
	return TRUE;
}

EXPENTRY BOOL Daemon_xinitialized(HAB hab, HWND mainhwnd) {
	if (!EbGetHookAccess())
		return FALSE;
	if(*_threadid != daemonthread || phase != 2) {
		EbReleaseHookAccess();
		return FALSE;
	}

	realdesktop = WinQueryDesktopWindow(hab, NULLHANDLE);
	objwindow = mainhwnd;
	EbInstallHooks(hab, mainhwnd);
	phase = 3;
	EbReleaseHookAccess();
	return TRUE;
}

EXPENTRY BOOL Daemon_shutdown(HAB hab) {
	EB_Resource *current = processlist;

	if (!EbGetHookAccess())
		return FALSE;
	if(*_threadid != daemonthread || phase != 3) {
		EbReleaseHookAccess();
		return FALSE;
	}

	EbReleaseHooks(hab);
	EbSetHookShutdownState( TRUE);
	EbReleaseHookDataAccess();

	if(current)
		while((current = current->next)) {
			EB_Process *process = getResource(EBPROCESS, (XID)current);
			DosKillProcess(DKP_PROCESS, process->pid);
		}

	phase = 4;
	EbReleaseHookAccess();
	return TRUE;
}

EXPENTRY Bool Daemon_getProcess(EB_Resource **procres) {
	int i;

	if (!EbGetHookAccess())
		return FALSE;
	if(!phase) {
		if(startDaemon())
			phase = 1;
		else {
			EbReleaseHookAccess();
			return False;
		}
	}
	EbReleaseHookAccess();

	for(i = 0; phase < 3 && i < 40; i++)
		DosSleep(25);
	if(phase != 3) {
		fprintf(stderr, "error connecting to daemon!\n");
		return False;
	}

	EbGetHookAccess();
	if(!*procres) {
		EB_Process *process = scalloc(1, sizeof(EB_Process));
		*procres = createResource(EBPROCESS, process);
	}

	EbReleaseHookAccess();
	Daemon_getPMHandle(*procres, NULL);
	if (EbGetHookAccess()) {
		addResource(&processlist, *procres);
		EbReleaseHookAccess();
	}
	return True;
}

BOOL startDaemon() {
	STARTDATA sd = { 0 };
	ULONG pidsession;
	PID pid;
	APIRET ulrc;
static  PSZ pszExename = "xdaemon.exe";
	CHAR szFullExename[ _MAX_PATH];

	ulrc = DosSearchPath( SEARCH_IGNORENETERRS |
		SEARCH_ENVIRONMENT |
		SEARCH_CUR_DIRECTORY,
		"PATH",
		pszExename,
		szFullExename,
		sizeof( szFullExename));
	if (ulrc) {
		fprintf(stderr, "SYS%04ld - cannot find %s (not in PATH)\n",
			ulrc, pszExename);
		return False;
	}
	
	sd.Length = sizeof(STARTDATA);
	sd.FgBg = SSF_FGBG_BACK;
	sd.PgmName = szFullExename;
	sd.PgmControl = SSF_CONTROL_INVISIBLE;
	
	ulrc = DosStartSession(&sd, &pidsession, &pid);
	if(ulrc)
		fprintf(stderr, "SYS%04ld - Error starting %s\n", ulrc, pszExename);

	return (ulrc == 0);
}
