/*
 * Shared heap functions for OS/2
 *
 * Initially commit 16 kb, add more when required
 *
 * NOTE: Hardcoded limit of 512 KB (increase when required)
 *
 * TODO: Not process/thread safe (initializing/destroying heap)
 * 
 * ASSUMPTION: Rtl library takes care of protection of heap increase/decrease
 *             (from multiple threads/processes)
 *
 * Copyright 1999 Sander van Leeuwen (sandervl@xs4all.nl)
 *
 */

#include "daemondll.h"
#include <umalloc.h>
#include <malloc.h>

#define MAX_HEAPSIZE (2*1048576)
#define PAGE_SIZE 4096
#define MAX_HEAPPAGES (MAX_HEAPSIZE/PAGE_SIZE)
#define INCR_HEAPSIZE (16*1024)
// #define LOGFILENAME "e:\\alloc.log"

#define PAG_ANY 0x00000400
#define QSV_VIRTUALADDRESSLIMIT 30 

//Global DLL Data

Heap_t sharedHeap = 0;
PVOID pSharedMem = NULL;
BYTE pageBitmap[MAX_HEAPPAGES] = { 0 };
ULONG refCount = 0;
FILE *alloclog;

void *getmoreShared(Heap_t pHeap, size_t *size, int *clean);
void releaseShared(Heap_t pHeap, void *block, size_t size);

