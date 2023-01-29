#include "raylib.h"

int main()
{
    const char *title = "My Game";
    const int viewHeight{720};
    const int viewWidth{1280};
    InitWindow(viewWidth, viewHeight, title);
    int velocity{0};
    const int gravity{1'000};
    const int jumpVel{-600};
    bool isInAir{false};
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png"); // 8x8
    Rectangle nebRec{0.0, 0.0, nebula.width / 8, nebula.height / 8};
    Vector2 nebPos{viewWidth, viewHeight - nebRec.height};
    int nebVelocity{-600};
    Texture2D scarfy = LoadTexture("textures/scarfy.png"); // 1x6
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width / 6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = viewWidth / 2 - scarfyRec.width / 2;
    scarfyPos.y = viewHeight - scarfyRec.height;
    int frame{};
    float updateTime{1.0 / 12.0};
    float runningTime{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        const float dt{GetFrameTime()};
        BeginDrawing();
        ClearBackground(WHITE);

        if (scarfyPos.y >= (viewHeight - scarfyRec.height))
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
            velocity += gravity * dt;
            isInAir = true;
        }
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }
        nebPos.x += nebVelocity * dt;
        scarfyPos.y += velocity * dt;
        runningTime += dt;
        if (runningTime >= updateTime)
        {
            runningTime = 0.0;
            scarfyRec.x = frame * scarfyRec.width;
            frame++;
            if (frame > 5)
            {
                frame = 0;
            }
        }
        DrawTextureRec(nebula, nebRec, nebPos, WHITE);
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}