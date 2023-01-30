#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

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
    AnimData nebData{
        {0.0, 0.0, nebula.width / 8, nebula.height / 8},
        {viewWidth, viewHeight - nebData.rec.height},
        0,
        1.0 / 12.0,
        0};
    AnimData neb2Data{
        {0.0, 0.0, nebula.width / 8, nebula.height / 8},
        {viewWidth + 300, viewHeight - nebData.rec.height},
        0,
        1.0 / 16.0,
        0.0};
    Rectangle nebRec{0.0, 0.0, nebula.width / 8, nebula.height / 8};
    Vector2 nebPos{viewWidth, viewHeight - nebRec.height};
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

    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width / 6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = viewWidth / 2 - scarfyRec.width / 2;
    scarfyPos.y = viewHeight - scarfyRec.height;
    int chFrame{};
    float chUpdateTime{1.0 / 12.0};
    float chRunningTime{};
    int nebFrame{};
    const float nebUpdateTime{1.0 / 12.0};
    float nebRunningTime{};

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
        if (!isInAir)
            chRunningTime += dt;
        {
            if (chRunningTime >= chUpdateTime)
            {
                chRunningTime = 0.0;
                scarfyRec.x = chFrame * scarfyRec.width;
                chFrame++;
                if (chFrame > 5)
                {
                    chFrame = 0;
                }
            }
        }
        nebRunningTime += dt;
        if (nebRunningTime >= nebUpdateTime)
        {
            nebRunningTime = 0.0;
            nebRec.x = nebFrame * nebRec.width;
            nebFrame++;
            if (nebFrame > 7)
            {
                nebFrame = 0;
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