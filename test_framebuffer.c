
#include <stdio.h>
#include <windows.h>

#include <TWAPI/TW_window.h>
#include <TWAPI/TW_framebuffer.h>

int main(void)
{
    TWAPI_Window* window = TWAPI_CreateWindow(
        100, 100, 960, 600,
        "TimAPI - Software Framebuffer"
    );

    if (!window) {
        printf("Failed to create window.\n");
        return 1;
    }

    TWAPI_Framebuffer* framebuffer =
        TWAPI_CreateFramebuffer(640, 400);

    if (!framebuffer) {
        printf("Failed to create framebuffer.\n");
        TWAPI_DestroyWindow(window);
        return 1;
    }

    float playerX = 300.0f;
    float playerY = 180.0f;
    const float playerSize = 32.0f;
    const float moveSpeed = 220.0f;

    ULONGLONG previousTime = GetTickCount64();

    while (!TWAPI_WindowShouldClose(window)) {
        TWAPI_PollEvents();

        ULONGLONG currentTime = GetTickCount64();
        float deltaTime =
            (float)(currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        /* Avoid large jumps if execution pauses. */
        if (deltaTime > 0.05f)
            deltaTime = 0.05f;

        if (TWAPI_IsKeyPressed(window, TWAPI_KEY_ESCAPE))
            break;

        if (TWAPI_IsKeyDown(window, TWAPI_KEY_A))
            playerX -= moveSpeed * deltaTime;
        if (TWAPI_IsKeyDown(window, TWAPI_KEY_D))
            playerX += moveSpeed * deltaTime;
        if (TWAPI_IsKeyDown(window, TWAPI_KEY_W))
            playerY -= moveSpeed * deltaTime;
        if (TWAPI_IsKeyDown(window, TWAPI_KEY_S))
            playerY += moveSpeed * deltaTime;

        /* Keep the rectangle inside the framebuffer. */
        if (playerX < 0) playerX = 0;
        if (playerY < 0) playerY = 0;

        if (playerX > framebuffer->width - playerSize)
            playerX = (float)(framebuffer->width - playerSize);
        if (playerY > framebuffer->height - playerSize)
            playerY = (float)(framebuffer->height - playerSize);

        /* Background. */
        TWAPI_Clear(framebuffer, 0x00101828);

        /* Horizon and ground. */
        TWAPI_FillRect(
            framebuffer, 0, 0, 640, 200, 0x001B3554
        );
        TWAPI_FillRect(
            framebuffer, 0, 200, 640, 200, 0x002A4935
        );

        /* A simple grid on the ground. */
        for (int x = 0; x < 640; x += 32)
            TWAPI_FillRect(
                framebuffer, x, 200, 1, 200, 0x00415A43
            );

        for (int y = 216; y < 400; y += 24)
            TWAPI_FillRect(
                framebuffer, 0, y, 640, 1, 0x00415A43
            );

        /* A yellow frame and a red player rectangle. */
        TWAPI_FillRect(
            framebuffer, 20, 20, 600, 4, 0x00F0D060
        );
        TWAPI_FillRect(
            framebuffer, 20, 20, 4, 360, 0x00F0D060
        );
        TWAPI_FillRect(
            framebuffer, 616, 20, 4, 360, 0x00F0D060
        );
        TWAPI_FillRect(
            framebuffer, 20, 376, 600, 4, 0x00F0D060
        );

        TWAPI_FillRect(
            framebuffer,
            (int)playerX + 4, (int)playerY + 4,
            (int)playerSize, (int)playerSize,
            0x00000000
        );

        TWAPI_FillRect(
            framebuffer,
            (int)playerX, (int)playerY,
            (int)playerSize, (int)playerSize,
            0x00F04444
        );

        TWAPI_FillRect(
            framebuffer,
            (int)playerX + 6, (int)playerY + 6,
            8, 8, 0x00FFFFFF
        );

        TWAPI_Present(window, framebuffer);

        Sleep(1);
    }

    TWAPI_DestroyFramebuffer(framebuffer);
    TWAPI_DestroyWindow(window);

    printf("TimAPI framebuffer test ended.\n");
    return 0;
}
