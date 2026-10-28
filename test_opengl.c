
#include <stdio.h>
#include <stdlib.h>

#include "TW_window.h"
#include <glad/gl10.h>

/*
    TimAPI OpenGL 1.1 Resize Test

    Tests:
    - Window creation
    - OpenGL 1.1 context creation
    - GLAD loading
    - Dynamic window dimensions
    - Dynamic OpenGL viewport
    - Window closing and cleanup
*/

static GLADapiproc gladGetProcAddress(const char* name)
{
    return (GLADapiproc)TWAPI_GetOpenGLProcAddress(name);
}

int main(void)
{
    printf("====================================\n");
    printf(" TimAPI OpenGL Resize Test\n");
    printf("====================================\n");

    /* Create the window. */
    // TWAPI_Window* window = TWAPI_CreateWindow(
    //     100,
    //     100,
    //     960,
    //     600,
    //     "TimAPI - OpenGL Resize Test"
    // );

    TWAPI_WindowConfig windowConfig = {
        0,      /* x */
        0,      /* y */
        800,    /* width, ignored for fullscreen */
        600,    /* height, ignored for fullscreen */
        "TimAPI OpenGL Test",
        0,      /* resizable */
        0       /* fullscreen */
    };

    TWAPI_Window* window = TWAPI_CreateWindowEx(&windowConfig);

    if (!window)
    {
        printf("Failed to create window!\n");
        return 1;
    }

    TWAPI_SetVSync(window, 1);

    printf("[TimAPI] Window created.\n");

    /* Request a legacy OpenGL 1.1 context. */
    TWAPI_OpenGLContextConfig config;

    config.majorVersion = 1;
    config.minorVersion = 0;
    config.profile = TWAPI_OPENGL_PROFILE_DEFAULT;
    config.debug = 0;
    config.samples = 4;

    if (!TWAPI_CreateOpenGLContextEx(window, &config))
    {
        fprintf(stderr, "ERROR: Could not create OpenGL context.\n");
        TWAPI_DestroyWindow(window);
        return EXIT_FAILURE;
    }

    printf("[OpenGL] Context created.\n");

    /* Load the GLAD OpenGL functions. */
    if (!gladLoadGL(gladGetProcAddress))
    {
        fprintf(stderr, "ERROR: GLAD failed to load OpenGL.\n");
        TWAPI_DestroyOpenGLContext(window);
        TWAPI_DestroyWindow(window);
        return EXIT_FAILURE;
    }

    printf("[OpenGL] GLAD loaded successfully.\n");
    printf("Resize the window to test the viewport.\n");
    printf("Close the window to exit.\n\n");

    int lastWidth = -1;
    int lastHeight = -1;

    /*
        Main loop.

        Poll events first so WM_SIZE can update the
        dimensions before we query them.
    */

    static int previousWidth = -1;
    static int previousHeight = -1;

    int width = 0;
    int height = 0;

    static float i = 0.0f;
    static float direction = 1.0f;
    const float speed = 0.01f;
    
    while (!TWAPI_WindowShouldClose(window))
    {
        TWAPI_PollEvents();


        if (TWAPI_IsKeyReleased(window, TWAPI_KEY_ESCAPE)) 
        {
            if (TWAPI_IsFullscreen(window) == 0)
            {
                TWAPI_SetFullscreen(window, 1);
            }
            else
            {
                TWAPI_SetFullscreen(window, 0);
            }
            
        }

        TWAPI_GetWindowSize(window, &width, &height);

        if (width != previousWidth || height != previousHeight)
        {
            printf("Resize: %d x %d\n", width, height);

            previousWidth = width;
            previousHeight = height;
        }

        int width = 0;
        int height = 0;

        TWAPI_GetWindowSize(window, &width, &height);

        /* Report size changes. */
        if (width != lastWidth || height != lastHeight)
        {
            printf("[Resize] Client area: %d x %d\n",
                width, height);

            lastWidth = width;
            lastHeight = height;
        }

        /*
            Avoid a zero-sized viewport when minimized.
            OpenGL viewport dimensions must not be negative.
        */
        if (width <= 0 || height <= 0)
            continue;

        /* Match the OpenGL viewport to the client area. */
        glViewport(0, 0, width, height);

        /* Clear the window. */
        glClearColor(0.08f, 0.10f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*
            Use an aspect-correct orthographic projection.

            The visible vertical range stays at -1 to 1.
            The horizontal range adapts to the window aspect
            ratio so the triangle does not stretch sideways.
        */
        float aspect = (float)width / (float)height;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(
            -aspect, aspect,
            -1.0, 1.0,
            -1.0, 1.0
        );

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        i += speed * direction;

        if (i >= 0.75f)
        {
            i = 0.75f;
            direction = -1.0f;
        }
        else if (i <= -0.75f)
        {
            i = -0.75f;
            direction = 1.0f;
        }

        /* Draw a colored triangle using OpenGL 1.0. */
        glBegin(GL_TRIANGLES);

            glColor3f(1.0f, 0.2f, 0.2f);
            glVertex2f(-i, 0.75f);

            glColor3f(0.2f, 1.0f, 0.3f);
            glVertex2f(-0.75f, i);

            glColor3f(0.2f, 0.5f, 1.0f);
            glVertex2f(i, -0.65f);

        glEnd();

        /* Present the rendered frame. */
        if (!TWAPI_SwapOpenGLBuffers(window))
        {
            fprintf(stderr,
                "WARNING: OpenGL buffer swap failed.\n");
            break;
        }
    }

    printf("\n[TimAPI] Shutting down...\n");

    TWAPI_DestroyOpenGLContext(window);
    TWAPI_DestroyWindow(window);

    printf("[TimAPI] Cleanup complete.\n");

    return EXIT_SUCCESS;
}