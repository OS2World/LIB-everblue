#include "X11.h"

extern long const _Xevent_to_mask[];      

/* _XReadEvents - Flush the output queue,
 * then read as many events as possible (but at least 1) and enqueue them
 */
void _XReadEvents(
	register Display *dpy)
{
	DBUG_ENTER("_XReadEvents")
	// TODO allow event write (partial unlock)
	while(!dpy->qlen)
		XNoOp(dpy); // TODO polling -> select
	// TODO disallow event write (partial lock)
	DBUG_VOID_RETURN;
}


/*
 * _XDeq - Remove event packet from the display's queue.
 */
void _XDeq(
    register Display *dpy,
    register _XQEvent *prev,	/* element before qelt */
    register _XQEvent *qelt)	/* element to be unlinked */
{
	DBUG_ENTER("_XDeq")
	XEvent *dummy;
	read(dpy->fd, (char *)&dummy, sizeof(void *));

    if (prev) {
	if ((prev->next = qelt->next) == NULL)
	    dpy->tail = prev;
    } else {
	/* no prev, so removing first elt */
	if ((dpy->head = qelt->next) == NULL)
	    dpy->tail = NULL;
    }
    qelt->qserial_num = 0;
    qelt->next = dpy->qfree;
    dpy->qfree = qelt;
    dpy->qlen--;
//printf("\n   event type: %x", qelt->event.type);
	if(&qelt->event != dummy)
		fprintf(stderr, "Pipe out of sync: queue %x (%ld), pipe %x (%ld)\n", &qelt->event, qelt->event.type, dummy, dummy->type);
	DBUG_VOID_RETURN;
}

/* 
 * I don't think passing the pointers throught the pipe
 * is a good idea, considering the code for peeking and
 * putting back events, I am leaving the passing of
 * pointers in but only as something to set off the fd
 * flag, I am using the EventQueue linked list for all 
 * reads and writes to the event queue.  Brian Smith
 */


#if 0
int XNextEvent(Display *display, XEvent *event)
{
	DBUG_ENTER("XNextEvent")
	EB_Process *ebproc = getResource(EBPROCESS, (XID)process);
//	int rc;
//	XEvent *ev;

	while(!display->qlen)
		XNoOp(display);

	/* For performance, assume pipe read/write is atomic */
#if 0
	rc = read(display->fd, (char *)&ev, sizeof(void *));
	if(rc < 0) {
		DBUG_RETURN(False);	/* broken by signal or alarm */
	}
#endif

	memcpy(event, ebproc->event_queue->next->xevent, sizeof(XEvent));
	freeResource(ebproc->event_queue->next);
	display->qlen--;

	DBUG_RETURN(True);
}

int XPending(Display *display)
{
	DBUG_ENTER("XPending")
	int ret = XEventsQueued(display, QueuedAfterFlush);
	DBUG_RETURN(ret);
}

int XPeekEvent(Display *display, XEvent *event_return) {
	DBUG_ENTER("XPeekEvent")
	EB_Process *ebproc = getResource(EBPROCESS, (XID)process);

	/* If we don't have an event waiting block until we have one */
	while(!display->qlen)
		XNoOp(display);

	/* Return the tail end of the queue */
	memcpy(event_return, ebproc->event_queue->next->xevent, sizeof(XEvent));

	DBUG_RETURN(True);
}

int XPutBackEvent(Display *display, XEvent *event)
{
	DBUG_ENTER("XPutBackEvent")
	EB_Resource *tmp;

  	EbLockMutexSem(event_lock);

	/* Add the event to the top of the list */
	tmp = Xmalloc(sizeof(EB_Resource));
	tmp->next = EventQueue;
	EventQueue = tmp;

	memcpy(&EventQueue->xevent, event, sizeof(XEvent));
	display->qlen++;
	write(pmout[1], (char *) &EventQueue->xevent, sizeof(void *));
	EbReleaseMutexSem(event_lock);
	DBUG_RETURN(True);
}

