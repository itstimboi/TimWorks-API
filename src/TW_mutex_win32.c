#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdlib.h>

#include "TW_mutex_win32.h"

struct TWAPI_Win32_Mutex
{
    CRITICAL_SECTION handle;
};

TWAPI_Win32_Mutex* TWAPI_Win32_CreateMutex(void)
{
    TWAPI_Win32_Mutex* mutex;

    mutex = (TWAPI_Win32_Mutex*)malloc(sizeof(TWAPI_Win32_Mutex));

    if (mutex == NULL)
        return NULL;

    InitializeCriticalSection(&mutex->handle);

    return mutex;
}

void TWAPI_Win32_DestroyMutex(TWAPI_Win32_Mutex* mutex)
{
    if (mutex == NULL)
        return;

    DeleteCriticalSection(&mutex->handle);

    free(mutex);
}

int TWAPI_Win32_LockMutex(TWAPI_Win32_Mutex* mutex)
{
    if (mutex == NULL)
        return 0;

    EnterCriticalSection(&mutex->handle);

    return 1;
}

int TWAPI_Win32_UnlockMutex(TWAPI_Win32_Mutex* mutex)
{
    if (mutex == NULL)
        return 0;

    LeaveCriticalSection(&mutex->handle);

    return 1;
}

int TWAPI_Win32_TryLockMutex(TWAPI_Win32_Mutex* mutex)
{
    if (mutex == NULL)
        return 0;

    return TryEnterCriticalSection(&mutex->handle) ? 1 : 0;
}