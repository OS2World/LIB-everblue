
#ifndef SEM_H
#define SEM_H

BOOL EbCreateMutexSem( PHMTX phmtx);
BOOL EbOpenMutexSem( HMTX hmtx);
BOOL EbCloseMutexSem( HMTX hmtx);
BOOL EbLockMutexSem( HMTX hmtx);
BOOL EbReleaseMutexSem( HMTX hmtx);

BOOL EbCreateEventSem( PHEV phev, BOOL fInitiallyPosted);
BOOL EbOpenEventSem( HEV hev);
BOOL EbCloseEventSem( HEV hev);
BOOL EbPostEventSem( HEV hev);
BOOL EbWaitEventSem( HEV hev);
BOOL EbResetEventSem( HEV hev);

BOOL EbKillMutexSemOwner( HMTX hmtx);

#endif // SEM_H

