#include "daemondll.h"

// phase codes: 0=init value, 1=started, 2=registered, 3=ready, 4=finished

int phase = 0;
int daemonthread = -1;

EXPENTRY BOOL Daemon_register() {
	mutex_lock(global_lock, FALSE);
	if(daemonthread != -1 || phase > 1) {
		mutex_unlock(global_lock);
		return FALSE;
	}
	daemonthread = *_threadid;
	phase = 2;
	mutex_unlock(global_lock);
	return TRUE;
}

EXPENTRY BOOL Daemon_xinitialized(HAB hab, HWND mainhwnd) {
	mutex_lock(global_lock, FALSE);
	if(*_threadid != daemonthread || phase != 2) {
		mutex_unlock(global_lock);
		return FALSE;
	}

	realdesktop = WinQueryDesktopWindow(hab, NULLHANDLE);
	objwindow = mainhwnd;
	installhooks(hab);
	phase = 3;
	mutex_unlock(global_lock);
	return TRUE;
}

EXPENTRY BOOL Daemon_shutdown(HAB hab) {
	EB_Resource *current = processlist;

	DosRequestMutexSem(global_lock, 100);
	if(*_threadid != daemonthread || phase != 3) {
		DosReleaseMutexSem(global_lock);
		return FALSE;
	}

	releaseReadAccess(&xeventsem);
	releasehooks(hab);

	if(current)
		while((current = current->next)) {
			EB_Process *process = getResource(EBPROCESS, (XID)current);
			DosKillProcess(DKP_PROCESS, process->pid);
		}

	phase = 4;
	DosReleaseMutexSem(global_lock);
	return TRUE;
}

EXPENTRY Bool Daemon_getProcess(EB_Resource **procres) {
	int i;

	mutex_lock(global_lock, FALSE);
	if(!phase) {
		if(startDaemon())
			phase = 1;
		else {
			mutex_unlock(global_lock);
			return False;
		}
	}
	mutex_unlock(global_lock);

	for(i = 0; phase < 3 && i < 40; i++)
		DosSleep(25);
	if(phase != 3) {
		fprintf(stderr, "error connecting to daemon!\n");
		return False;
	}

	mutex_lock(global_lock, FALSE);
	if(!*procres) {
		EB_Process *process = scalloc(1, sizeof(EB_Process));
		*procres = createResource(EBPROCESS, process);
	}

	mutex_unlock(global_lock);
	Daemon_getPMHandle(*procres, NULL, NULL);
	mutex_lock(global_lock, FALSE);
	addResource(&processlist, *procres);
	mutex_unlock(global_lock);
	return True;
}

BOOL startDaemon() {
	STARTDATA sd = { 0 };
	ULONG pidsession;
	PID pid;
	APIRET ulrc;

	sd.Length = sizeof(STARTDATA);
	sd.FgBg = SSF_FGBG_BACK;
	sd.PgmName = "xdaemon.exe";
	sd.PgmControl = SSF_CONTROL_INVISIBLE;
	
	ulrc = DosStartSession(&sd, &pidsession, &pid);
	if(ulrc)
		fprintf(stderr, "SYS%04ld - Error starting xdaemon.exe (is it in path?)\n", ulrc);

	return (ulrc == 0);
}
