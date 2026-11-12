#ifndef TWAPI_H
#define TWAPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TW_audio.h"
#include "TW_framebuffer.h"
#include "TW_mutex.h"
#include "TW_platform.h"
#include "TW_window.h"

int TWAPI_Init( void );
void TWAPI_ShutDown( void );

#ifdef __cplusplus
}
#endif

#endif