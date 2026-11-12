#include <TWAPI/TWAPI.h>

#include <stdio.h>

int main(void)
{
    // Checks if TimWorks API couldn't initialize.
    if (!TWAPI_Init())
    {
        fprintf(stderr, "Failed to initialize the TimWorks API.\n");
        return -1;
    }

    // Loads an audio clip. Supports MP3, WAV, FLAC, or VORBIS (OGG).
    TWAPI_Audio* music =
        TWAPI_LoadAudio("assets/music.ogg");

    // Loads an audio clip. Supports MP3, WAV, FLAC, or VORBIS (OGG).
    TWAPI_Audio* effect =
        TWAPI_LoadAudio("assets/effect.wav");

    // Checks if either audio clips failed to load.
    if (!music || !effect)
    {
        fprintf(stderr, "Failed to load audio files.\n");
        TWAPI_AudioShutdown();
        return 1;
    }

    // Sets panning on the "music" variable.
    TWAPI_SetAudioLooping(music, 1);
    TWAPI_SetAudioVolume(music, 0.5f);
    TWAPI_SetAudioPan(music, 0.0f);

    // Plays the "music" variable.
    TWAPI_PlayAudio(music);

    // Puts options on how to customize how a sound plays.
    printf("TimWorks API Audio Example\n");
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

    // This gives a lot of options for us.
    while (running)
    {
        int choice;

        printf("> ");

        if (scanf("%d", &choice) != 1)
            break;

        switch (choice)
        {
            case 1:
                // Plays the effect if the player types in 1.
                TWAPI_PlayAudio(effect);
                break;

            case 2:
                // Pauses the music if the player types in 2.
                TWAPI_PauseAudio(music);
                break;

            case 3:
                // Resumes the music if the player types in 3.
                TWAPI_ResumeAudio(music);
                break;

            case 4:
                // Sets the panning of the music to the left ear if the player types in 4.
                TWAPI_SetAudioPan(music, -1.0f);
                printf("Panning left.\n");
                break;

            case 5:
                // Sets the panning of the music to the center if the player types in 5.
                TWAPI_SetAudioPan(music, 0.0f);
                printf("Panning center.\n");
                break;

            case 6:
                // Sets the panning of the music to the right ear if the player types in 6.
                TWAPI_SetAudioPan(music, 1.0f);
                printf("Panning right.\n");
                break;

            case 7:
                // Stops the music if the player types in 7.
                TWAPI_StopAudio(music);
                break;

            case 8:
                // Rewinds the music if the player types in 8.
                TWAPI_RewindAudio(music);
                TWAPI_PlayAudio(music);
                break;

            case 9:
                // Shuts down the application if player types in 9.
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