# TimWorks API

This is an API I made specifically for TimWorks Engine.

# Code Examples

The following examples demonstrate how to use the main features of TimWorks API:

  1. Window Creation and OpenGL Context Creation: Create a window and optionally initialize OpenGL.
  2. Input: Read keyboard and mouse input.
  3. Audio: Load, play, pause, resume, stop, and control audio playback.
  4. Framebuffer: Draw using a CPU-side framebuffer and present the result in a window.

### Window Creation and OpenGL Context Creation

```c
#include <stdio.h>

#include <TWAPI/TWAPI.h>

#include <windows.h>

int main(void)
{

    // Checks if TimWorks API couldn't initialize.
    if (!TWAPI_Init())
    {
        fprintf(stderr, "Failed to initialize the TimWorks API.\n");
        return -1;
    }

    // This creates window properties
    TWAPI_WindowConfig winConfig = 
    {
        100,        // This is the X window position.
        100,        // This is the Y window position.
        800,        // This is the window's width.
        600,        // This is the window's height.
        "Title",    // This is the window's title.
        0,          // If this is equal to zero, then the window is unresizable, if it is, then it's resizable.
        0           // If this is equal to one, then the window is fullscreened, if it is equal to zero, then it's windowed.
    };

    // This creates the window from those window properties.
    TWAPI_Window* window = TWAPI_CreateWindowEx(&winConfig);

    // This checks if the failed.
    if (!window)
    {
        fprintf(stderr, "Failed to create TimWorks API window.\n");
        return -1;
    }

    // This enables VSync.
    TWAPI_SetVSync(window, 1);


    // This is an example on how to create an OpenGL context.
    /* TWAPI_OpenGLContextConfig config = 
    {
        1,                              // This is the OpenGL Major Version.
        1,                              // This is the OpenGL Minor Version.
        TWAPI_OPENGL_PROFILE_DEFAULT,   // This is the OpenGL Profile.
        0,                              // This is the OpenGL debug option.
        4                               // This is the MSAA (Anti-Aliasing) samples.
    };

    if (!TWAPI_CreateOpenGLContextEx(window, &config))
    {
        fprintf(stderr, "ERROR: Could not create OpenGL context.\n");
        TWAPI_DestroyWindow(window);
        return -1;
    } 
        
#include <GL/gl.h>
    */

    // This is the main game loop
    while (!TWAPI_WindowShouldClose(window))
    {
        // Handle Events.
        TWAPI_PollEvents();
        
        /*
        
        And now... it's your turn! 
        
        Put calculation code here!
        
        Put graphics code here!

        glViewport(0, 0, 800, 600);
        glClearColor(0.08f, 0.10f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);

            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex2f(0.5f, 0.0f);

            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex2f(0.0f, 1.0f);

            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(1.0f, 1.0f);

        glEnd();

        This shows a simple triangle that says "Hello!".

        */

        // Since the window doesn't use OpenGL, we have to handle framing ourselves.
        Sleep(16);
        // But if it does, use "TWAPI_SwapOpenGLBuffers(window);".
    }

    // This destroys the window we made.
    TWAPI_DestroyWindow(window);

    return 0;
}
```

### Input Example

```c

#include <stdio.h>
#include <windows.h>

#include <TWAPI/TWAPI.h>

int main(void)
{

    // Checks if TimWorks API couldn't initialize.
    if (!TWAPI_Init())
    {
        fprintf(stderr, "Failed to initialize the TimWorks API.\n");
        return -1;
    }

    // Creates a window.
    TWAPI_Window* window = TWAPI_CreateWindow(
        100, 100, 800, 600, "Input Showcase"
    );

    // Verifies if it's created.
    if (!window) {
        printf("Failed to create window.\n");
        return 1;
    }

    // Main game loop.
    while (!TWAPI_WindowShouldClose(window)) 
    {
        // Checks for events.
        TWAPI_PollEvents();

        // Checks if the W key is held down.
        if (TWAPI_IsKeyDown(window, TWAPI_KEY_W))
            printf("W is held\r");

        // Checks if the Space bar is pressed.
        if (TWAPI_IsKeyPressed(window, TWAPI_KEY_SPACE))
            printf("Space pressed!\n");

        // Checks if the Space Bar is released.
        if (TWAPI_IsKeyReleased(window, TWAPI_KEY_SPACE))
            printf("Space released!\n");

        // Checks if the left mouse button is pressed.
        if (TWAPI_IsMousePressed(window, TWAPI_MOUSE_LEFT))
            printf("Left mouse button pressed!\n");

        // Checks if the left mouse button is held down.
        if (TWAPI_IsMouseDown(window, TWAPI_MOUSE_LEFT))
            printf("Mouse: %d, %d       \r",
                TWAPI_GetMouseX(window),
                TWAPI_GetMouseY(window));

        // Checks if the escape key is pressed.
        if (TWAPI_IsKeyPressed(window, TWAPI_KEY_ESCAPE))
            break;

        // Calculates each frame.
        Sleep(16);
    }

    // Destroys the window we made.
    TWAPI_DestroyWindow(window);

    return 0;
}

```

### Audio Example

```c

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

```

### Framebuffer Example

```c

#include <stdio.h>
#include <windows.h>

#include <TWAPI/TWAPI.h>

int main(void)
{

    // Checks if TimWorks API couldn't initialize.
    if (!TWAPI_Init())
    {
        fprintf(stderr, "Failed to initialize the TimWorks API.\n");
        return -1;
    }

    // Creates the window.
    TWAPI_Window* window = TWAPI_CreateWindow(
        100, 100, 960, 600,
        "TimWorks API - Software Framebuffer"
    );

    // This checks if the window creation failed.
    if (!window) {
        printf("Failed to create window.\n");
        return 1;
    }

    // Creates the framebuffer
    TWAPI_Framebuffer* framebuffer =
        TWAPI_CreateFramebuffer(960, 600);

    // This checks if the framebuffer creation failed.
    if (!framebuffer) {
        printf("Failed to create framebuffer.\n");
        TWAPI_DestroyWindow(window);
        return 1;
    }

    // Main game loop.
    while (!TWAPI_WindowShouldClose(window)) 
    {
        // Handle Events.
        TWAPI_PollEvents();

        // This draws a color of the framebuffer in the     RRGGBB format.
        //                                              0x00101828
        TWAPI_Clear(framebuffer, 0x00101828);

        // Presents the framebuffer
        TWAPI_Present(window, framebuffer);

        // Since the window doesn't use OpenGL, we have to handle framing ourselves.
        Sleep(16);
        // But if it does, use "TWAPI_SwapOpenGLBuffers(window);".
    }

    // This destroys the framebuffer and window objects we made.
    TWAPI_DestroyFramebuffer(framebuffer);
    TWAPI_DestroyWindow(window);

    return 0;
}

```
