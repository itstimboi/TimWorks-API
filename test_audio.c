#include "TW_audio.h"

#include <stdio.h>

int main(void)
{
    if (!TWAPI_AudioInit())
    {
        fprintf(stderr, "Failed to initialize TimAPI audio.\n");
        return 1;
    }

    TWAPI_Audio* music =
        TWAPI_LoadAudio("assets/music.ogg");

    TWAPI_Audio* effect =
        TWAPI_LoadAudio("assets/effect.wav");

    if (!music || !effect)
    {
        fprintf(stderr, "Failed to load audio files.\n");
        TWAPI_AudioShutdown();
        return 1;
    }

    TWAPI_SetAudioLooping(music, 1);
    TWAPI_SetAudioVolume(music, 0.5f);
    TWAPI_SetAudioPan(music, 0.0f);

    TWAPI_PlayAudio(music);

    printf("TimAPI Audio Test\n");
    printf("-----------------\n");
    printf("1 = Play sound effect\n");
    printf("2 = Pause music\n");
    printf("3 = Resume music\n");
    printf("4 = Pan left\n");
    printf("5 = Pan center\n");
    printf("6 = Pan right\n");
    printf("7 = Stop music\n");
    printf("8 = Rewind music\n");
    printf("9 = Quit\n");

    int running = 1;

    while (running)
    {
        int choice;

        printf("> ");

        if (scanf("%d", &choice) != 1)
            break;

        switch (choice)
        {
            case 1:
                TWAPI_PlayAudio(effect);
                break;

            case 2:
                TWAPI_PauseAudio(music);
                break;

            case 3:
                TWAPI_ResumeAudio(music);
                break;

            case 4:
                TWAPI_SetAudioPan(music, -1.0f);
                printf("Panning left.\n");
                break;

            case 5:
                TWAPI_SetAudioPan(music, 0.0f);
                printf("Panning center.\n");
                break;

            case 6:
                TWAPI_SetAudioPan(music, 1.0f);
                printf("Panning right.\n");
                break;

            case 7:
                TWAPI_StopAudio(music);
                break;

            case 8:
                TWAPI_RewindAudio(music);
                TWAPI_PlayAudio(music);
                break;

            case 9:
                running = 0;
                break;

            default:
                printf("Unknown option.\n");
                break;
        }
    }

    /*
        Shutdown releases all registered audio objects,
        including music and effect.
    */
    TWAPI_AudioShutdown();

    return 0;
}