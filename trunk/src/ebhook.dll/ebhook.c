#include "ebhookdll.h"

HAB hookhab = 0;
QMSG msgxchange;
MPARAM msgparameter;
BOOL returnvalue = FALSE;

EXPENTRY void installhooks(HAB hab) {
	hookhab = hab;
	WinSetHook(hab, NULLHANDLE, HK_INPUT, (PFN)hook_inputQueueHook, hk_module);
	WinSetHook(hab, NULLHANDLE, HK_SENDMSG, (PFN)hook_sendMsgHook, hk_module);
}

EXPENTRY void releasehooks(HAB hab) {
	if(hookhab == hab) {
		WinReleaseHook(hookhab, NULLHANDLE, HK_INPUT, (PFN)hook_inputQueueHook, hk_module);
		WinReleaseHook(hookhab, NULLHANDLE, HK_SENDMSG, (PFN)hook_sendMsgHook, hk_module);
		hookhab = 0;
	}		
}

BOOL hook_inputQueueHook(HAB hab, PQMSG pQmsg, USHORT fs) {
	switch(pQmsg->msg) {
	case WM_PAINT:
		break;
	default:
		return FALSE;
	}
	if(pQmsg->msg != WM_TIMER) {
		while(!getWriteAccess(&xeventsem, TRUE)) {
			killBadProcess();
			if(!hookhab)
				return FALSE;
		}
		msgxchange = *pQmsg;
		while(!releaseWriteAccess(&xeventsem, TRUE, TRUE)) {
			killBadProcess();
			if(!hookhab)
				return FALSE;
		}
	}
	return returnvalue;
}

VOID EXPENTRY hook_sendMsgHook(HAB hab, PSMHSTRUCT psmh, BOOL fInterTask) {
	switch(psmh->msg) {
	case WM_CREATE:
		msgparameter = ((CREATESTRUCT *)(psmh->mp2))->pCtlData;
	case WM_DESTROY:
		break;
	default:
		return;
	}
	if(psmh->msg != WM_TIMER) {
		while(!getWriteAccess(&xeventsem, TRUE)) {
			killBadProcess();
			if(!hookhab)
				return;
		}
		msgxchange.msg = psmh->msg;
		msgxchange.hwnd = psmh->hwnd;
		msgxchange.mp1 = psmh->mp1;
		msgxchange.mp2 = psmh->mp2;
		while(!releaseWriteAccess(&xeventsem, TRUE, TRUE) && hookhab)
			killBadProcess();
	}
}
