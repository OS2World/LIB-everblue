#include "ebhookdll.h"

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

EXPENTRY BOOL getWriteAccess(EB_SemComplex *semcomplex, BOOL weak) {
	if(weak) {
		if(DosRequestMutexSem(semcomplex->mutex, 300))
			return FALSE;
		if(DosWaitEventSem(semcomplex->write, 300)) {
			DosReleaseMutexSem(semcomplex->mutex);
			return FALSE;
		}
	} else {
		DosRequestMutexSem(semcomplex->mutex, SEM_INDEFINITE_WAIT);
		DosWaitEventSem(semcomplex->write, SEM_INDEFINITE_WAIT);
	}
	return TRUE;
}

EXPENTRY BOOL releaseWriteAccess(EB_SemComplex *semcomplex, BOOL synchronous, BOOL weak) {
	BOOL result = TRUE;
	ULONG temp;

	DosResetEventSem(semcomplex->write, &temp);
	DosPostEventSem(semcomplex->read);
	if(synchronous) {
		if(weak) {
			if(DosWaitEventSem(semcomplex->write, 300))
				result = FALSE;
		} else
			DosWaitEventSem(semcomplex->write, SEM_INDEFINITE_WAIT);
	}
	DosReleaseMutexSem(semcomplex->mutex);
	return result;
}

EXPENTRY void getReadAccess(EB_SemComplex *semcomplex) {
	while(!DosWaitEventSem(semcomplex->write, SEM_IMMEDIATE_RETURN) &&
			DosWaitEventSem(semcomplex->read, 100));
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

	if(!DosQueryMutexSem(global_lock, &pid, &tid, &count) && count)
		DosKillProcess(DKP_PROCESS, pid);
}
