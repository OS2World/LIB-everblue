#define INCL_WIN
#define INCL_DOS
#include <os2emx.h>
#define EBHOOKDLL
#include "../EBhook.h"

#define UM_HOOKMSG (WM_USER+0)
#define WM_POINTERENTER 0x041e
#define WM_POINTERLEAVE 0x041f

BOOL hook_inputQueueHook(HAB hab, PQMSG pQmsg, USHORT fs);
VOID EXPENTRY hook_sendMsgHook(HAB hab, PSMHSTRUCT psmh, BOOL fInterTask);
EXPENTRY BOOL getWriteAccess(EB_SemComplex *semcomplex, BOOL force);
EXPENTRY void releaseWriteAccess(EB_SemComplex *semcomplex, BOOL force);
EXPENTRY void killBadProcess();
BOOL forceWrite(EB_SemComplex *semcomplex);

HMODULE hk_module;
HMTX global_lock;

EB_SemComplex xeventsem;
volatile BOOL returnvalue;
