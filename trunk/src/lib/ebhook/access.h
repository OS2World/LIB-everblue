
#ifndef ACCESS_H
#define ACCESS_H

BOOL EbHookInitializeAccess( VOID);
BOOL EbHookEnableAccess( VOID);
VOID EbHookTerminateAccess( VOID);
BOOL EbHookGetWriteAccess( BOOL fForce);
VOID EbHookReleaseWriteAccess( VOID);
BOOL EbHookGetReadAccess( VOID);
VOID EbHookReleaseReadAccess( VOID);
BOOL EbHookKeepWaitingForRequestAccess( ULONG ulTimeout);
VOID EbHookSetShutdownState( BOOL fNewState);
BOOL EbHookQueryShutdownState( VOID);

#endif // ACCESS_H

