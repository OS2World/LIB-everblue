#define UM_HOOK (WM_USER + 13)

typedef struct {
	HMTX mutex;
	HEV read;
	HEV write;
} EB_SemComplex;

EXPENTRY void releaseReadAccess(EB_SemComplex *semcomplex);
EXPENTRY void getReadAccess(EB_SemComplex *semcomplex);
EXPENTRY void mutex_unlock(HMTX mutex);
EXPENTRY void mutex_lock(HMTX mutex, BOOL weak);
EXPENTRY void mutex_close(HMTX mutex);
EXPENTRY void mutex_createopen(HMTX *pmutex);
EXPENTRY void releasehooks(HAB hab);
EXPENTRY void installhooks(HAB hab);

#ifndef EBHOOKDLL
	extern QMSG msgxchange;
	extern MPARAM msgparameter;
	extern EB_SemComplex xeventsem;
	extern Bool returnvalue;
	extern HMTX global_lock;
#endif
