#define INCL_WIN
#define INCL_DOS
#include <os2emx.h>
#define EBHOOKDLL
#include "../EBhook.h"

BOOL hook_inputQueueHook(HAB hab, PQMSG pQmsg, USHORT fs);
VOID EXPENTRY hook_sendMsgHook(HAB hab, PSMHSTRUCT psmh, BOOL fInterTask);
EXPENTRY BOOL getWriteAccess(EB_SemComplex *semcomplex, BOOL weak);
EXPENTRY BOOL releaseWriteAccess(EB_SemComplex *semcomplex, BOOL synchronous, BOOL weak);
EXPENTRY void event_createopen(HEV *pevent, BOOL state);
EXPENTRY void event_close(HEV event);
EXPENTRY void killBadProcess();

HMODULE hk_module;
HMTX global_lock;

EB_SemComplex xeventsem;
BOOL returnvalue;
