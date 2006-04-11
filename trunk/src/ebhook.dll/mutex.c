#include "ebhookdll.h"

extern volatile HAB hookhab;

EXPENTRY void mutex_lock(HMTX mutex, BOOL force) {
	while(DosRequestMutexSem(mutex, 300) == 640 && force)
		killBadProcess();
}

EXPENTRY void mutex_unlock(HMTX mutex) {
	DosReleaseMutexSem(mutex);
}

EXPENTRY void mutex_createopen(HMTX *pmutex) {
	if (*pmutex == NULLHANDLE)
		DosCreateMutexSem(NULL, pmutex, DC_SEM_SHARED, FALSE);
	else
		DosOpenMutexSem(NULL, pmutex);
}

EXPENTRY void event_createopen(HEV *pevent, BOOL state) {
	if (*pevent == NULLHANDLE)
		DosCreateEventSem(NULL, pevent, DC_SEM_SHARED, state);
	else
		DosOpenEventSem(NULL, pevent);
}

EXPENTRY void mutex_close(HMTX mutex) {
	DosCloseMutexSem(mutex);
}

EXPENTRY void event_close(HEV event) {
	DosCloseEventSem(event);
}

EXPENTRY BOOL getWriteAccess(EB_SemComplex *semcomplex, BOOL force) {
	if(force) {
/*		int i = 0;
		do {
			if(++i > 30) {
				semcomplex->finish = TRUE;
				DosBeep(1000, 300);
			}
			if(semcomplex->finish | !hookhab)
				return FALSE;
		} while(DosRequestMutexSem(semcomplex->mutex, 100));
*/
		if(!(forceWrite(semcomplex) || forceWrite(semcomplex) || forceWrite(semcomplex)))
			DosBeep(1000, 300);
		if(semcomplex->finish || !hookhab) {
//			DosReleaseMutexSem(semcomplex->mutex);
			return FALSE;
		} else
			return TRUE;
	} else {
		DosRequestMutexSem(semcomplex->mutex, SEM_INDEFINITE_WAIT);
		DosWaitEventSem(semcomplex->write, SEM_INDEFINITE_WAIT);
	}
	return TRUE;
}

EXPENTRY void releaseWriteAccess(EB_SemComplex *semcomplex, BOOL force) {
	ULONG temp;

	DosResetEventSem(semcomplex->write, &temp);
	DosPostEventSem(semcomplex->read);
}

EXPENTRY void getReadAccess(EB_SemComplex *semcomplex) {
	DosWaitEventSem(semcomplex->read, SEM_INDEFINITE_WAIT);
}

EXPENTRY void releaseReadAccess(EB_SemComplex *semcomplex) {
	ULONG temp;

	DosResetEventSem(semcomplex->read, &temp);
	DosPostEventSem(semcomplex->write);
}

EXPENTRY void killBadProcess() {
	PID pid;
	TID tid;
	ULONG count;

	if(!DosQueryMutexSem(global_lock, &pid, &tid, &count) && count) {
		DosKillProcess(DKP_PROCESS, pid);
		DosBeep(1000, 300);
	}
}

BOOL forceWrite(EB_SemComplex *semcomplex) {
	PID pid;
	PID oldpid = 0;
	TID tid;
	ULONG count;
	int i;

	if(!DosQueryMutexSem(global_lock, &pid, &tid, &count) && count)
		oldpid = pid;
	for(i = 0; i < 5; i++) {
		if(semcomplex->finish | !hookhab)
			return TRUE;
		if(!DosWaitEventSem(semcomplex->write, 50))
			return TRUE;
	}
	if(!DosQueryMutexSem(global_lock, &pid, &tid, &count) && count && oldpid == pid) {
		DosKillProcess(DKP_PROCESS, pid);
		DosBeep(1000, 300);
	}
	return FALSE;
}
