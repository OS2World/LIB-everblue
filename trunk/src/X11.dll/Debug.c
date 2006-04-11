#include "X11.h"

int Xlib_DebugEnter(char* name,int lnum,char* proc, char* info) {
	int DebugLevel = ++Xlib_DebugOffset;
	if(DebugLevel == 1) {
		if(process) {
			HAB hab;
			Daemon_getPMHandle(process, &hab);
			int error = ERRORIDERROR(WinGetLastError(hab));
			if(error)
				printf("\nPM-Error: %x\n\n", error);
		}
		if(schck()) {
			fprintf(stderr, "corrupted shared heap\n");
			abort();
		}
//		DosSleep(50);
		printf("Entered (%d): %s", DebugLevel, proc);
	}
	Xlib_DebugInfo[DebugLevel].filename = name;
	Xlib_DebugInfo[DebugLevel].linenumb = lnum;
	Xlib_DebugInfo[DebugLevel].procname = proc;
	Xlib_DebugInfo[DebugLevel].procinfo = info;
	return DebugLevel;
}

void Xlib_DebugLeave(unsigned int a) {
	if(Xlib_DebugOffset == 1)
		printf(" -> %x\n", a);
}

void Xlib_DebugLeaveVoid() {
	if(Xlib_DebugOffset == 1)
		printf(" -|\n");
}
