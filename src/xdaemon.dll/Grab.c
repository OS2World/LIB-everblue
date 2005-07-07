#include "daemondll.h"

#define hookart HK_INPUT

#define CLIENTHWND(wnd) WinWindowFromID(FID_CLIENT, wnd)
#define vk(key,shift)	(unsigned int)(((key) & 0x1ff) | (((shift) & 6) << 8))
#define vkmatch(a,b)	((a)&&(((a)& 0x7ff) == ((b)& 0x7ff)))

Xlib_Grab *Grab = NULL;
Xlib_Grab GrabList[1024], *FreeList[1024];
int LastGrab = 0, GrabAvail = 0;

char *AtomTbl[1024] = { "", NULL };
char AtomBuf[65536], *AtomTail = AtomBuf;
int LastAtom = 1;

int getpid(void) {
	PPIB pib;
	DosGetInfoBlocks(NULL, &pib);
	return pib->pib_ulpid;
}

Xlib_Grab *hook_newGrab(void)
{
	Xlib_Grab *newg;

	/* Add event to the bottom of the list */
	DosRequestMutexSem(hook_lock, SEM_INDEFINITE_WAIT);

	if (GrabAvail)
		newg = FreeList[--GrabAvail];
	else
		newg = &GrabList[LastGrab++];

		newg->special = 0;
	newg->pid = getpid();
	newg->next = NULL;

	if(Grab == NULL)
		Grab = newg;
	else
	{
		Xlib_Grab *tmp;

		tmp = Grab;
		while(tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = newg;
	}
	DosReleaseMutexSem(hook_lock);
	return newg;
}

void hook_removeGrab(int type)
{
	Xlib_Grab *prev = NULL, *tmp;
	int pid = getpid();

	DosRequestMutexSem(hook_lock, SEM_INDEFINITE_WAIT);

	tmp = Grab;
	while(tmp != NULL)
	{
		if(tmp->pid == pid && (tmp->type == type || type == GrabAny))
		{
			if(prev) 
				prev->next = tmp->next;
			else
				Grab = tmp->next;
			tmp->pid = tmp->type = 0;
			FreeList[GrabAvail++] = tmp;
			DosReleaseMutexSem(hook_lock);
			return;
		}
		prev = tmp;
		tmp=tmp->next;
	}
	DosReleaseMutexSem(hook_lock);
}

void hook_removeGrabAny(int type)
{
	Xlib_Grab *prev = NULL, *tmp;

		DosRequestMutexSem(hook_lock, SEM_INDEFINITE_WAIT);
		tmp = Grab;
	while(tmp != NULL)
	{
		if(tmp->type == type || type == GrabAny)
				{
			if(prev) 
				prev->next = tmp->next;
			else
				Grab = tmp->next;
			tmp->pid = tmp->type = 0;
						FreeList[GrabAvail++] = tmp;
						if(prev)
							tmp = prev;
						else
							tmp = Grab;
		}
				prev = tmp;
				if(tmp)
					tmp=tmp->next;
		}
	DosReleaseMutexSem(hook_lock);
}

Xlib_Grab *__FindGrab(int pid, int type)
{
	Xlib_Grab *tmp;

	DosRequestMutexSem(hook_lock, SEM_INDEFINITE_WAIT);
	tmp = Grab;
	while(tmp != NULL)
	{
		if((!pid || tmp->pid == pid) && (tmp->type == type || type == GrabAny))
		{
		DosReleaseMutexSem(hook_lock);
			return tmp;
		}
		
		tmp = tmp->next;
	}
	DosReleaseMutexSem(hook_lock);
	return NULL;
}

Xlib_Grab *hook_findGrab(int type)
{
	return __FindGrab(getpid(), type);
}

BOOL hook_inputQueueHook(HAB hab, PQMSG pQmsg, USHORT fs)
{
	Xlib_Grab *current;
	HWND client;

	return FALSE;

	if(!Grab)
		return FALSE;

	switch(pQmsg->msg)
	{
	case WM_CHAR:
		if ((current = __FindGrab(0,GrabKeyboard)) && 
		CLIENTHWND(pQmsg->hwnd) != (client = CLIENTHWND(current->window)))
		{
			if(current->special == SyncKeyboard)
			{
				current->special = 0;
				return FALSE;
			}
			WinPostMsg(client, pQmsg->msg, pQmsg->mp1, pQmsg->mp2);
			if(current->keyboard_mode == GrabModeAsync)
				return FALSE;
			else
				return TRUE;
		}
		if ((current = __FindGrab(0,GrabKey)) && 
		CLIENTHWND(pQmsg->hwnd) != (client = CLIENTHWND(current->window)))
		{
		int keycode;
			/* TODO: Antony, there probably needs to be keycode
			 translation done here, want to do it or should I
			 try and figure it out? :) Brian */
		/* RPLY: Brian, how's this code? %^) */
		if (SHORT1FROMMP(pQmsg->mp1) & KC_CHAR)
		keycode = vk(SHORT1FROMMP(pQmsg->mp2), \
			SHORT1FROMMP(pQmsg->mp1)&~(KC_SCANCODE|KC_VIRTUALKEY)); else
		if (SHORT1FROMMP(pQmsg->mp1) & KC_VIRTUALKEY)
		keycode = vk(SHORT2FROMMP(pQmsg->mp2), \
			SHORT1FROMMP(pQmsg->mp1)&~(KC_CHAR|KC_SCANCODE)); else
		if (SHORT1FROMMP(pQmsg->mp1) & KC_SCANCODE)
		keycode = vk(CHAR4FROMMP(pQmsg->mp1), \
			SHORT1FROMMP(pQmsg->mp1)&~(KC_CHAR|KC_VIRTUALKEY)); 
		else return TRUE;
			if (vkmatch(current->keycode, keycode))
			{
				WinPostMsg(client, pQmsg->msg, pQmsg->mp1, pQmsg->mp2);
				if(current->keyboard_mode == GrabModeAsync)
					return FALSE;
				else
					return TRUE;
			}
		}
		if((current = __FindGrab(0,GrabServer)) && 
		CLIENTHWND(pQmsg->hwnd) != (client = CLIENTHWND(current->window)))
		{
			WinPostMsg(client, pQmsg->msg, pQmsg->mp1, pQmsg->mp2);
			return TRUE;
		}
		break;
	case WM_BUTTON1UP:
	case WM_BUTTON2UP:
	case WM_BUTTON3UP:
	case WM_BUTTON1DOWN:
	case WM_BUTTON2DOWN:
	case WM_BUTTON3DOWN:
	case WM_MOUSEMOVE:
		if((current = __FindGrab(0,GrabPointer)) && 
		CLIENTHWND(pQmsg->hwnd) != (client = CLIENTHWND(current->window)))
		{
			if(current->special == SyncPointer)
			{
				current->special = 0;
				return FALSE;
			}
			WinPostMsg(client, pQmsg->msg, pQmsg->mp1, pQmsg->mp2);
			if(current->pointer_mode == GrabModeAsync)
				return FALSE;
			else
				return TRUE;
		}
		if((current = __FindGrab(0,GrabButton)) && 
		CLIENTHWND(pQmsg->hwnd) != (client = CLIENTHWND(current->window)))
		{
			if((current->button == 1 && (pQmsg->msg == WM_BUTTON1UP || pQmsg->msg == WM_BUTTON1DOWN)) ||
			   (current->button == 2 && (pQmsg->msg == WM_BUTTON3UP || pQmsg->msg == WM_BUTTON3DOWN)) ||
			   (current->button == 3 && (pQmsg->msg == WM_BUTTON2UP || pQmsg->msg == WM_BUTTON2DOWN)))
			{
				WinPostMsg(client, pQmsg->msg, pQmsg->mp1, pQmsg->mp2);
				if(current->pointer_mode == GrabModeAsync)
					return FALSE;
				else
					return TRUE;
			}
		}
		if((current = __FindGrab(0,GrabServer)) && 
		CLIENTHWND(pQmsg->hwnd) != (client = CLIENTHWND(current->window)))
		{
			WinPostMsg(client, pQmsg->msg, pQmsg->mp1, pQmsg->mp2);
			return TRUE;
		}
		break;
	}
		
	return FALSE;
}

Atom hook_XInternAtom(char* atom_name, Bool only_if_exists)
{
	char **tbl = AtomTbl;
	int i = 0;
	if (atom_name) {
		DosRequestMutexSem(hook_lock, SEM_INDEFINITE_WAIT);

		while (i<LastAtom) {
			if (!strcmp(*tbl, atom_name)) {
				DosReleaseMutexSem(hook_lock);
				return (Atom) i;
			}
			i++; tbl++;
		}
		if (!only_if_exists) {
			strcpy(AtomTbl[i = LastAtom++] = AtomTail, atom_name);
			AtomTail += strlen(atom_name)+1;
			DosReleaseMutexSem(hook_lock);
			return (Atom) i;
		}
		DosReleaseMutexSem(hook_lock);
	}
	return (Atom) 0;
}

char *hook_getAtomName(Atom atom)
{
	if (atom > 0 && atom < LastAtom) {
		return AtomTbl[atom];
	} else 
		return NULL;
}