Bool XCheckWindowEvent(Display *display, Window w, long event_mask, XEvent *event_return)
{
	DBUG_ENTER("XCheckWindowEvent")
	EB_Resource *tmp, *prev = NULL;

  	EbLockMutexSem(event_lock);
	tmp = EventQueue;
	while(tmp != NULL)
	{
		if((tmp->xevent->xany.window == w) && (event_mask & _Xevent_to_mask[tmp->xevent->type]))
		{
			EB_Window *attrib;
			XEvent *ev;
			int rc = 0;

			memcpy(event_return, &tmp->xevent, sizeof(XEvent));

			if(prev)
			    prev->next = tmp->next;
			else
			    EventQueue = tmp->next;
			display->qlen--;

			Xfree(tmp);

			attrib = WinQueryWindowPtr(event_return->xany.window,QWP_WINDOW);

			/* For performance, assume pipe read/write is atomic */
			rc = read(display->fd, (char *)&ev, sizeof(void *));
			EbReleaseMutexSem(event_lock);
			if (rc<0)
			    DBUG_RETURN(False);	/* broken by signal or alarm */
			DBUG_RETURN(True);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	EbReleaseMutexSem(event_lock);
	DBUG_RETURN(False);
}

Bool XCheckTypedWindowEvent(Display *display, Window w, int event_type, XEvent *event_return)
{
	DBUG_ENTER("XCheckTypedWindowEvent")
	EB_Resource *tmp, *prev = NULL;

  	EbLockMutexSem(event_lock);
	tmp = EventQueue;
	while(tmp != NULL)
	{
		if((tmp->xevent->xany.window == w) && (event_type == tmp->xevent->type))
		{
			EB_Window *attrib;
			XEvent *ev;
			int rc = 0;

			memcpy(event_return, &tmp->xevent, sizeof(XEvent));

			if(prev)
			    prev->next = tmp->next;
			else
			    EventQueue = tmp->next;
			display->qlen--;

			Xfree(tmp);

			/* For performance, assume pipe read/write is atomic */
			attrib = WinQueryWindowPtr(event_return->xany.window,QWP_WINDOW);
			rc = read(display->fd, (char *)&ev, sizeof(void *));
			EbReleaseMutexSem(event_lock);
			if (rc<0)
			    DBUG_RETURN(False);	/* broken by signal or alarm */
			DBUG_RETURN(True);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	EbReleaseMutexSem(event_lock);
	DBUG_RETURN(False);
}

Bool XCheckTypedEvent(Display *display, int event_type, XEvent *event_return)
{
	DBUG_ENTER("XCheckTypedEvent")
	EB_Resource *tmp, *prev = NULL;

  	EbLockMutexSem(event_lock);
	tmp = EventQueue;
	while(tmp != NULL)
	{
		if(event_type == tmp->xevent->type)
		{
			EB_Window *attrib;
			XEvent *ev;
			int rc = 0;

			memcpy(event_return, &tmp->xevent, sizeof(XEvent));

			if(prev)
			    prev->next = tmp->next;
			else
			    EventQueue = tmp->next;
			display->qlen--;

			Xfree(tmp);

			/* For performance, assume pipe read/write is atomic */
			attrib = WinQueryWindowPtr(event_return->xany.window,QWP_WINDOW);
			rc = read(display->fd, (char *)&ev, sizeof(void *));
			EbReleaseMutexSem(event_lock);
			if (rc<0)
			    DBUG_RETURN(False);	/* broken by signal or alarm */
			DBUG_RETURN(True);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	EbReleaseMutexSem(event_lock);
	DBUG_RETURN(False);
}

int XWindowEvent(Display *display, Window w, long event_mask, XEvent *event_return)
{
    /* There is probably a better way of doing this but it
     is eluding me at the moment. - Brian */
    DBUG_ENTER("XWindowEvent")
    while(!XCheckWindowEvent(display, w, event_mask, event_return))
	XNoOp(display);
    DBUG_RETURN(True);
}


Bool XCheckMaskEvent(Display *display, long event_mask, XEvent *event_return)
{
	DBUG_ENTER("XCheckMaskEvent")
	EB_Resource *tmp, *prev = NULL;

  	EbLockMutexSem(event_lock);
	tmp = EventQueue;
	while(tmp != NULL)
	{
		if((event_mask & _Xevent_to_mask[tmp->xevent->type]))
		{
			EB_Window *attrib;
			XEvent *ev;
			int rc = 0;

			memcpy(event_return, &tmp->xevent, sizeof(XEvent));

			if(prev)
			    prev->next = tmp->next;
			else
			    EventQueue = tmp->next;
			display->qlen--;

			Xfree(tmp);

			/* For performance, assume pipe read/write is atomic */
			attrib = WinQueryWindowPtr(event_return->xany.window,QWP_WINDOW);
			rc = read(display->fd, (char *)&ev, sizeof(void *));
			EbReleaseMutexSem(event_lock);
			if (rc<0)
			    DBUG_RETURN(False);	/* broken by signal or alarm */
			DBUG_RETURN(True);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	EbReleaseMutexSem(event_lock);
	DBUG_RETURN(False);
}


int XMaskEvent(Display *display, long event_mask, XEvent *event_return)
{
    DBUG_ENTER("XMaskEvent")
    while(!XCheckMaskEvent(display, event_mask, event_return))
	XNoOp(display);
    DBUG_RETURN(True);
}


Status XSendEvent(Display *display, Window w, Bool propagate, 
		  long event_mask, XEvent *event_send)
{
    DBUG_ENTER("XSendEvent")
    Status result = Daemon_SendEvent(display, w, propagate, event_mask, event_send, TRUE);
    DBUG_RETURN(result);
}

#endif

int XSelectInput(Display *display, Window w, long event_mask)
{
	DBUG_ENTER("XSelectInput")
	int ret = Daemon_addEventMask(w, process, event_mask);
	DBUG_RETURN(ret);
}

/* Mode parameter appears to have no use on OS/2 since we cannot
 * flush the connection and see if there are more events.
 */
int _XEventsQueued(Display *display, int mode) {
	DBUG_ENTER("_XEventsQueued")
	int qlen;

	if(!display)
		DBUG_RETURN(0);
	XFlush(display);
//	EbLockMutexSem(event_lock);
	qlen = display->qlen;
//	EbReleaseMutexSem(event_lock);
//	if(!qlen)
//		XNoOp(display);
	DBUG_RETURN(qlen);
}

#if 0

int XIfEvent (Display *display, XEvent *event_return, Bool (*predicate)(Display *display, XEvent *event, XPointer arg), XPointer arg) {
	DBUG_ENTER("XIfEvent")
	EB_Process *ebproc = getResource(EBPROCESS, (XID)process);
	EB_Resource *current = ebproc->event_queue;

	while(1) {
		EbLockMutexSem(event_lock);
		if(current)
			while((current = current->next)) {
				if((*predicate)(display, current->xevent, arg)) {
					memcpy(event_return, &current->xevent, sizeof(XEvent));
					freeResource(current);
					EbReleaseMutexSem(event_lock);
					DBUG_RETURN(True);
				}
		}
		EbReleaseMutexSem(event_lock);

		/* If we don't have an event waiting block until we have one */
		while(!display->qlen)
			XNoOp(display);
	}	
}

int XPeekIfEvent (display, event, predicate, arg)
	register Display *display;
	register XEvent *event;
	Bool (*predicate)(
#if NeedNestedPrototypes
			  Display*			/* display */,
			  XEvent*			/* event */,
			  char*				/* arg */
#endif
			  );
	char *arg;
{
	DBUG_ENTER("XPeekIfEvent")
	EB_Window *attrib;
	EB_Resource *tmp;

	while(1)
	{
	    EbLockMutexSem(event_lock);
	    tmp = EventQueue;

	    while(tmp != NULL)
	    {
		if((*predicate)(display, tmp->xevent, arg))
		{
			memcpy(event, &tmp->xevent, sizeof(XEvent));
			attrib = WinQueryWindowPtr(tmp->xevent->xany.window,QWP_WINDOW);
			EbReleaseMutexSem(event_lock);
			DBUG_RETURN(True);
		}
		tmp = tmp->next;

	    }
	    EbReleaseMutexSem(event_lock);

    	/* If we don't have an event waiting block until we have one */
	    while (!EventQueue)
			XNoOp(display);
	}
	DBUG_RETURN(False);
}

int XCheckIfEvent (display, event, predicate, arg)
	register Display *display;
	Bool (*predicate)(
#if NeedNestedPrototypes
			  Display*			/* display */,
			  XEvent*			/* event */,
			  char*				/* arg */
#endif						     
			  );		/* function to call */
	register XEvent *event;		/* XEvent to be filled in. */
	char *arg;
{
	DBUG_ENTER("XCheckIfEvent")
	EB_Window *attrib;
	EB_Resource *tmp = EventQueue;

  	EbLockMutexSem(event_lock);
	while(tmp != NULL)
	{
	    if((*predicate)(display, tmp->xevent, arg))
	    {
		memcpy(event, &tmp->xevent, sizeof(XEvent));
		attrib = WinQueryWindowPtr(tmp->xevent->xany.window,QWP_WINDOW);
		Daemon_removeEvent(tmp->xevent);
		EbReleaseMutexSem(event_lock);
		DBUG_RETURN(True);
	    }
	    tmp = tmp->next;

	}
	EbReleaseMutexSem(event_lock);
	DBUG_RETURN(False);
}
#endif
