#include "x11daemon.h"

void um_openPipe(EB_Resource *procres) {
	if(procres) {
		EB_Process *process = getResource(EBPROCESS, (XID)procres);
		process->pipeserver = nextpipe;
	}
	DosCreateNPipe("\\PIPE\\EVERBLUE", &nextpipe, NP_ACCESS_OUTBOUND,
			NP_NOWAIT | NP_UNLIMITED_INSTANCES, 8192, 0, 0);
	DosConnectNPipe(nextpipe);
}

void closePipe(HPIPE handle) {
	DosDisConnectNPipe(handle);
	DosClose(handle);
}
