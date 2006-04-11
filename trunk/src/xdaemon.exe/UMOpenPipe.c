#include "x11daemon.h"

void um_openHandles(EB_Resource *procres) {
	if(procres) {
		EB_Process *process = getResource(EBPROCESS, (XID)procres);
		process->pipeserver = nextpipe;
		event_createopen(&process->postsem, FALSE);
	}
	DosCreateNPipe("\\PIPE\\EVERBLUE", &nextpipe, NP_ACCESS_OUTBOUND,
			NP_NOWAIT | NP_UNLIMITED_INSTANCES, 8192, 0, 0);
	DosConnectNPipe(nextpipe);
}

void closeHandles(HPIPE pipe, HEV event) {
	DosDisConnectNPipe(pipe);
	DosClose(pipe);
	if(event)
		event_close(event);
}
