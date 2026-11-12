
#ifndef TW_MUTEX_H
#define TW_MUTEX_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TWAPI_MutexBackend {
    TWAPI_MUTEX_BACKEND_NONE = 0,
    TWAPI_MUTEX_BACKEND_WIN32,
    TWAPI_MUTEX_BACKEND_POSIX
} TWAPI_MutexBackend;

typedef struct TWAPI_Mutex TWAPI_Mutex;

/* Create and destroy a mutex. */
TWAPI_Mutex* TWAPI_Mutex_CreateMutex(void);
void TWAPI_Mutex_DestroyMutex(TWAPI_Mutex* mutex);

/* Lock and unlock. */
int TWAPI_Mutex_LockMutex(TWAPI_Mutex* mutex);
int TWAPI_Mutex_UnlockMutex(TWAPI_Mutex* mutex);

/* Try to lock without waiting. */
int TWAPI_Mutex_TryLockMutex(TWAPI_Mutex* mutex);

#ifdef __cplusplus
}
#endif

#endif
