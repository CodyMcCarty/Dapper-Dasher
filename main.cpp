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
    Texture2D backgroud = LoadTexture("textures/far-buildings.png");
    float backgroundScale = viewHeight / 180;
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float mgx{};
    float fgx{};
    float finishLine{nebs[sizeOfNebs - 1].pos.x};
    bool collision{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        const float dt{GetFrameTime()};
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dt;
        if (bgX <= -backgroud.width * backgroundScale)
        {
            bgX = 0.0;
        }
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(backgroud, bg1Pos, 0.0, backgroundScale, WHITE);
        Vector2 bg2Pos{bgX + backgroud.width * backgroundScale, 0.0};
        DrawTextureEx(backgroud, bg2Pos, 0.0, backgroundScale, WHITE);
        Vector2 bg3Pos{bgX + backgroud.width * (backgroundScale * 2), 0.0};
        DrawTextureEx(backgroud, bg3Pos, 0.0, backgroundScale, WHITE);
        mgx -= 40 * dt;
        if (mgx <= -midground.width * backgroundScale)
        {
            mgx = 0.0;
        }
        Vector2 mg1pos{mgx, 0.0};
        DrawTextureEx(midground, mg1pos, 0.0, backgroundScale, WHITE);
        Vector2 mg2pos{mgx + midground.width * backgroundScale, 0.0};
        DrawTextureEx(midground, mg2pos, 0.0, backgroundScale, WHITE);
        Vector2 mg3pos{mgx + midground.width * (backgroundScale * 2), 0.0};
        DrawTextureEx(midground, mg3pos, 0.0, backgroundScale, WHITE);
        fgx -= 80 * dt;
        if (fgx <= -foreground.width * backgroundScale)
        {
            fgx = 0.0;
        }
        Vector2 fg1pos{fgx, 0.0};
        DrawTextureEx(foreground, fg1pos, 0.0, backgroundScale, WHITE);
        Vector2 fg2pos{fgx + foreground.width * backgroundScale, 0.0};
        DrawTextureEx(foreground, fg2pos, 0.0, backgroundScale, WHITE);
        Vector2 fg3pos{fgx + foreground.width * (backgroundScale * 2), 0.0};
        DrawTextureEx(foreground, fg3pos, 0.0, backgroundScale, WHITE);

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
        if (!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dt, 5);
        }
        for (int i = 0; i < sizeOfNebs; i++)
        {
            nebs[i].pos.x += nebVelocity * dt;
        }
        for (int i = 0; i < sizeOfNebs; i++)
        {
            nebs[i] = updateAnimData(nebs[i], dt, 7);
        }

        for (AnimData neb : nebs)
        {
            float padding{50};
            Rectangle nebRec{neb.pos.x + padding, neb.pos.y + padding / 2, neb.rec.width - padding * 2, neb.rec.height};
            Rectangle scarfyRec{scarfyData.pos.x, scarfyData.pos.y, scarfyData.rec.width, scarfyData.rec.height};
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true; // TODO: becomes true godmode
            }
        }

        if (collision)
        {
            DrawText("Game Over!", viewWidth / 2, viewHeight / 2, 20, RED);
        }
        else
        {
            for (int i = 0; i < sizeOfNebs; i++)
            {
                DrawTextureRec(nebula, nebs[i].rec, nebs[i].pos, WHITE);
            }
            finishLine += nebVelocity * dt;
            scarfyData.pos.y += velocity * dt;
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        if (scarfyData.pos.x > nebs[sizeOfNebs - 1].pos.x + 400)
        {
            DrawText("You Win!", viewWidth / 2, viewHeight / 2, 20, RED);
        }

        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(backgroud);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}