#include "x11daemon.h"

void shutDownPM() {
	WinDestroyWindow(mainhwnd);
	WinDestroyMsgQueue(hmq);
	WinTerminate(pmctls_hab);
}
