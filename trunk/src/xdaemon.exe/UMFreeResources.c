#include "x11daemon.h"

void um_freeResources(EB_Resource *procres) {
	EB_Process *process = getResource(EBPROCESS, (XID)procres);
	EB_Resource *current = process->res;

	if(current) {
		EB_Resource *next = current->next;
		while((current = next)) {
			next = current->next;
			unChainResource(current);
			if(current->restype != EBWINDOW)
				unMonitorResource(current);
			else
				um_destroyWindow(current); // TODO save set, close down mode
		}
		sfree(current);
		process->res = NULL;
	}

	Daemon_freeEventMask(&process->event_masks);
	closePipe(process->pipeserver);
	Daemon_freeEventQueue(&process->event_queue);
}
