#include "ebhookdll.h"
#include <emx/startup.h>

EB_SemComplex xeventsem = { NULLHANDLE, NULLHANDLE, NULLHANDLE };
HMTX global_lock = NULLHANDLE;

unsigned long APIENTRY _DLL_InitTerm(unsigned long mod_handle, unsigned long flag) {
	switch(flag) {
		case 0:
			hk_module = mod_handle;
			mutex_createopen(&xeventsem.mutex);
			event_createopen(&xeventsem.read, TRUE);
			event_createopen(&xeventsem.write, FALSE);
			return 1;
		case 1:
			mutex_close(xeventsem.mutex);
			event_close(xeventsem.read);
			event_close(xeventsem.write);
			return 1;
	}
	return 0;
}
