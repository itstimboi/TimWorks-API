
#ifndef TW_AUDIO_H
#define TW_AUDIO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TWAPI_Audio TWAPI_Audio;

typedef struct TWAPI_Vector3
{
    float x;
    float y;
    float z;
} TWAPI_Vector3;

/* Engine lifetime */
int TWAPI_AudioInit(void);
void TWAPI_AudioShutdown(void);

/* Loading audio */
TWAPI_Audio* TWAPI_LoadAudio(const char* filename);

TWAPI_Audio* TWAPI_LoadAudioFromMemory(
    const void* bytes,
    size_t size
);

TWAPI_Audio* TWAPI_LoadAudioFromBase64(
    const char* base64
);

void TWAPI_FreeAudio(TWAPI_Audio* audio);

/* Playback */
int TWAPI_PlayAudio(TWAPI_Audio* audio);
void TWAPI_StopAudio(TWAPI_Audio* audio);
void TWAPI_PauseAudio(TWAPI_Audio* audio);
void TWAPI_ResumeAudio(TWAPI_Audio* audio);
int TWAPI_IsAudioPlaying(const TWAPI_Audio* audio);
int TWAPI_RewindAudio(TWAPI_Audio* audio);

/* Basic sound properties */
void TWAPI_SetAudioLooping(TWAPI_Audio* audio, int looping);

void TWAPI_SetAudioVolume(TWAPI_Audio* audio, float volume);
float TWAPI_GetAudioVolume(const TWAPI_Audio* audio);

void TWAPI_SetAudioPan(TWAPI_Audio* audio, float pan);
float TWAPI_GetAudioPan(const TWAPI_Audio* audio);

/* 3D listener */
void TWAPI_SetAudioListener(
    TWAPI_Vector3 position,
    TWAPI_Vector3 forward,
    TWAPI_Vector3 up
);

/* 3D sound source */
void TWAPI_SetAudioPosition(
    TWAPI_Audio* audio,
    TWAPI_Vector3 position
);

void TWAPI_SetAudioDistance(
    TWAPI_Audio* audio,
    float minDistance,
    float maxDistance
);

void TWAPI_SetAudioVelocity(
    TWAPI_Audio* audio,
    TWAPI_Vector3 velocity
);

void TWAPI_SetAudioDopplerFactor(
    TWAPI_Audio* audio,
    float factor
);

void TWAPI_DisableAudio3D(TWAPI_Audio* audio);

/*
    Call every frame with elapsed time in seconds.
    Smoothly moves 3D sound sources toward their target positions.
*/
void TWAPI_UpdateAudio(float deltaTime);

TWAPI_Audio* TWAPI_LoadAudioStream(const char* filename);

#ifdef __cplusplus
}
#endif

#endif
