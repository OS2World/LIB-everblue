#include "daemondll.h"
#include <emx/startup.h>

unsigned long _DLL_InitTerm(unsigned long mod_handle, unsigned long flag) {
	switch (flag) {
		case 0:
			if(_CRT_init() != 0)
				break;
			__ctordtorInit();
			mutex_createopen(&global_lock);
			InitializeSharedHeap();
			return 1;
		case 1:
			mutex_close(global_lock);
			DestroySharedHeap();
			return 1;
	}
	return 0;
}
