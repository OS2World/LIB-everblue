#include "X11.h"

// TODO initialize
LockInfoPtr _Xglobal_lock = NULL;
void (*_XCreateMutex_fn)(LockInfoPtr) = NULL;
void (*_XFreeMutex_fn)(LockInfoPtr) = NULL;
void (*_XLockMutex_fn)(LockInfoPtr) = NULL;
void (*_XUnlockMutex_fn)(LockInfoPtr) = NULL;
