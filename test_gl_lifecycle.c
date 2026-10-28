#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#include <glad/gl10.h>
#include "TW_window.h"

static GLADapiproc gladGetProcAddress(const char* name)
{
    return (GLADapiproc)TWAPI_GetOpenGLProcAddress(name);
}

int main(void)
{
    const int testCount = 1000;

    TWAPI_OpenGLContextConfig config = {
        1, 1,
        TWAPI_OPENGL_PROFILE_DEFAULT,
        0
    };

    printf("TimAPI OpenGL lifecycle test\n");
    printf("Iterations: %d\n\n", testCount);

    for (int i = 0; i < testCount; ++i)
    {
        printf("[%d/%d] Creating window...\n",
               i + 1, testCount);

        TWAPI_Window* window = TWAPI_CreateWindow(
            100, 100, 640, 400,
            "TimAPI Lifecycle Test"
        );

        if (!window)
        {
            fprintf(stderr, "FAIL: Window creation failed.\n");
            return 1;
        }

        if (!TWAPI_CreateOpenGLContextEx(window, &config))
        {
            fprintf(stderr,
                    "FAIL: OpenGL context creation failed.\n");

            TWAPI_DestroyWindow(window);
            return 1;
        }

        if (wglGetCurrentContext() == NULL)
        {
            fprintf(stderr,
                    "FAIL: No current OpenGL context.\n");

            TWAPI_DestroyOpenGLContext(window);
            TWAPI_DestroyWindow(window);
            return 1;
        }

        if (!TWAPI_MakeOpenGLCurrent(window))
        {
            fprintf(stderr,
                    "FAIL: Could not make context current.\n");

            TWAPI_DestroyOpenGLContext(window);
            TWAPI_DestroyWindow(window);
            return 1;
        }

        if (!gladLoadGL(gladGetProcAddress))
        {
            fprintf(stderr,
                    "FAIL: GLAD could not load OpenGL.\n");

            TWAPI_DestroyOpenGLContext(window);
            TWAPI_DestroyWindow(window);
            return 1;
        }

        glViewport(0, 0, 640, 400);
        glClearColor(0.05f, 0.15f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        TWAPI_SwapOpenGLBuffers(window);

        TWAPI_DestroyOpenGLContext(window);

        if (wglGetCurrentContext() != NULL)
        {
            fprintf(stderr,
                    "FAIL: Context is still current after "
                    "TWAPI_DestroyOpenGLContext().\n");

            /*
             * Avoid destroying the window while a context
             * might still be current.
             */
            wglMakeCurrent(NULL, NULL);
            TWAPI_DestroyWindow(window);
            return 1;
        }

        TWAPI_DestroyWindow(window);

        printf("[%d/%d] PASS\n", i + 1, testCount);
    }

    printf("\nAll %d lifecycle tests passed!\n", testCount);
    return 0;
}