//******************************************************************************
//******************************************************************************
BOOL InitializeSharedHeap()
{
    APIRET rc;
    ULONG  flAllocMem = 0, ulSysinfo;

    rc = DosQuerySysInfo(QSV_VIRTUALADDRESSLIMIT, QSV_VIRTUALADDRESSLIMIT, &ulSysinfo, sizeof(ulSysinfo));
    if (rc == 0 && ulSysinfo > 512)   //VirtualAddresslimit is in MB
    {
        flAllocMem = PAG_ANY;
    }

    if(pSharedMem == NULL) {
		int i;

  	    rc = DosAllocSharedMem(&pSharedMem, NULL, MAX_HEAPSIZE, PAG_READ|PAG_WRITE|OBJ_GETTABLE|flAllocMem);
  	    if(rc != 0) {
#ifdef LOGFILENAME
			alloclog = fopen(LOGFILENAME, "a");
    		fprintf(alloclog, "InitializeSharedHeap: DosAllocSharedMem failed with %d\n", rc);
			fclose(alloclog);
#endif
            return FALSE;
	    }
	    rc = DosSetMem(pSharedMem, INCR_HEAPSIZE, PAG_READ|PAG_WRITE|PAG_COMMIT);
	    if(rc != 0) {
    		DosFreeMem(pSharedMem);
#ifdef LOGFILENAME
			alloclog = fopen(LOGFILENAME, "a");
	        fprintf(alloclog, "InitializeSharedHeap: DosSetMem failed with %d\n", rc);
			fclose(alloclog);
#endif
	        return FALSE;
	    }
  	    sharedHeap = _ucreate(pSharedMem, INCR_HEAPSIZE, _BLOCK_CLEAN, _HEAP_REGULAR|_HEAP_SHARED,
                	          getmoreShared, releaseShared);

  	    if(sharedHeap == NULL) {
    		DosFreeMem(pSharedMem);
#ifdef LOGFILENAME
			alloclog = fopen(LOGFILENAME, "a");
	        fprintf(alloclog, "InitializeSharedHeap: _ucreate failed!\n");
			fclose(alloclog);
#endif
	        return FALSE;
	    }
#ifdef LOGFILENAME
//			alloclog = fopen(LOGFILENAME, "a");
//  	    fprintf(alloclog, "HeapShared: First InitializeSharedHeap %x %x\n", pSharedMem, sharedHeap);
//			fclose(alloclog);
#endif
	    for(i=0;i<INCR_HEAPSIZE/PAGE_SIZE;i++) {
    	  	pageBitmap[i] = 1; //mark as committed
    	}
    }
    else {
    	if(DosGetSharedMem(pSharedMem, PAG_READ|PAG_WRITE) != 0) {
#ifdef LOGFILENAME
			alloclog = fopen(LOGFILENAME, "a");
		    fprintf(alloclog, "InitializeSharedHeap: DosGetSharedMem failed!\n");
			fclose(alloclog);
#endif
		    return FALSE;
	    }
#ifdef LOGFILENAME
//			alloclog = fopen(LOGFILENAME, "a");
//  	    fprintf(alloclog, "HeapShared: InitializeSharedHeap %x %x refcount %d\n", pSharedMem, sharedHeap, refCount);
//			fclose(alloclog);
#endif
	    if(_uopen(sharedHeap) != 0) {
#ifdef LOGFILENAME
			alloclog = fopen(LOGFILENAME, "a");
    		fprintf(alloclog, "InitializeSharedHeap: unable to open shared heap!\n");
			fclose(alloclog);
#endif
		    return FALSE;
	    }
    }
    refCount++;
    return TRUE;
}
//******************************************************************************
//******************************************************************************
void DestroySharedHeap()
{
#ifdef LOGFILENAME
//	alloclog = fopen(LOGFILENAME, "a");
//	fprintf(alloclog, "HeapShared: DestroySharedHeap %d\n", refCount);
//	fclose(alloclog);
#endif
    if(--refCount == 0) {
  	    if(sharedHeap) {
    		_uclose(sharedHeap);
		    _udestroy(sharedHeap, _FORCE);
		    sharedHeap = NULL;
	    }
	    if(pSharedMem) {
    		DosFreeMem(pSharedMem);
		    pSharedMem = NULL;
	    }
    }
    else {
    	_uclose(sharedHeap);
    }
}
//******************************************************************************
//******************************************************************************
ULONG GetPageRangeFree(ULONG pageoffset)
{
	int i;

#ifdef LOGFILENAME
//	alloclog = fopen(LOGFILENAME, "a");
//	fprintf(alloclog, "HeapShared: GetPageRangeFree(%08xh)\n", pageoffset);
//	fclose(alloclog);
#endif

    for(i=pageoffset;i<MAX_HEAPPAGES;i++) {
        if(pageBitmap[i] == 1) {
		    break;
	    }
    }
    return i-pageoffset;
}
//******************************************************************************
//******************************************************************************
void *getmoreShared(Heap_t pHeap, size_t *size, int *clean)
{
    APIRET rc;
    PVOID newblock;
	int i;

#ifdef LOGFILENAME
//	alloclog = fopen(LOGFILENAME, "a");
//	fprintf(alloclog, "HeapShared: getmoreShared(%08xh, %08xh, %08xh)\n", pHeap, *size, *clean);
//	fclose(alloclog);
#endif

    /* round the size up to a multiple of 4K */
    // *size = (*size / 4096) * 4096 + 4096;
    // @@@PH speed improvement
    *size = (*size + 4096) & 0xFFFFF000;
    AssignMax(*size, INCR_HEAPSIZE);

    for(i=0;i<MAX_HEAPPAGES;i++)
    {
    	int nrpagesfree = GetPageRangeFree(i);
    	if(nrpagesfree >= *size/PAGE_SIZE) 
        {
			int j;

  		    newblock = (PVOID)((ULONG)pSharedMem + i*PAGE_SIZE);
  		    rc = DosSetMem(newblock, *size, PAG_READ|PAG_WRITE|PAG_COMMIT);
  		    if(rc != 0) {
#ifdef LOGFILENAME
				alloclog = fopen(LOGFILENAME, "a");
    			fprintf(alloclog, "getmoreShared: DosSetMem failed with %d\n", rc);
				fclose(alloclog);
#endif
			    return NULL;
  		    }
  		    for(j=0;j < *size/PAGE_SIZE; j++)
            {
  			    pageBitmap[i+j] = 1; //mark as committed
  		    }

		    *clean = _BLOCK_CLEAN;
#ifdef LOGFILENAME
//			alloclog = fopen(LOGFILENAME, "a");
//		    fprintf(alloclog, "HeapShared: getmoreShared %x %d\n", newblock, *size);
//			fclose(alloclog);
#endif
		    return newblock;
	    }
	    if(nrpagesfree)
    		i += nrpagesfree-1;
    }
#ifdef LOGFILENAME
	alloclog = fopen(LOGFILENAME, "a");
    fprintf(alloclog, "HeapShared: getmoreShared NOTHING LEFT (%d)\n", *size);
	fclose(alloclog);
#endif
    return NULL;
}
//******************************************************************************
//******************************************************************************
void releaseShared(Heap_t pHeap, void *block, size_t size)
{
    ULONG pagenr;
	int i;

#ifdef LOGFILENAME
//	alloclog = fopen(LOGFILENAME, "a");
//	fprintf(alloclog, "HeapShared: releaseShared %x %d\n", block, size);
//	fclose(alloclog);
#endif
    DosSetMem(block, size, PAG_READ|PAG_WRITE|PAG_DECOMMIT);

    pagenr  = (ULONG)block - (ULONG)pSharedMem;
    pagenr /= PAGE_SIZE;
    for(i=pagenr;i<pagenr+size/PAGE_SIZE;i++) {
  	    pageBitmap[i] = 0; //mark as decommitted
    }
}
//******************************************************************************
//******************************************************************************
EXPENTRY void *smalloc(int size)
{
    void *chunk;

	if(!size)
		size = 1;
    chunk = _umalloc(sharedHeap, size);
#ifdef LOGFILENAME
	alloclog = fopen(LOGFILENAME, "a");
    fprintf(alloclog, "smalloc %x returned %x\n", size, chunk);
	fclose(alloclog);
#endif
    return chunk;
}
//******************************************************************************
//******************************************************************************
EXPENTRY void *scalloc(int elements, int size)
{
    void *chunk;

	if(!elements)
		elements = 1;
	if(!size)
		size = 1;
    chunk = _ucalloc(sharedHeap, elements, size);
#ifdef LOGFILENAME
	alloclog = fopen(LOGFILENAME, "a");
    fprintf(alloclog, "scalloc %x * %x returned %x\n", elements, size, chunk);
	fclose(alloclog);
#endif
    return chunk;
}
//******************************************************************************
//******************************************************************************
EXPENTRY void *srealloc(void *mem, int size)
{
    void *chunk;

	if(!size);
		size = 1;
	chunk = realloc(mem, size);
#ifdef LOGFILENAME
	alloclog = fopen(LOGFILENAME, "a");
    fprintf(alloclog, "srealloc %x returned %x\n", mem, chunk);
	fclose(alloclog);
#endif
    return chunk;
}
//******************************************************************************
//******************************************************************************
EXPENTRY void sfree(void *mem)
{
    free(mem);
#ifdef LOGFILENAME
	alloclog = fopen(LOGFILENAME, "a");
    fprintf(alloclog, "sfree freed %x\n", mem);
	fclose(alloclog);
#endif
}
//******************************************************************************
//******************************************************************************
