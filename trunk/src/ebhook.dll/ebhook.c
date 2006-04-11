#include "ebhookdll.h"

volatile HAB hookhab = 0;
volatile QMSG msgxchange;
volatile MPARAM msgparameter;
volatile BOOL returnvalue = FALSE;
HWND daemonhwnd;

EXPENTRY void installhooks(HAB hab, HWND hwnd) {
	hookhab = hab;
	daemonhwnd = hwnd;
//	DosBeep(500, 300);
	WinSetHook(hab, NULLHANDLE, HK_INPUT, (PFN)hook_inputQueueHook, hk_module);
	WinSetHook(hab, NULLHANDLE, HK_SENDMSG, (PFN)hook_sendMsgHook, hk_module);
}

EXPENTRY void releasehooks(HAB hab) {
	if(hookhab == hab) {
		WinReleaseHook(hookhab, NULLHANDLE, HK_INPUT, (PFN)hook_inputQueueHook, hk_module);
		WinReleaseHook(hookhab, NULLHANDLE, HK_SENDMSG, (PFN)hook_sendMsgHook, hk_module);
//		DosBeep(1000, 300);
		hookhab = 0;
	}		
}

BOOL hook_inputQueueHook(HAB hab, PQMSG pQmsg, USHORT fs) {
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
		if(!getWriteAccess(&xeventsem, TRUE))
			return FALSE;
		msgxchange = *pQmsg;
		releaseWriteAccess(&xeventsem, TRUE);
		WinSendMsg(daemonhwnd, UM_HOOKMSG, 0, 0);
//		mutex_unlock(xeventsem.mutex);
	}
	return returnvalue;
}

VOID EXPENTRY hook_sendMsgHook(HAB hab, PSMHSTRUCT psmh, BOOL fInterTask) {
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
		if(!getWriteAccess(&xeventsem, TRUE))
			return;
		msgxchange.msg = psmh->msg;
		msgxchange.hwnd = psmh->hwnd;
		msgxchange.mp1 = psmh->mp1;
		msgxchange.mp2 = psmh->mp2;
		releaseWriteAccess(&xeventsem, TRUE);
		WinSendMsg(daemonhwnd, UM_HOOKMSG, 0, 0);
//		mutex_unlock(xeventsem.mutex);
	}
}
