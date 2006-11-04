#include "x11daemon.h"

Window current;
#if 0
void cursorMove(PPOINTL pptl) {
	Window new = getWindowFromPoint(pptl);
	if(new == current)
		return;
	current = new;
}
#endif