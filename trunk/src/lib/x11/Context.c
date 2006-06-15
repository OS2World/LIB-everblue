/* This module implements a simple sparse array.

   XSaveContext(a,b,c,d) will store d in position (a,b,c) of the array.
   XFindContext(a,b,c,&d) will set d to be the value in position (a,b,c).
   XDeleteContext(a,b,c) will delete the entry in (a,b,c).

   a is a display id, b is a resource id, and c is a Context.  d is just an
   XPointer.  This code will work with any range of parameters, but is geared
   to be most efficient with very few (one or two) different a's.

*/

#include "X11.h"

#define INITHASHMASK 63 /* Number of entries originally in the hash table. */

typedef struct _TableEntryRec {	/* Stores one entry. */
    XID 			rid;
    XContext			context;
    XPointer			data;
    struct _TableEntryRec	*next;
} TableEntryRec, *TableEntry;

typedef struct _XContextDB {	/* Stores hash table for one display. */
    TableEntry *table;		/* Pointer to array of hash entries. */
    int mask;			/* Current size of hash table minus 1. */
    int numentries;		/* Number of entries currently in table. */
    HMTX linfo;
} DBRec, *DB;

#ifdef MOTIFBC
static DB NullDB = (DB)0;
#endif

/* Given an XID and a context, returns a value between 0 and HashSize-1.
   Currently, this requires that HashSize be a power of 2.
*/

#define Hash(db,rid,context) \
    (db)->table[(((rid) << 1) + context) & (db)->mask]

/* Resize the given db */

static void ResizeTable(db)
    register DB db;
{
    DBUG_ENTER("ResizeTable")
    TableEntry *otable;
    register TableEntry entry, next, *pold, *head;
    register int i, j;

    otable = db->table;
    for (i = INITHASHMASK+1; (i + i) < db->numentries; )
	i += i;
    db->table = (TableEntry *) Xcalloc((unsigned)i, sizeof(TableEntry));
    if (!db->table) {
	db->table = otable;
	DBUG_VOID_RETURN;
    }
    j = db->mask + 1;
    db->mask = i - 1;
    for (pold = otable ; --j >= 0; pold++) {
	for (entry = *pold; entry; entry = next) {
	    next = entry->next;
	    head = &Hash(db, entry->rid, entry->context);
	    entry->next = *head;
	    *head = entry;
	}
    }
    Xfree((char *) otable);
    DBUG_VOID_RETURN;
}

static void _XFreeContextDB(display)
    Display *display;
{
    DBUG_ENTER("_XFreeContextDB")
    register DB db;
    register int i;
    register TableEntry *pentry, entry, next;

    db = display->context_db;
    if (db) {
	for (i = db->mask + 1, pentry = db->table ; --i >= 0; pentry++) {
	    for (entry = *pentry; entry; entry = next) {
		next = entry->next;
		Xfree((char *)entry);
	    }
	}
	Xfree((char *) db->table);
	EbCloseMutexSem(&db->linfo);
	Xfree((char *) db);
    }
    DBUG_VOID_RETURN;
}

/* Public routines. */

/* Save the given value of data to correspond with the keys XID and context.
   Returns nonzero error code if an error has occured, 0 otherwise.
   Possible errors are Out-of-memory.
*/   

#if NeedFunctionPrototypes
int XSaveContext(
    Display *display,
    register XID rid,
    register XContext context,
    _Xconst char* data)
#else
int XSaveContext(display, rid, context, data)
    Display *display;
    register XID rid;
    register XContext context;
    XPointer data;
