
#ifndef SEM_H
#define SEM_H

BOOL EbCreateOpenMutexSem( PHMTX phmtx);
BOOL EbCloseMutexSem( PHMTX phmtx);
BOOL EbLockMutexSem( HMTX hmtx);
BOOL EbReleaseMutexSem( HMTX hmtx);

BOOL EbCreateOpenEventSem( PHEV phev, BOOL fInitiallyPosted);
BOOL EbCloseEventSem( PHEV phev);
BOOL EbPostEventSem( HEV hev);
BOOL EbWaitEventSem( HEV hev);
BOOL EbResetEventSem( HEV hev);

BOOL EbKillMutexSemOwner( HMTX hmtx);

#endif // SEM_H

