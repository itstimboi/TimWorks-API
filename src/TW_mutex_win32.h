#ifndef TW_MUTEX_WIN32_H
#define TW_MUTEX_WIN32_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TWAPI_Win32_Mutex TWAPI_Win32_Mutex;

TWAPI_Win32_Mutex* TWAPI_Win32_CreateMutex(void);
void TWAPI_Win32_DestroyMutex(TWAPI_Win32_Mutex* mutex);

int TWAPI_Win32_LockMutex(TWAPI_Win32_Mutex* mutex);
int TWAPI_Win32_UnlockMutex(TWAPI_Win32_Mutex* mutex);

int TWAPI_Win32_TryLockMutex(TWAPI_Win32_Mutex* mutex);

#ifdef __cplusplus
}
#endif

#endif