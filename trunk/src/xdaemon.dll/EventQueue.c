#include "daemondll.h"

XEvent *Daemon_copyEvent(register _XQEvent **newq, _XQEvent *origq);

int serial = 0;

EXPENTRY void Daemon_addEvent(_XQEvent *newq, EB_Resource *procres, Bool copy) {
	EB_Process *process = getResource(EBPROCESS, (XID)procres);
	register Display *display = process->display;
	LockDisplay(display);

/*	if(display->qlen >= 2048) {
		sfree(newq);
		fprintf(stderr, "Daemon_AddEvent: event pipe overflow!!!\n");
		return;
	}*/

	if(copy) {
		_XQEvent *newqold = newq;
		Daemon_copyEvent(&newq, newqold);
	}

	newq->event.xany.display = process->display;
	newq->next = NULL;
	if(display->tail)
		display->tail->next = newq;
	else
		display->head = newq;
	display->tail = newq;
	display->qlen++;
	XUnlockDisplay(display);

	write(process->pipeserver, (char *)&newq->event, sizeof(void *));
}

EXPENTRY XEvent *Daemon_createEvent(register _XQEvent **newq, Window w, int type)
{
	*newq = smalloc(sizeof(_XQEvent));
	XEvent *new = &(*newq)->event;

	new->type = type;
	new->xany.serial = serial++;
	new->xany.send_event = False;
	new->xany.window = w;
	return new;
}

XEvent *Daemon_copyEvent(register _XQEvent **newq, _XQEvent *origq)
{
	XEvent *new;
	*newq = smalloc(sizeof(_XQEvent));

	if(!*newq)
	{
		fprintf(stderr, "EventQueue.c/Daemon_CopyEvent: Error allocating memory\n");
		return NULL;
	}
	new = &(*newq)->event;
	memcpy(new, &origq->event, sizeof(XEvent));
	return new;
}

void Daemon_doEvent(_XQEvent *newq, EB_Resource *event_masks, int mask) {
	EB_Resource *current = event_masks;
	Bool copy = False;
	Display *first = NULL;

	if(current)
		while((current = current->next))
			if(current->event_mask & mask) {
				EB_Resource *current2 = getResource(EBEVENTMASK1, (XID)current);
				if(first == NULL) {
					EB_Process *process = getResource(EBPROCESS, (XID)current2->procres);
					first = process->display;
					LockDisplay(first);
				}
				Daemon_addEvent(newq, current2->procres, copy);
				copy = True;
			}
	if(first)
		XUnlockDisplay(first);
}

void Daemon_propagateEvent(_XQEvent *newq, int mask, BOOL weakMouseEvent) {
	char winclass[32];
	EB_Window *attrib;
	XEvent *new;
	SWP child;
	SWP parent;

	new = &newq->event;
	WinQueryWindowPos(new->xany.window, &child);
	while(TRUE) {
		EB_Window *ebw = getResource(EBWINDOW, new->xany.window);
		winclass[WinQueryClassName(ebw->hwnd, sizeof(winclass), winclass)] = 0;
		if(!strcmp(winclass,"XPMChild")) {
			if(new->type == MotionNotify && ebw->event_mask & PointerMotionHintMask) {
				if(weakMouseEvent)
					break;
				else
					new->xmotion.is_hint = NotifyHint;
			}
			if(ebw->event_mask & mask) {
				Daemon_doEvent(newq, ebw->event_masks, mask);
				return;
			} else
				if(ebw->do_not_propagate_mask & mask)
					break;
			new->xkey.subwindow = new->xany.window;
			new->xany.window = getWindow(getParent(ebw->hwnd), False, NULL);
			if(!new->xany.window)
				break;
			WinQueryWindowPos(ebw->hwnd, &parent);
			new->xkey.x += child.x + ebw->border_width;
			new->xkey.y += parent.cy - child.y - 1 + ebw->border_width;
			child.x = parent.x;
			child.y = parent.y;
			Daemon_doEvent(newq, ebw->event_masks, mask);
			return;
		} else
			break;
	}
	sfree(newq);
}

void Daemon_recurseEvent(_XQEvent *origq, XEvent *orig, int mask, Window stop)
{
#if 0
	char winclass[32];
	EB_Window *attrib;

	orig = origq->xevent;
	while(orig->xany.window != stop)
	{
		winclass[WinQueryClassName(orig->xany.window, sizeof(winclass), winclass)] = 0;
		if (strcmp(winclass,"XPMChild") == 0)
		{
			if (!mask || ((attrib = WinQueryWindowPtr(orig->xany.window, QWP_WINDOW))
				&& (attrib->winattrib->your_event_mask & mask)))
			{
				EB_Resource *newq;
				XEvent *new;

				new = Daemon_copyEvent(&newq, origq);
				Daemon_addEvent(newq, False);
			}
			orig->xany.window = WinQueryWindowULong(orig->xany.window, QWP_PARENTHWND);
		}
		else
			break;
	}
	sfree(origq);
#endif
}
#if 0
XEvent *Daemon_newEvent()
{
	EB_Resource *newq;

	/* Add event to the bottom of the list */
	mutex_lock(event_lock);
	newq = smalloc(sizeof(EB_Resource));
	newq->next = NULL;
	if(EventQueue == NULL)
		EventQueue = newq;
	else
	{
		EB_Resource *tmp;

		tmp = EventQueue;
		while(tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = newq;
	}
	mutex_unlock(event_lock);
	return newq->xevent;
}
#endif
EXPENTRY void Daemon_removeEvent(XEvent *ev)
{
#if 0
	EB_Window *attrib;
	EB_Resource *prev = NULL, *tmp = EventQueue;

	mutex_lock(event_lock);
	while(tmp != NULL)
	{
		if(tmp->xevent == ev)
		{
			if(prev) 
				prev->next = tmp->next;
			else
				EventQueue = tmp->next;
			sfree(tmp);
//			maindisplay->qlen--;
			attrib = WinQueryWindowPtr(tmp->xevent->xany.window, QWP_WINDOW);
			mutex_unlock(event_lock);
			return;
		}
		prev = tmp;
		tmp=tmp->next;
	}
	mutex_unlock(event_lock);
	return;
#endif
}

/* This is here because functions often times generate an event, 
 * so I figured it would be nice to be able to have a function to
 * send an Event and beable to specify if it was "send_event" or
 * not for convenience. Brian Smith.
 */
EXPENTRY Status Daemon_SendEvent(Display *display, Window w, Bool propagate, 
                  long event_mask, XEvent *event_send, Bool send_event)
{
#if 0
	XEvent *new;

	/* Need to look into how the event mask should be
	   handled here. */

	mutex_lock(event_lock);
	new = Daemon_newEvent();
	memcpy(new, event_send, sizeof(XEvent));
	/* Does serial get incremented in sent events? */
	new->xany.serial = 0;
	new->xany.send_event = send_event;
	new->xany.display = display;
	new->xany.window = w;
	display->qlen++;
	write(pmout[1], (char *) &new, sizeof(void *));
	mutex_unlock(event_lock);
#endif
	return True;
}

EXPENTRY void Daemon_freeEventQueue(Display *display) {
	_XQEvent *current = display->head;

	if(current) {
		_XQEvent *next = current->next;
		sfree(current);
		while((current = next)) {
			next = current->next;
			sfree(current);
		}
		display->head = display->tail = NULL;
	}
}
