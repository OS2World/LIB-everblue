#include "daemondll.h"

int serial = 0;

EXPENTRY void Daemon_addEvent(EB_Resource *newq, EB_Resource *procres, Bool copy) {
	XEvent *ev = newq->xevent;
	EB_Process *process = getResource(EBPROCESS, (XID)procres);

/*	if(process->display->qlen >= 2048) {
		sfree(newq);
		fprintf(stderr, "Daemon_AddEvent: event pipe overflow!!!\n");
		return;
	}*/

	ev->xany.display = process->display;
	addResourceLast(&process->event_queue, newq);

	process->display->qlen++;
	write(process->pipeserver, (char *)&ev, sizeof(void *));
//printf("Event added (%ld) for %x, %x\n", ev->type, ev->xany.window, newq);
}

EXPENTRY XEvent *Daemon_createEvent(register EB_Resource **newq, Window w, int type)
{
	XEvent *new = smalloc(sizeof(XEvent));

	new->type = type;
	new->xany.serial = serial++;
	new->xany.send_event = False;
	new->xany.window = w;
	*newq = createResource(EBEVENT, new);
	return new;
}

XEvent *Daemon_copyEvent(register EB_Resource **newq, EB_Resource *origq)
{
	XEvent *new;
	*newq = smalloc(sizeof(EB_Resource));

	if(!*newq)
	{
		fprintf(stderr, "EventQueue.c/Daemon_CopyEvent: Error allocating memory\n");
		return NULL;
	}
	new = (*newq)->xevent;
	memcpy(new, origq->xevent, sizeof(XEvent));
	return new;
}

void Daemon_doEvent(EB_Resource *newq, EB_Resource *event_masks, int mask) {
	EB_Resource *current = event_masks;
	Bool copy = False;

	if(current)
		while((current = current->next))
			if(current->event_mask & mask) {
				EB_Resource *current2 = getResource(EBEVENTMASK1, (XID)current);
				Daemon_addEvent(newq, current2->procres, copy);
				copy = True;
			}
}

void Daemon_propagateEvent(EB_Resource *newq, int mask, BOOL weakMouseEvent)
{
#if 0
	char winclass[32];
	EB_Window *attrib;
	XEvent *new;
	SWP child;
	SWP parent;

	new = newq->xevent;
	WinQueryWindowPos(new->xany.window, &child);
	while(TRUE)
	{
		winclass[WinQueryClassName(new->xany.window, sizeof(winclass), winclass)] = 0;
		if ((strcmp(winclass,"XPMChild") == 0)
			&& (attrib = WinQueryWindowPtr(new->xany.window, QWP_WINDOW)))
		{
			if (new->type == MotionNotify &&
				attrib->winattrib->your_event_mask & PointerMotionHintMask)
			{
				if(weakMouseEvent)
					break;
				else
					new->xmotion.is_hint = NotifyHint;
			}
			if (attrib->winattrib->your_event_mask & mask) {
				Daemon_addEvent(newq, False);
				return;
			}
			else
				if (attrib->winattrib->do_not_propagate_mask & mask)
					break;
			new->xkey.subwindow = new->xany.window;
			new->xany.window = WinQueryWindowULong(new->xany.window, QWP_PARENTHWND);
			WinQueryWindowPos(new->xany.window, &parent);
			new->xkey.x += child.x + attrib->winattrib->border_width;
			new->xkey.y += parent.cy - child.y - 1 + attrib->winattrib->border_width;
			child.x = parent.x;
			child.y = parent.y;
		}
		else
			break;
	}
	sfree(newq);
#endif
}

void Daemon_recurseEvent(EB_Resource *origq, XEvent *orig, int mask, Window stop)
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

EXPENTRY void Daemon_freeEventQueue(EB_Resource **chain) {
	EB_Resource *current = *chain;

	if(current) {
		EB_Resource *next = current->next;
		sfree(current);
		while((current = next)) {
			next = current->next;
			sfree(current->xevent);
			freeResource(current);
		}
		sfree(current);
		*chain = NULL;
	}
}
