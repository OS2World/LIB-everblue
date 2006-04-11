#include "X11.h"

// server based extensions not supported
// for local ones XAddExtension() is used
// perhaps this function must be changed to simulate extensions

XExtCodes *XInitExtension(Display *dpy, _Xconst char *name) {
	DBUG_ENTER("XInitExtension")
	printf("Program would use extension %s\n", name);
	DBUG_RETURN(NULL);
}

Bool XQueryExtension(register Display *dpy, _Xconst char *name,
	int *major_opcode, int *first_event, int *first_error) {       
	DBUG_ENTER("XQueryExtension")
	fprintf(stderr,"XQueryExtension: '%s'\n",name);
	*major_opcode = 0;
	*first_event = 0;
	*first_error = 0;
	DBUG_RETURN(False);
}

char **XListExtensions(Display *dpy, int *nextensions) {
	*nextensions = 0;
	return(NULL);
}

int XFreeExtensionList (char **list) {
	return 1;
}

XExtCodes *XAddExtension (dpy)
    Display *dpy;
{
    register _XExtension *ext;

    LockDisplay (dpy);
    if (! (ext = (_XExtension *) Xcalloc (1, sizeof (_XExtension)))) {
	UnlockDisplay(dpy);
	return (XExtCodes *) NULL;
    }
    ext->codes.extension = dpy->ext_number++;

    /* chain it onto the display list */
    ext->next = dpy->ext_procs;
    dpy->ext_procs = ext;
    UnlockDisplay (dpy);

    return (&ext->codes);		/* tell him which extension */
}

static _XExtension *XLookupExtension (
	register Display *dpy,	/* display */
	register int extension)	/* extension number */
{
	register _XExtension *ext;
	for (ext = dpy->ext_procs; ext; ext = ext->next)
	    if (ext->codes.extension == extension) return (ext);
	return (NULL);
}

XExtData **XEHeadOfExtensionList(object)
    XEDataObject object;
{
    return *(XExtData ***)&object;
}

int
XAddToExtensionList(structure, ext_data)
    XExtData **structure;
    XExtData *ext_data;
{
    ext_data->next = *structure;
    *structure = ext_data;
    return 1;
}

XExtData *XFindOnExtensionList(structure, number)
    XExtData **structure;
    int number;
{
    XExtData *ext;

    ext = *structure;
    while (ext && (ext->number != number))
	ext = ext->next;
    return ext;
}

/*
 * Routines to hang procs on the extension structure.
 */
CreateGCType XESetCreateGC(dpy, extension, proc)
	Display *dpy;		/* display */
	int extension;		/* extension number */
	CreateGCType proc;	/* routine to call when GC created */
{
	register _XExtension *e;	/* for lookup of extension */
	register CreateGCType oldproc;
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->create_GC;
	e->create_GC = proc;
	UnlockDisplay(dpy);
	return (CreateGCType)oldproc;
}

CopyGCType XESetCopyGC(dpy, extension, proc)
	Display *dpy;		/* display */
	int extension;		/* extension number */
	CopyGCType proc;	/* routine to call when GC copied */
{
	register _XExtension *e;	/* for lookup of extension */
	register CopyGCType oldproc;
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->copy_GC;
	e->copy_GC = proc;
	UnlockDisplay(dpy);
	return (CopyGCType)oldproc;
}

FlushGCType XESetFlushGC(dpy, extension, proc)
	Display *dpy;		/* display */
	int extension;		/* extension number */
	FlushGCType proc;	/* routine to call when GC copied */
{
	register _XExtension *e;	/* for lookup of extension */
	register FlushGCType oldproc;
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->flush_GC;
	e->flush_GC = proc;
	UnlockDisplay(dpy);
	return (FlushGCType)oldproc;
}

FreeGCType XESetFreeGC(dpy, extension, proc)
	Display *dpy;		/* display */
	int extension;		/* extension number */
	FreeGCType proc;	/* routine to call when GC freed */
{
	register _XExtension *e;	/* for lookup of extension */
	register FreeGCType oldproc;
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->free_GC;
	e->free_GC = proc;
	UnlockDisplay(dpy);
	return (FreeGCType)oldproc;
}

CreateFontType XESetCreateFont(dpy, extension, proc)
	Display *dpy;		/* display */
	int extension;		/* extension number */
	CreateFontType proc;	/* routine to call when font created */
{
	register _XExtension *e;	/* for lookup of extension */
	register CreateFontType oldproc;
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->create_Font;
	e->create_Font = proc;
	UnlockDisplay(dpy);
	return (CreateFontType)oldproc;
}

FreeFontType XESetFreeFont(dpy, extension, proc)
	Display *dpy;		/* display */
	int extension;		/* extension number */
	FreeFontType proc;	/* routine to call when font freed */
{
	register _XExtension *e;	/* for lookup of extension */
	register FreeFontType oldproc;
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->free_Font;
	e->free_Font = proc;
	UnlockDisplay(dpy);
	return (FreeFontType)oldproc;
}

CloseDisplayType XESetCloseDisplay(dpy, extension, proc)
	Display *dpy;		/* display */
	int extension;		/* extension number */
	CloseDisplayType proc;	/* routine to call when display closed */
{
	register _XExtension *e;	/* for lookup of extension */
	register CloseDisplayType oldproc;
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->close_display;
	e->close_display = proc;
	UnlockDisplay(dpy);
	return (CloseDisplayType)oldproc;
}

BeforeFlushType XESetBeforeFlush(dpy, extension, proc)
	Display *dpy;		/* display */
	int extension;		/* extension number */
	BeforeFlushType proc;	/* routine to call on flush */
{
	register _XExtension *e;	/* for lookup of extension */
	register BeforeFlushType oldproc;
	register _XExtension *ext;
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->before_flush;
	e->before_flush = proc;
	for (ext = dpy->flushes; ext && ext != e; ext = ext->next)
	    ;
	if (!ext) {
	    e->next_flush = dpy->flushes;
	    dpy->flushes = e;
	}	    
	UnlockDisplay(dpy);
	return (BeforeFlushType)oldproc;
}

ErrorStringType XESetErrorString(dpy, extension, proc)
	Display *dpy;		/* display */
	int extension;		/* extension number */
	ErrorStringType proc;	/* routine to call when I/O error happens */
{
	register _XExtension *e;	/* for lookup of extension */
	register ErrorStringType oldproc;
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->error_string;
	e->error_string = proc;
	UnlockDisplay(dpy);
	return (ErrorStringType)oldproc;
}

PrintErrorType XESetPrintErrorValues(dpy, extension, proc)
	Display *dpy;		/* display */
	int extension;		/* extension number */
	PrintErrorType proc;	/* routine to call to print */
{
	register _XExtension *e;	/* for lookup of extension */
	register PrintErrorType oldproc;
	if ((e = XLookupExtension (dpy, extension)) == NULL) return (NULL);
	LockDisplay(dpy);
	oldproc = e->error_values;
	e->error_values = proc;
	UnlockDisplay(dpy);
	return (PrintErrorType)oldproc;
}
