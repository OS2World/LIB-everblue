
#ifndef EBHOOK_H
#define EBHOOK_H

// -------------------------------------------------------------

// TODO: eliminate this or duplicate definition
//       in src\xdaemon\app\x11daemon.h

#define WM_POINTERENTER 0x041e
#define WM_POINTERLEAVE 0x041f

// -------------------------------------------------------------

// TODO: eliminate this or duplicate definition
//       in src\xdaemon\app\EBmessage.h

#define UM_HOOKMSG (WM_USER + 0)

// -------------------------------------------------------------

EXPENTRY VOID EbInstallHooks(HAB hab, HWND hwnd);
EXPENTRY VOID EbReleaseHooks(HAB hab);
EXPENTRY VOID EbWaitForHookAccess( VOID);
EXPENTRY BOOL EbGetHookAccess( VOID);
EXPENTRY VOID EbReleaseHookAccess( VOID);
EXPENTRY BOOL EbGetHookDataAccess( VOID);
EXPENTRY VOID EbReleaseHookDataAccess( VOID);
EXPENTRY VOID EbSetHookShutdownState( BOOL fNewState);
EXPENTRY BOOL EbQueryHookShutdownState( VOID);

// -------------------------------------------------------------

#ifndef EBHOOKSRC
        extern HMTX global_lock;
        extern volatile MPARAM msgparameter;
        extern volatile QMSG msgxchange;
        extern volatile BOOL returnvalue;
#endif

#endif // EBHOOK_H

