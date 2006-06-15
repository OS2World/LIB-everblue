#include "daemondll.h"
#include "common.h"

EXPENTRY MRESULT Daemon_exec(EB_Resource *procres, ULONG id, void *params) {
	ULONG temp;
	MRESULT result;
	EB_Process *process = getResource(EBPROCESS, (XID)procres);

	Daemon_getPMHandle(procres, NULL);
	if (EbLockMutexSem(process->postmtx)) {
		DosResetEventSem(process->postsem, &temp);
		WinPostMsg(objwindow, id, params, procres);
		DosWaitEventSem(process->postsem, SEM_INDEFINITE_WAIT);
		EbReleaseMutexSem(process->postmtx);
	}
	return process->postret;
}
