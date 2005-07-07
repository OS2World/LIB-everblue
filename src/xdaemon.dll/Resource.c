#include "daemondll.h"

EXPENTRY EB_Resource *createResource(EB_ResID type, void *structure) {
	EB_Resource *res = smalloc(sizeof(EB_Resource));

	res->restype = type;
	res->refcount = 1;
	res->structure = structure;
	res->prev = NULL;
	res->next = NULL;

	return res;
}

EXPENTRY void *getResource(EB_ResID type, XID res) {
	EB_Resource *ebres = (EB_Resource *)res;

	if(!ebres || ebres->restype != type)
		return NULL;

	return ebres->structure;
}

EXPENTRY void addResource(EB_Resource **anchor, EB_Resource *new) {
	if(!*anchor)
		*anchor = createResource(EBANCHOR, NULL);
	new->prev = *anchor;
	new->next = (*anchor)->next;
	if(new->next)
		new->next->prev = new;
	new->prev->next = new;
}

EXPENTRY void addResourceLast(EB_Resource **anchor, EB_Resource *new) {
	EB_Resource *current;
	if(!*anchor)
		*anchor = createResource(EBANCHOR, NULL);

	current = *anchor;
	while(current->next)
		current = current->next;
	new->prev = current;
	new->prev->next = new;
}

EXPENTRY void unChainResource(EB_Resource *res) {
	if(res->next) {
		res->next->prev = res->prev;
	}
	if(res->prev) {
		res->prev->next = res->next;
		res->prev = NULL;
	}
	res->next = NULL;
}

EXPENTRY void freeResource(EB_Resource *res) {
	if(res->structure) {
		sfree(res->structure);
		res->structure = NULL;
	}
	unChainResource(res);
#ifdef DESTROY_XID
	sfree(res);
#endif
}

EXPENTRY void monitorResource(EB_Resource *res) {
	res->refcount++;
}

EXPENTRY void unMonitorResource(EB_Resource *res) {
	if(!--res->refcount)
		freeResource(res);
}
