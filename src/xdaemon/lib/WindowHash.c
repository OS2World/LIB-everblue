#include "daemondll.h"

#define HASHBITS 10
#define HASHSIZE 1 << HASHBITS
#define HASHMASK HASHSIZE - 1

EB_Resource *hashtable[HASHSIZE] = { NULL };

EXPENTRY Window getWindow(HWND hwnd, BOOL create, EB_Window *newebw) {
	EB_Resource *current = hashtable[hwnd & HASHMASK];
	EB_Window *ebw;

	if(!hwnd)
		return None;
	if(hwnd == realdesktop)
		hwnd = HWND_DESKTOP;
	while(current) {
		ebw = getResource(EBWINDOW, (XID)current);
		if(ebw->hwnd == hwnd)
			return (Window)current;
		current = ebw->next;
	}
	if(!create)
		return NULL;

// must be desktop, foreign child of desktop, XPMInput or XPMChild!

	if(!newebw)
		newebw = scalloc(1, sizeof(EB_Window));
	newebw->hwnd = hwnd;
	current = createResource(EBWINDOW, newebw);
	if(hashtable[hwnd & HASHMASK])
		ebw->next = current;
	else
		hashtable[hwnd & HASHMASK] = current;
	return (Window)current;
}

EXPENTRY Window removeWindow(HWND hwnd) {
	EB_Resource *current = hashtable[hwnd & HASHMASK];
	EB_Window *ebw = NULL;

	while(current) {
		EB_Window *old = ebw;
		ebw = getResource(EBWINDOW, (XID)current);
		if(ebw->hwnd == hwnd) {
			if(old)
				old->next = ebw->next;
			else
				hashtable[hwnd & HASHMASK] = ebw->next;
			return (Window)current;
		}
		current = ebw->next;
	}
	return NULL;
}