#endif
{
    DBUG_ENTER("XSaveContext")
    DB *pdb;
    register DB db;
    TableEntry *head;
    register TableEntry entry;

#ifdef MOTIFBC
    if (!display) {
	pdb = &NullDB;
	db = *pdb;
    } else
#endif
    {
	LockDisplay(display);
	pdb = &display->context_db;
	db = *pdb;
	UnlockDisplay(display);
    }
    if (!db) {
	db = (DB) Xmalloc(sizeof(DBRec));
	if (!db)
	    DBUG_RETURN(XCNOMEM);
	db->mask = INITHASHMASK;
	db->table = (TableEntry *)Xcalloc(db->mask + 1, sizeof(TableEntry));
	if (!db->table) {
	    Xfree((char *)db);
	    DBUG_RETURN(XCNOMEM);
	}
	db->numentries = 0;
	EbCreateOpenMutexSem(&db->linfo);
#ifdef MOTIFBC
	if (!display) *pdb = db; else
#endif
	{
	    LockDisplay(display);
	    *pdb = db;
	    display->free_funcs->context_db = _XFreeContextDB;
	    UnlockDisplay(display);
	}
    }
    EbLockMutexSem(db->linfo);
    head = &Hash(db, rid, context);
    EbReleaseMutexSem(db->linfo);
    for (entry = *head; entry; entry = entry->next) {
	if (entry->rid == rid && entry->context == context) {
	    entry->data = (XPointer)data;
	    DBUG_RETURN(0);
	}
    }
    entry = (TableEntry) Xmalloc(sizeof(TableEntryRec));
    if (!entry)
	DBUG_RETURN(XCNOMEM);
    entry->rid = rid;
    entry->context = context;
    entry->data = (XPointer)data;
    entry->next = *head;
    *head = entry;
    EbLockMutexSem(db->linfo);
    db->numentries++;
    if (db->numentries > (db->mask << 2))
	ResizeTable(db);
    EbReleaseMutexSem(db->linfo);
    DBUG_RETURN(0);
}



/* Given an XID and context, returns the associated data.  Note that data 
   here is a pointer since it is a return value.  Returns nonzero error code
   if an error has occured, 0 otherwise.  Possible errors are Entry-not-found.
*/

int XFindContext(display, rid, context, data)
    Display *display;
    register XID rid;
    register XContext context;
    XPointer *data;		/* RETURN */
{
    DBUG_ENTER("XFindContext")
    register DB db;
    register TableEntry entry;

#ifdef MOTIFBC
    if (!display) db = NullDB; else
#endif
    {
	LockDisplay(display);
	db = display->context_db;
	UnlockDisplay(display);
    }
    if (!db)
	DBUG_RETURN(XCNOENT);
    EbLockMutexSem(db->linfo);
    for (entry = Hash(db, rid, context); entry; entry = entry->next)
    {
	if (entry->rid == rid && entry->context == context) {
	    *data = (XPointer)entry->data;
	    EbReleaseMutexSem(db->linfo);
	    DBUG_RETURN(0);
	}
    }
    EbReleaseMutexSem(db->linfo);
    DBUG_RETURN(XCNOENT);
}



/* Deletes the entry for the given XID and context from the datastructure.
   This returns the same thing that FindContext would have returned if called
   with the same arguments.
*/

int XDeleteContext(display, rid, context)
    Display *display;
    register XID rid;
    register XContext context;
{
    DBUG_ENTER("XDeleteContext")
    register DB db;
    register TableEntry entry, *prev;

#ifdef MOTIFBC
    if (!display) db = NullDB; else
#endif
    {
	LockDisplay(display);
	db = display->context_db;
	UnlockDisplay(display);
    }
    if (!db)
	DBUG_RETURN(XCNOENT);
    EbLockMutexSem(db->linfo);
    for (prev = &Hash(db, rid, context);
	 (entry = *prev);
	 prev = &entry->next) {
	if (entry->rid == rid && entry->context == context) {
	    *prev = entry->next;
	    Xfree((char *) entry);
	    db->numentries--;
	    if (db->numentries < db->mask && db->mask > INITHASHMASK)
		ResizeTable(db);
	    EbReleaseMutexSem(db->linfo);
	    DBUG_RETURN(0);
	}
    }
    EbReleaseMutexSem(db->linfo);
    DBUG_RETURN(XCNOENT);
}
