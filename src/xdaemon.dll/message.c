#include "daemondll.h"

EXPENTRY MRESULT Daemon_exec(EB_Resource *procres, ULONG id, void *params) {
	ULONG temp;
	MRESULT result;
	EB_Process *process = getResource(EBPROCESS, (XID)procres);

	Daemon_getPMHandle(procres, NULL);
	mutex_lock(process->postmtx, FALSE);
	DosResetEventSem(process->postsem, &temp);
	WinPostMsg(objwindow, id, params, procres);
	DosWaitEventSem(process->postsem, SEM_INDEFINITE_WAIT);
	mutex_unlock(process->postmtx);
	return process->postret;
}
