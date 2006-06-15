#define INCL_ERRORS
#define INCL_WIN
#define INCL_DOS
#include <os2emx.h>

#include <stdio.h>

#include "access.h"
#include "ebhook.h"

#define COMPILE_FPRINTF 0

// -------------------------------------------------------------

// global vars from ebhook.c
extern volatile HAB hookhab;

// global vars to control access on hook data
HMTX hmtxAccess      = NULLHANDLE;
HEV  hevReadAccess   = NULLHANDLE;
HEV  hevWriteAccess  = NULLHANDLE;
volatile BOOL fFinish = FALSE;

// -------------------------------------------------------------

// TODO: was macht diese Funktion ?

static BOOL _forceWriteAccess( VOID) {
	PID pid;
	PID oldpid = 0;
	TID tid;
	ULONG count;
	int i;

	// get id of process holding the global semaphore
	if(!DosQueryMutexSem( global_lock, &pid, &tid, &count) && count)
		oldpid = pid;

	for(i = 0; i < 5; i++) {
		if(fFinish | !hookhab)
			return TRUE;
		if(!DosWaitEventSem( hevWriteAccess, 50))
			return TRUE;
	}
	if(!DosQueryMutexSem( global_lock, &pid, &tid, &count) && count && oldpid == pid) {
		DosKillProcess(DKP_PROCESS, pid);
		DosBeep(1000, 300);
	}
	return FALSE;
}

// -------------------------------------------------------------

BOOL EbHookInitializeAccess( VOID) {
	ULONG ulError = 0;

	ulError += !EbCreateOpenMutexSem( &global_lock);
	ulError += !EbCreateOpenMutexSem( &hmtxAccess);
	ulError += !EbCreateOpenEventSem( &hevReadAccess, TRUE);
	ulError += !EbCreateOpenEventSem( &hevWriteAccess, FALSE);

	if (ulError) {
#if COMPILE_FPRINTF
		fprintf(stderr, "ebhook: error: cannot initialize hook access\n");
#endif
		EbHookTerminateAccess();
	}

	return (ulError == NO_ERROR);
}

// -------------------------------------------------------------

VOID EbHookTerminateAccess( VOID) {
	EbCloseMutexSem(&global_lock);
	EbCloseMutexSem(&hmtxAccess);
	EbCloseEventSem(&hevReadAccess);
	EbCloseEventSem(&hevWriteAccess);
}

// -------------------------------------------------------------

BOOL EbHookGetWriteAccess( BOOL fForce) {
	if(fForce) {

#if 0
		int i = 0;
		do {
			if(++i > 30) {
				fFinish = TRUE;
				DosBeep(1000, 300);
			}
			if( fFinish | !hookhab)
				return FALSE;
		} while(DosRequestMutexSem( hmtxAccess, 100));
#endif

		if(!(_forceWriteAccess() ||
		     _forceWriteAccess() ||
		     _forceWriteAccess()))
			DosBeep(1000, 300);
		if(fFinish || !hookhab) {
//			DosReleaseMutexSem( hmtxAccess);
			return FALSE;
		} else
			return TRUE;
	} else {
		DosRequestMutexSem( hmtxAccess, SEM_INDEFINITE_WAIT);
		DosWaitEventSem( hevWriteAccess, SEM_INDEFINITE_WAIT);
	}
	return TRUE;
}

// -------------------------------------------------------------

VOID EbHookReleaseWriteAccess( VOID) {
	ULONG ulPostCount;
	DosResetEventSem( hevWriteAccess, &ulPostCount);
	DosPostEventSem( hevReadAccess);
	return;
}

// -------------------------------------------------------------

BOOL EbHookGetReadAccess( VOID) {
	APIRET rc = DosWaitEventSem( hevReadAccess, SEM_INDEFINITE_WAIT);
#if COMPILE_FPRINTF
	if (rc)
		fprintf(stderr, "ebhook: error: cannot get read access, rc=%u\n", rc);
#endif
	return (rc == NO_ERROR);
}

// -------------------------------------------------------------

VOID EbHookReleaseReadAccess( VOID) {
	ULONG ulPostCount;
	DosResetEventSem(hevReadAccess, &ulPostCount);
	DosPostEventSem(hevWriteAccess);
	return;
}

// -------------------------------------------------------------

BOOL EbHookKeepWaitingForRequestAccess( ULONG ulTimeout) {
	return (DosRequestMutexSem( hmtxAccess, ulTimeout) == ERROR_TIMEOUT);
}

// -------------------------------------------------------------

VOID EbHookSetShutdownState( BOOL fNewState)
{
fFinish = fNewState;
return;
}

// -------------------------------------------------------------

BOOL EbHookQueryShutdownState( VOID)
{
return fFinish;
}

