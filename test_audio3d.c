
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "TW_audio.h"

#include <math.h>
#include <stdio.h>
#include <time.h>

#define PI 3.14159265358979323846

int main(void)
{
    if (!TWAPI_AudioInit())
    {
        fprintf(stderr, "Audio initialization failed.\n");
        return 1;
    }

    TWAPI_Audio* sound =
        TWAPI_LoadAudio("assets/effect.wav");

    if (!sound)
    {
        fprintf(stderr, "Could not load assets/effect.wav\n");
        TWAPI_AudioShutdown();
        return 1;
    }

    TWAPI_SetAudioLooping(sound, 1);
    TWAPI_SetAudioVolume(sound, 0.8f);
    TWAPI_SetAudioDistance(sound, 1.0f, 15.0f);
    TWAPI_SetAudioDopplerFactor(sound, 1.0f);

    TWAPI_SetAudioListener(
        (TWAPI_Vector3){ 0.0f, 0.0f, 0.0f },
        (TWAPI_Vector3){ 0.0f, 0.0f, -1.0f },
        (TWAPI_Vector3){ 0.0f, 1.0f, 0.0f }
    );

    TWAPI_SetAudioPosition(
        sound,
        (TWAPI_Vector3){ 5.0f, 0.0f, 0.0f }
    );

    if (!TWAPI_PlayAudio(sound))
    {
        fprintf(stderr, "Playback failed.\n");
        TWAPI_AudioShutdown();
        return 1;
    }

    printf("TimAPI 3D Audio Test\n");
    printf("The sound circles the listener.\n");
    printf("Press Ctrl+C to quit.\n");

    LARGE_INTEGER frequency;
    LARGE_INTEGER previous;
    LARGE_INTEGER current;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&previous);

    float angle = 0.0f;
    const float radius = 5.0f;
    const float angularSpeed = 0.8f;

    for (;;)
    {
        QueryPerformanceCounter(&current);

        float deltaTime =
            (float)(current.QuadPart - previous.QuadPart) /
            (float)frequency.QuadPart;

        previous = current;

        if (deltaTime > 0.1f)
            deltaTime = 0.1f;

        angle += angularSpeed * deltaTime;

        TWAPI_Vector3 position = {
            sinf(angle) * radius,
            0.0f,
            -cosf(angle) * radius
        };

        TWAPI_SetAudioPosition(sound, position);
        TWAPI_UpdateAudio(deltaTime);

        Sleep(1);
    }

    /* Normal loop exit can call TWAPI_AudioShutdown(). */
    TWAPI_AudioShutdown();
    return 0;
}
