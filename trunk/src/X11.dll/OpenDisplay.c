#include "X11.h"

VOID APIENTRY ExitHandler(ULONG ulTerm);
void EMXRegisterHandle(int handle);

ULONG oldapptype;
PULONG apptypeaddr = NULL;
EB_Resource *process = NULL;

EXPENTRY Display *XOpenDisplay(_Xconst char *displayname) {
	DBUG_ENTER("XOpenDisplay")
	EB_Process *ebproc = getResource(EBPROCESS, (XID)process);

	fprintf(stderr, "Everblue built " __DATE__ " " __TIME__ "\n");
	if(ebproc && ebproc->display)
		DBUG_RETURN(ebproc->display);

	if(!apptypeaddr) {
		PPIB pib = NULL;

		/* Make this a PM app */
		DosGetInfoBlocks(NULL, &pib);
		apptypeaddr = &pib->pib_ultype;
		oldapptype = *apptypeaddr;
		*apptypeaddr = 3;
		DosExitList(EXLST_ADD, ExitHandler);
		setbuf(stdout, NULL);
		setbuf(stderr, NULL);
	}

	if(!Daemon_getProcess(&process))
		DBUG_RETURN(NULL)
	ebproc = getResource(EBPROCESS, (XID)process);

	Xlib_InitOS2I18N();

	ebproc->display = newdisplay();
	Xlib_InitAtoms(ebproc->display);

	ebproc->display->fd = open("\\PIPE\\EVERBLUE", O_RDONLY | O_BINARY);
	EMXRegisterHandle(ebproc->display->fd);	
	mutex_createopen(&ebproc->postmtx);
	event_createopen(&ebproc->postsem, FALSE);

	Daemon_exec(process, UM_OPENHANDLES, process);

	OsInitColors();

//	XkbUseExtension(ebproc->display, NULL, NULL);
	DBUG_RETURN(ebproc->display)
}


EXPENTRY int XCloseDisplay(Display* dpy) {
	DosClose(process->ebprocess->display->fd);
	Daemon_exec(process, UM_FREERESOURCES, process);
	freeOldHPS(&process->ebprocess->hpstodelete);
	freeOldHPS(&process->ebprocess->hpsres);
	Daemon_Thread_Close(&process);
	freeDisplay(dpy);
	if(apptypeaddr) {
		*apptypeaddr = oldapptype;
		apptypeaddr = NULL;
		DosExitList(EXLST_REMOVE, (PFNEXITLIST)ExitHandler);
	}
	return 0;
}

VOID APIENTRY ExitHandler(ULONG ulTerm)
{
	*apptypeaddr = oldapptype;
	Daemon_died(process);
	freeOldHPS(&process->ebprocess->hpstodelete);
	freeOldHPS(&process->ebprocess->hpsres);
	DosExitList(EXLST_EXIT, (PFNEXITLIST)NULL);
}
