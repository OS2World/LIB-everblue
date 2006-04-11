#include <io.h>
#include <stdlib.h>

void EMXRegisterHandle(int handle) {
	_imphandle(handle);
}

void EMXFree(void *data) {
	free(data);
}
