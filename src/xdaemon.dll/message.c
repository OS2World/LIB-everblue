#include "daemondll.h"

EXPENTRY MRESULT Daemon_exec(EB_Resource *process, ULONG id, void *params, void *user, int size) {
	MRESULT result;
	void *usershared = NULL;

	if(user) {
		usershared = smalloc(size);
		memcpy(usershared, user, size);
	}
	Daemon_getPMHandle(process, NULL, NULL);
	result = WinSendMsg(objwindow, id, params, (MPARAM)usershared);
	if(user)
		sfree(usershared);
	return result;
}
