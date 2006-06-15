#define INCL_WIN
#define INCL_DOS
#include <os2emx.h>
#include <emx/startup.h>

#define EBHOOKSRC
#include "EBhook.h"

#include "access.h"

// -------------------------------------------------------------

#define USEAPPHOOK 1
#if USEAPPHOOK
#define HOOKEDQUEUE HMQ_CURRENT
#else
#define HOOKEDQUEUE NULLHANDLE
#endif

// -------------------------------------------------------------


// global vars, used by other modules of ebhook and outside of DLL
HMTX global_lock     = NULLHANDLE;
volatile MPARAM msgparameter;
volatile QMSG msgxchange;
volatile BOOL returnvalue = FALSE;

// global vars, used by other modules of ebhook
HMODULE hk_module = NULLHANDLE;
volatile HAB hookhab = 0;

// global vars of this module
static HWND daemonhwnd;

// -------------------------------------------------------------

unsigned long _DLL_InitTerm(unsigned long mod_handle, unsigned long flag) {
	switch(flag) {
		case 0:
			hk_module = mod_handle;
			return EbHookInitializeAccess();
		case 1:
			EbHookTerminateAccess();
			return 1;
	}
	return 0;
}

// -------------------------------------------------------------

static BOOL _inputQueueHook(HAB hab, PQMSG pQmsg, USHORT fs) {
	switch(pQmsg->msg) {
	case WM_CHAR:
	case WM_PAINT:
	case WM_MOUSEMOVE:
	case WM_BUTTON1MOTIONSTART:
	case WM_BUTTON2MOTIONSTART:
	case WM_BUTTON3MOTIONSTART:
	case WM_BUTTON1MOTIONEND:
	case WM_BUTTON2MOTIONEND:
	case WM_BUTTON3MOTIONEND:
	case WM_SEM4:
	case WM_BUTTON1UP:
	case WM_BUTTON2UP:
	case WM_BUTTON3UP:
	case WM_BUTTON1DOWN:
	case WM_BUTTON1DBLCLK:
	case WM_BUTTON2DOWN:
	case WM_BUTTON2DBLCLK:
	case WM_BUTTON3DOWN:
	case WM_BUTTON3DBLCLK:
	case WM_POINTERENTER:
	case WM_POINTERLEAVE:
		break;
	default:
		return FALSE;
	}
	if(pQmsg->msg != WM_TIMER) {
		if(!EbHookGetWriteAccess( TRUE))
			return FALSE;
		msgxchange = *pQmsg;
		EbHookReleaseWriteAccess();
		WinSendMsg(daemonhwnd, UM_HOOKMSG, 0, 0);
//	      mutex_unlock(xeventsem.mutex);
	}
	return returnvalue;
}

// -------------------------------------------------------------

static VOID EXPENTRY _sendMsgHook(HAB hab, PSMHSTRUCT psmh, BOOL fInterTask) {
	switch(psmh->msg) {
	case WM_CREATE:
		msgparameter = ((CREATESTRUCT *)(psmh->mp2))->pCtlData;
	case WM_DESTROY:
	case WM_SETFOCUS:
	case WM_SHOW:
		break;
	default:
		return;
	}
	if(psmh->msg != WM_TIMER) {
		if(!EbHookGetWriteAccess( TRUE))
			return;
		msgxchange.msg = psmh->msg;
		msgxchange.hwnd = psmh->hwnd;
		msgxchange.mp1 = psmh->mp1;
		msgxchange.mp2 = psmh->mp2;
		EbHookReleaseWriteAccess();
		WinSendMsg(daemonhwnd, UM_HOOKMSG, 0, 0);
//	      mutex_unlock(xeventsem.mutex);
	}
}

// -------------------------------------------------------------

EXPENTRY VOID EbInstallHooks(HAB hab, HWND hwnd) {
	ULONG ulErrorCount = 0;
	hookhab = hab;
	daemonhwnd = hwnd;
	ulErrorCount += !WinSetHook(hab, HOOKEDQUEUE, HK_INPUT,   (PFN)_inputQueueHook, hk_module);
	ulErrorCount += !WinSetHook(hab, HOOKEDQUEUE, HK_SENDMSG, (PFN)_sendMsgHook,    hk_module);
	DosBeep(500, 300);
	if (ulErrorCount) {
		DosSleep(100);
		DosBeep(500, 300);
	}
}

// -------------------------------------------------------------

EXPENTRY VOID EbReleaseHooks(HAB hab) {
	ULONG ulErrorCount = 0;
	if(hookhab == hab) {
		ulErrorCount += !WinReleaseHook(hookhab, HOOKEDQUEUE, HK_INPUT,   (PFN)_inputQueueHook, hk_module);
		ulErrorCount += !WinReleaseHook(hookhab, HOOKEDQUEUE, HK_SENDMSG, (PFN)_sendMsgHook, hk_module);
		DosBeep(1000, 300);
		if (ulErrorCount) {
			DosSleep(100);
			DosBeep( 1000, 300);
		}
		hookhab = 0;
	}
}

// -------------------------------------------------------------

EXPENTRY VOID EbWaitForHookAccess( VOID) {
	while( EbHookKeepWaitingForRequestAccess( 300))
		EbKillMutexSemOwner( global_lock);
}

// -------------------------------------------------------------

EXPENTRY BOOL EbGetHookAccess( VOID) { return EbLockMutexSem(global_lock); }

// -------------------------------------------------------------

EXPENTRY VOID EbReleaseHookAccess( VOID) { EbReleaseMutexSem(global_lock); }

// -------------------------------------------------------------

EXPENTRY BOOL EbGetHookDataAccess( VOID) { return EbHookGetReadAccess(); }

// -------------------------------------------------------------

EXPENTRY VOID EbReleaseHookDataAccess( VOID) { EbHookReleaseReadAccess(); }

// -------------------------------------------------------------

EXPENTRY VOID EbSetHookShutdownState( BOOL fNewState) { EbHookSetShutdownState( fNewState); }

// -------------------------------------------------------------

EXPENTRY BOOL EbQueryHookShutdownState( VOID) { return EbHookQueryShutdownState(); }

