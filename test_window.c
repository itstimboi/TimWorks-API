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

    // This checks if the window creation failed.
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

        #include <GL/gl.h>

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
