#include "X11.h"

int Xlib_DebugEnter(char* name,int lnum,char* proc, char* info)
{
	int DebugLevel = ++Xlib_DebugOffset;
//if(DebugLevel == 1)printf("Entered (%ld): %s\n", DebugLevel, proc);
//if(DebugLevel == 1)DosSleep(50);
	Xlib_DebugInfo[DebugLevel].filename = name;
	Xlib_DebugInfo[DebugLevel].linenumb = lnum;
	Xlib_DebugInfo[DebugLevel].procname = proc;
	Xlib_DebugInfo[DebugLevel].procinfo = info;
	return DebugLevel;
}
