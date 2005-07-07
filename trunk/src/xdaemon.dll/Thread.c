/* Each thread needs its own PM handles,
   but X11 functions can be invoked from
   any thread with the same display handle!
   We must cache the handles for each thread
   by allocating thread local memory.
   We always have to check the pid, as
   fork() is not able to clone TLM.
                                 ShadoW
*/

#include "daemondll.h"

Bool checkProcesses = False;

PVOID G_pvLocalInfoSeg = NULL;
// inexpensive way to check current PID by umoeller (xwphelpers)

static VOID GetInfoSegs() {
    SEL GlobalInfoSegSelector, LocalInfoSegSelector;

	_THUNK_PROLOG(8);
	_THUNK_FLAT(&GlobalInfoSegSelector);
	_THUNK_FLAT(&LocalInfoSegSelector);
	_THUNK_CALL(Dos16GetInfoSeg);

    G_pvLocalInfoSeg  = (PVOID)((LocalInfoSegSelector << 0x000D) & 0x01fff0000);
}

PID doshMyPID() {
    if (!G_pvLocalInfoSeg)
        GetInfoSegs();

    return *(PID *)G_pvLocalInfoSeg;
}

EXPENTRY void Daemon_getPMHandle(EB_Resource *procres, HAB *hab, HMQ *hmq) {
	PID current;
	EB_Process *process = getResource(EBPROCESS, (XID)procres);
	current = doshMyPID();

	if(current != process->pid) {
		DosAllocThreadLocalMemory(1, (PULONG *)&process->tlm);
		process->threadcount = 0;
		process->pid = current;
	}
	if(!*process->tlm) {
		*process->tlm = malloc(sizeof(TLM));
		(*process->tlm)->hab = WinInitialize(0);
		(*process->tlm)->hmq = WinCreateMsgQueue((*process->tlm)->hab, 0);
		WinGetLastError((*process->tlm)->hab); // PMERR_INV_MICROPS_FUNCTION???
		process->threadcount++; // TODO: locking can be faster done with inline assembly
	}
	if(hab)
		*hab = (*process->tlm)->hab;
	if(hmq)
		*hmq = (*process->tlm)->hmq;
}

EXPENTRY void Daemon_Thread_Close(EB_Resource **procres) {
	EB_Process *process = getResource(EBPROCESS, (XID)*procres);

	if(*process->tlm) { // we can only reach the current thread; PM does the rest
		WinDestroyMsgQueue((*process->tlm)->hmq);
		WinTerminate((*process->tlm)->hab);
		free(*process->tlm);
		process->threadcount--;
	}
	if(!process->threadcount) {
		DosFreeThreadLocalMemory((PULONG)process->tlm);
		freeResource(*procres);
		*procres = NULL;
	}
	else
		unChainResource(*procres);
}

/* special thread-safe code without locking
   possible because daemon is polling
   order in which checkProcesses is changed is important */

EXPENTRY void Daemon_died(EB_Resource *procres) {
	EB_Process *ebproc = getResource(EBPROCESS, (XID)procres);

	if(ebproc)
		ebproc->threadcount = -1;
	checkProcesses = True;
}

EXPENTRY void Daemon_doTasks(void freeMethod(EB_Resource *)) {
	EB_Resource *current = processlist;

	if(!checkProcesses)
		return;
	checkProcesses = False;
	if(current) {
		EB_Resource *next = current->next;
		while((current = next)) {
			EB_Process *process = getResource(EBPROCESS, (XID)current);
			next = current->next;
			if(process->threadcount == -1)
				freeMethod(current);
				freeResource(current);
		}
	}
}
