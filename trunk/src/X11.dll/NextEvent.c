#include "X11.h"

extern long const _Xevent_to_mask[];      

/* 
 * I don't think passing the pointers throught the pipe
 * is a good idea, considering the code for peeking and
 * putting back events, I am leaving the passing of
 * pointers in but only as something to set off the fd
 * flag, I am using the EventQueue linked list for all 
 * reads and writes to the event queue.  Brian Smith
 */


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
/*	struct timeval tv = { 0, 0 };
	fd_set readables;*/

	/*FD_ZERO(&readables);
	FD_SET(display->fd, &readables);
	select(display->fd+1, &readables, NULL, NULL, &tv);
	DBUG_RETURN(FD_ISSET(display->fd, &readables));
	DBUG_RETURN(EventQueue?1:0);*/
	DBUG_RETURN(XEventsQueued(display, QueuedAfterFlush));
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

  	mutex_lock(event_lock, TRUE);

	/* Add the event to the top of the list */
	tmp = Xmalloc(sizeof(EB_Resource));
	tmp->next = EventQueue;
	EventQueue = tmp;

	memcpy(&EventQueue->xevent, event, sizeof(XEvent));
	display->qlen++;
	write(pmout[1], (char *) &EventQueue->xevent, sizeof(void *));
	mutex_unlock(event_lock);
	DBUG_RETURN(True);
}

#if 0

Bool XCheckWindowEvent(Display *display, Window w, long event_mask, XEvent *event_return)
{
	DBUG_ENTER("XCheckWindowEvent")
	EB_Resource *tmp, *prev = NULL;

  	mutex_lock(event_lock);
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
			mutex_unlock(event_lock);
			if (rc<0)
			    DBUG_RETURN(False);	/* broken by signal or alarm */
			DBUG_RETURN(True);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	mutex_unlock(event_lock);
	DBUG_RETURN(False);
}

Bool XCheckTypedWindowEvent(Display *display, Window w, int event_type, XEvent *event_return)
{
	DBUG_ENTER("XCheckTypedWindowEvent")
	EB_Resource *tmp, *prev = NULL;

  	mutex_lock(event_lock);
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
			mutex_unlock(event_lock);
			if (rc<0)
			    DBUG_RETURN(False);	/* broken by signal or alarm */
			DBUG_RETURN(True);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	mutex_unlock(event_lock);
	DBUG_RETURN(False);
}

Bool XCheckTypedEvent(Display *display, int event_type, XEvent *event_return)
{
	DBUG_ENTER("XCheckTypedEvent")
	EB_Resource *tmp, *prev = NULL;

  	mutex_lock(event_lock);
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
			mutex_unlock(event_lock);
			if (rc<0)
			    DBUG_RETURN(False);	/* broken by signal or alarm */
			DBUG_RETURN(True);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	mutex_unlock(event_lock);
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

  	mutex_lock(event_lock);
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
			mutex_unlock(event_lock);
			if (rc<0)
			    DBUG_RETURN(False);	/* broken by signal or alarm */
			DBUG_RETURN(True);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	mutex_unlock(event_lock);
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
	DBUG_RETURN(Daemon_addEventMask(w, process, event_mask));
}

/* Mode parameter appears to have no use on OS/2 since we cannot
 * flush the connection and see if there are more events.
 */
int XEventsQueued(Display *display, int mode)
{
	DBUG_ENTER("XEventsQueued")
	int qlen;

	if(!display)
		DBUG_RETURN(0);
	XFlush(display);
//	mutex_lock(event_lock);
	qlen = display->qlen;
//	mutex_unlock(event_lock);
	if(!qlen)
		XNoOp(display);
	DBUG_RETURN(qlen);
}

int XIfEvent (Display *display, XEvent *event_return, Bool (*predicate)(Display *display, XEvent *event, XPointer arg), XPointer arg) {
	DBUG_ENTER("XIfEvent")
	EB_Process *ebproc = getResource(EBPROCESS, (XID)process);
	EB_Resource *current = ebproc->event_queue;

	while(1) {
		mutex_lock(event_lock, TRUE);
		if(current)
			while((current = current->next)) {
				if((*predicate)(display, current->xevent, arg)) {
					memcpy(event_return, &current->xevent, sizeof(XEvent));
					freeResource(current);
					mutex_unlock(event_lock);
					DBUG_RETURN(True);
				}
		}
		mutex_unlock(event_lock);

		/* If we don't have an event waiting block until we have one */
		while(!display->qlen)
			XNoOp(display);
	}	
}

#if 0

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
	    mutex_lock(event_lock);
	    tmp = EventQueue;

	    while(tmp != NULL)
	    {
		if((*predicate)(display, tmp->xevent, arg))
		{
			memcpy(event, &tmp->xevent, sizeof(XEvent));
			attrib = WinQueryWindowPtr(tmp->xevent->xany.window,QWP_WINDOW);
			mutex_unlock(event_lock);
			DBUG_RETURN(True);
		}
		tmp = tmp->next;

	    }
	    mutex_unlock(event_lock);

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

  	mutex_lock(event_lock);
	while(tmp != NULL)
	{
	    if((*predicate)(display, tmp->xevent, arg))
	    {
		memcpy(event, &tmp->xevent, sizeof(XEvent));
		attrib = WinQueryWindowPtr(tmp->xevent->xany.window,QWP_WINDOW);
		Daemon_removeEvent(tmp->xevent);
		mutex_unlock(event_lock);
		DBUG_RETURN(True);
	    }
	    tmp = tmp->next;

	}
	mutex_unlock(event_lock);
	DBUG_RETURN(False);
}
#endif
