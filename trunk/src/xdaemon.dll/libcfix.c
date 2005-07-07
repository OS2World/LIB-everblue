#include <emx/umalloc.h>
#include <sys/limits.h>

int _uopen (Heap_t h)
{
  if (h->magic != _UM_MAGIC_HEAP
      || h == _um_regular_heap || h == _um_tiled_heap)
    return -1;
  if (_rmutex_open (&h->rsem) != 0)
    return -1;
  return 0;
}

unsigned _rmutex_open (_rmutex *sem)
{
  unsigned rc;

  rc = _fmutex_open (&sem->fm);
  if (rc == 0)
    {
      if (sem->count == USHRT_MAX)
        abort ();
      sem->count += 1;
    }
  return rc;
}
