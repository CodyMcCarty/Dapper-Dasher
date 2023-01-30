#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= (windowHeight - data.rec.height);
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

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
    const int sizeOfNebs{6};
    AnimData nebs[sizeOfNebs]{};
    for (int i = 0; i < sizeOfNebs; i++)
    {
        nebs[i].rec.x = 0.0;
        nebs[i].rec.y = 0.0;
        nebs[i].rec.width = nebula.width / 8;
        nebs[i].rec.height = nebula.height / 8;
        nebs[i].pos.x = viewWidth + i * 300;
        nebs[i].pos.y = viewHeight - nebs[i].rec.height;
        nebs[i].frame = 0;
        nebs[i].updateTime = 1.0 / 12.0;
        nebs[i].runningTime = 0.0;
    }
    nebs[1].updateTime = 1.0 / 16.0;
    int nebVelocity{-200};
    Texture2D scarfy = LoadTexture("textures/scarfy.png"); // 1x6
    AnimData scarfyData;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.pos.x = viewWidth / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = viewHeight - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0 / 12.0;
    scarfyData.runningTime = 0.0;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        const float dt{GetFrameTime()};
        BeginDrawing();
        ClearBackground(WHITE);

        if (isOnGround(scarfyData, viewHeight))
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
        scarfyData.pos.y += velocity * dt;
        if (!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dt, 5);
        }
        // scarfyData.runningTime += dt;
        // {
        // if (scarfyData.runningTime >= scarfyData.updateTime)
        // {
        //     scarfyData.runningTime = 0.0;
        //     scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
        //     scarfyData.frame++;
        //     if (scarfyData.frame > 5)
        //     {
        //         scarfyData.frame = 0;
        //     }
        // }
        // }
        for (int i = 0; i < sizeOfNebs; i++)
        {
            nebs[i].pos.x += nebVelocity * dt;
        }
        for (int i = 0; i < sizeOfNebs; i++)
        {
            nebs[i] = updateAnimData(nebs[i], dt, 8);
            // nebs[i].runningTime += dt;
            // if (nebs[i].runningTime >= nebs[i].updateTime)
            // {
            //     nebs[i].runningTime = 0.0;
            //     nebs[i].rec.x = nebs[i].frame * nebs[i].rec.width;
            //     nebs[i].frame++;
            //     if (nebs[i].frame > 7)
            //     {
            //         nebs[i].frame = 0;
            //     }
            // }
        }
        for (int i = 0; i < sizeOfNebs; i++)
        {
            DrawTextureRec(nebula, nebs[i].rec, nebs[i].pos, WHITE);
        }
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}