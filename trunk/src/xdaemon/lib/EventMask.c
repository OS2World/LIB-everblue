#include "daemondll.h"

// check for critical (one-client) masks

Bool checkEventMask(EB_Resource *winchain, EB_Resource *ignoreproc, long mask) {
	EB_Resource *current = winchain;
	long mask2 = mask;

	if(mask2 &= (SubstructureRedirectMask | ResizeRedirectMask |
			ButtonPressMask) && current)
		while((current = current->next))
			if(current->event_mask & mask2 && current->ebresource->procres != ignoreproc)
				return False;
	return True;
}

// returns a EBEVENTMASK1

EXPENTRY EB_Resource *Daemon_findEventMask(EB_Resource *winchain, EB_Resource *procres) {
	EB_Resource *current = winchain;

	if(current)
		while((current = current->next))
			if(current->ebresource->procres == procres)
				return current;
	return NULL;
}

EXPENTRY int Daemon_addEventMask(Window w, EB_Resource *procres, long mask) {
	EB_Window *ebw = getResource(EBWINDOW, w);
	EB_Resource *found;

	if(!checkEventMask(ebw->event_masks, procres, mask))
		return BadAccess;

	found = Daemon_findEventMask(ebw->event_masks, procres);
	if(found)
		found->event_mask = mask;
	else {
		EB_Resource *em1 = createResource(EBEVENTMASK1, NULL);
		EB_Resource *em2 = createResource(EBEVENTMASK2, em1);

		em1->ebresource = em2;
		em1->event_mask = mask;
		em2->procres = procres;
		addResource(&ebw->event_masks, em1);
		addResource(&procres->ebprocess->event_masks, em2);
	}
	ebw->event_mask |= mask;

	return Success;
}

EXPENTRY void Daemon_freeEventMask(EB_Resource **chain) {
	EB_Resource *current = *chain;

	if(current) {
		EB_Resource *next = current->next;
		sfree(current);
		while((current = next)) {
			next = current->next;
			unChainResource(current->ebresource);
			freeResource(current);
		}
		sfree(current);
		*chain = NULL;
	}
}

EXPENTRY long Daemon_orEventMasks(EB_Resource *winchain) {
	EB_Resource *current = winchain;
	long result = 0;

	if(current)
		while((current = current->next))
			result |= current->event_mask;
	return result;
}
