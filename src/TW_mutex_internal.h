#ifndef TW_MUTEX_INTERNAL_H
#define TW_MUTEX_INTERNAL_H

#include "TWAPI/TW_mutex.h"

struct TWAPI_Mutex
{
    TWAPI_MutexBackend backend;

    void* handle;
};

#endif