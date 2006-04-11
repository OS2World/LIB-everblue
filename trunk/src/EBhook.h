#define UM_HOOK (WM_USER + 13)

typedef struct {
	HMTX mutex;
	HEV read;
	HEV write;
	volatile BOOL finish;
} EB_SemComplex;

EXPENTRY void releaseReadAccess(EB_SemComplex *semcomplex);
EXPENTRY void getReadAccess(EB_SemComplex *semcomplex);
EXPENTRY void mutex_unlock(HMTX mutex);
EXPENTRY void mutex_lock(HMTX mutex, BOOL force);
EXPENTRY void mutex_close(HMTX mutex);
EXPENTRY void mutex_createopen(HMTX *pmutex);
EXPENTRY void event_createopen(HEV *pevent, BOOL state);
EXPENTRY void event_close(HEV event);
EXPENTRY void releasehooks(HAB hab);
EXPENTRY void installhooks(HAB hab, HWND hwnd);

#ifndef EBHOOKDLL
	extern volatile QMSG msgxchange;
	extern volatile MPARAM msgparameter;
	extern EB_SemComplex xeventsem;
	extern volatile Bool returnvalue;
	extern HMTX global_lock;
#endif
