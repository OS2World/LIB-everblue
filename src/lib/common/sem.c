#define INCL_ERRORS
#define INCL_WIN
#define INCL_DOS
#include <os2emx.h>

#include <stdio.h>

#include "sem.h"

#define COMPILE_FPRINTF 0

// -------------------------------------------------------------

BOOL EbCreateOpenMutexSem( PHMTX phmtx) {
	APIRET rc = NO_ERROR;

	if (!phmtx)
		rc = ERROR_INVALID_PARAMETER;
	else if (!*phmtx)
		rc = DosCreateMutexSem( NULL, phmtx, DC_SEM_SHARED, FALSE);
	else
		rc = DosOpenMutexSem( NULL, phmtx);
#if COMPILE_FPRINTF
	if (rc)
		fprintf(stderr, "error: open/create mutex semaphore, rc=%u\n", rc);
#endif
	return (rc == NO_ERROR);
}

// -------------------------------------------------------------

BOOL EbCloseMutexSem( PHMTX phmtx) {
	APIRET rc = NO_ERROR;
	PID  pid;
	TID  tid;
	ULONG ulCount;

	if (!phmtx)
		rc = ERROR_INVALID_PARAMETER;
	else
		rc = DosCloseMutexSem( *phmtx);
#if COMPILE_FPRINTF
	if (rc)
		fprintf(stderr, "error: cannot close mutex semaphore, rc=%u\n", rc);
#endif
	return (rc == NO_ERROR);
}

// -------------------------------------------------------------

#define MUTEXLOCK_RETRIES   10
#define MUTEXLOCK_WAIT      200

BOOL EbLockMutexSem( HMTX hmtx) {
	APIRET rc = NO_ERROR;
	ULONG ulRetries = MUTEXLOCK_RETRIES;
	while (((rc = DosRequestMutexSem( hmtx, MUTEXLOCK_WAIT)) == ERROR_TIMEOUT ) &&
		(ulRetries))
		ulRetries--;
#if COMPILE_FPRINTF
	if (rc)
		fprintf(stderr, "error: cannot lock mutex semaphore, rc=%u\n", rc);
#endif
	return (rc == NO_ERROR);
}

// -------------------------------------------------------------

BOOL EbReleaseMutexSem( HMTX hmtx) {
	APIRET rc = NO_ERROR;
	rc = DosReleaseMutexSem( hmtx);
#if COMPILE_FPRINTF
	if (rc)
		fprintf(stderr, "error: cannot release mutex semaphore, rc=%u\n", rc);
#endif
	return (rc == NO_ERROR);
}

// =============================================================

BOOL EbCreateOpenEventSem( PHEV phev, BOOL fInitiallyPosted) {
	APIRET rc = NO_ERROR;
	if (!phev)
		rc = ERROR_INVALID_PARAMETER;
	else if (!*phev)
		rc = DosCreateEventSem( NULL, phev, DC_SEM_SHARED, fInitiallyPosted);
	else
		rc = DosOpenEventSem( NULL, phev);
#if COMPILE_FPRINTF
	if (rc)
		fprintf(stderr, "error: cannot open/create event semaphore, rc=%u\n", rc);
#endif
	return (rc == NO_ERROR);
}

// -------------------------------------------------------------

BOOL EbCloseEventSem( PHEV phev) {
	APIRET rc = NO_ERROR;
	ULONG ulPostCount;
	if (!phev)
		rc = ERROR_INVALID_PARAMETER;
	else
		rc = DosCloseEventSem( *phev);
#if COMPILE_FPRINTF
	if (rc)
		fprintf(stderr, "error: cannot close event semaphore, rc=%u\n", rc);
#endif
	return (rc == NO_ERROR);
}

// -------------------------------------------------------------

BOOL EbPostEventSem( HEV hev) { return (DosPostEventSem( hev) == NO_ERROR); }
BOOL EbWaitEventSem( HEV hev) { return (DosWaitEventSem( hev, SEM_INDEFINITE_WAIT) == NO_ERROR); }

// -------------------------------------------------------------

BOOL EbResetEventSem( HEV hev) {
	ULONG ulPostCount;
	return (DosResetEventSem( hev, &ulPostCount) == NO_ERROR);
}

// -------------------------------------------------------------

BOOL EbKillMutexSemOwner( HMTX hmtx) {
	APIRET rc = NO_ERROR;
	PID pid;
	TID tid;
	ULONG ulPostCount;

	if(!DosQueryMutexSem( hmtx, &pid, &tid, &ulPostCount) && ulPostCount) {
	        rc = DosKillProcess( DKP_PROCESS, pid);
	        DosBeep( 1000, 300);
	}
	return (rc == NO_ERROR);
}

