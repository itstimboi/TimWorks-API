#include <stdlib.h>

#include "TW_mutex_internal.h"

#if defined(_WIN32)
#include "TW_mutex_win32.h"
#endif

TWAPI_Mutex* TWAPI_Mutex_CreateMutex(void)
{
    TWAPI_Mutex* mutex;
    void* handle;

    mutex = (TWAPI_Mutex*)malloc(sizeof(TWAPI_Mutex));

    if (mutex == NULL)
        return NULL;

    mutex->backend = TWAPI_MUTEX_BACKEND_NONE;
    mutex->handle = NULL;

    handle = NULL;

#if defined(_WIN32)

    handle = (void*)TWAPI_Win32_CreateMutex();

    if (handle == NULL)
    {
        free(mutex);
        return NULL;
    }

    mutex->backend = TWAPI_MUTEX_BACKEND_WIN32;

#else

    free(mutex);
    return NULL;

#endif

    mutex->handle = handle;

    return mutex;
}

void TWAPI_Mutex_DestroyMutex(TWAPI_Mutex* mutex)
{
    if (mutex == NULL)
        return;

    switch (mutex->backend)
    {
        case TWAPI_MUTEX_BACKEND_WIN32:

#if defined(_WIN32)
            TWAPI_Win32_DestroyMutex(
                (TWAPI_Win32_Mutex*)mutex->handle
            );
#endif

            break;

        case TWAPI_MUTEX_BACKEND_POSIX:
            break;

        default:
            break;
    }

    free(mutex);
}

int TWAPI_Mutex_LockMutex(TWAPI_Mutex* mutex)
{
    if (mutex == NULL || mutex->handle == NULL)
        return 0;

    switch (mutex->backend)
    {
        case TWAPI_MUTEX_BACKEND_WIN32:

#if defined(_WIN32)
            return TWAPI_Win32_LockMutex(
                (TWAPI_Win32_Mutex*)mutex->handle
            );
#else
            return 0;
#endif

        default:
            return 0;
    }
}

int TWAPI_Mutex_UnlockMutex(TWAPI_Mutex* mutex)
{
    if (mutex == NULL || mutex->handle == NULL)
        return 0;

    switch (mutex->backend)
    {
        case TWAPI_MUTEX_BACKEND_WIN32:

#if defined(_WIN32)
            return TWAPI_Win32_UnlockMutex(
                (TWAPI_Win32_Mutex*)mutex->handle
            );
#else
            return 0;
#endif

        default:
            return 0;
    }
}

int TWAPI_Mutex_TryLockMutex(TWAPI_Mutex* mutex)
{
    if (mutex == NULL || mutex->handle == NULL)
        return 0;

    switch (mutex->backend)
    {
        case TWAPI_MUTEX_BACKEND_WIN32:

#if defined(_WIN32)
            return TWAPI_Win32_TryLockMutex(
                (TWAPI_Win32_Mutex*)mutex->handle
            );
#else
            return 0;
#endif

        default:
            return 0;
    }
}