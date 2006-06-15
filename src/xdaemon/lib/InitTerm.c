#include "daemondll.h"
#include "common.h"
#include <emx/startup.h>

#define INCL_ERRORS
#define INCL_WIN
#define INCL_DOS
#include <os2emx.h>

unsigned long _DLL_InitTerm(unsigned long mod_handle, unsigned long flag) {
	BOOL fSuccess = FALSE;

	switch (flag) {
		case 0:
			if(_CRT_init() != 0)
				break;
			__ctordtorInit();
			if(!InitializeSharedHeap())
				break;
			fSuccess = TRUE;
			break;

		case 1:
			DestroySharedHeap();
			break;
	}
	return fSuccess;
}

