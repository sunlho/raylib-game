#include "raylib.h"
#include "tmx.h"
#define RAYLIB_TMX_IMPLEMENTATION
#include "tmx-loader.h"
#include <math.h>
#include <stdio.h>

static const char* get_asset_path(const char* relative_path) {
    static char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s%s", ASSETS_PATH, relative_path);
    return full_path;
}

static Vector2 get_movement_input(float speed)
{
    static int lastHorizontalKey = 0;
    static int lastVerticalKey = 0;

    Vector2 dir = { 0.0f, 0.0f };

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) lastHorizontalKey = KEY_RIGHT;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) lastHorizontalKey = KEY_LEFT;

    bool rightDown = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    bool leftDown = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);

    if (rightDown && leftDown)
    {
        dir.x = (lastHorizontalKey == KEY_RIGHT) ? 1.0f : -1.0f;
    }
    else if (rightDown)
    {
        dir.x = 1.0f;
        lastHorizontalKey = KEY_RIGHT;
    }
    else if (leftDown)
    {
        dir.x = -1.0f;
        lastHorizontalKey = KEY_LEFT;
    }
    else
    {
        lastHorizontalKey = 0;
    }

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) lastVerticalKey = KEY_UP;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) lastVerticalKey = KEY_DOWN;

    bool upDown = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    bool downDown = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);

    if (upDown && downDown)
    {
        dir.y = (lastVerticalKey == KEY_UP) ? -1.0f : 1.0f;
    }
    else if (upDown)
    {
        dir.y = -1.0f;
        lastVerticalKey = KEY_UP;
    }
    else if (downDown)
    {
        dir.y = 1.0f;
        lastVerticalKey = KEY_DOWN;
    }
    else
    {
        lastVerticalKey = 0;
    }

    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (length > 0.0f)
    {
        dir.x = (dir.x / length) * speed;
        dir.y = (dir.y / length) * speed;
    }

    return dir;
}


int main(void)
{
    const int screenWidth = 925;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "[raylib-tmx] example");

    tmx_map* map = LoadTMX(get_asset_path("island.tmx"));

    if (!map) {
        tmx_perror("Cannot load map");
        return 1;
    }

    Texture2D scarfy = LoadTexture(get_asset_path("scarfy.png"));
	scarfy.width /= 3;
	scarfy.height /= 3;

    Vector2 position = { 350.0f, 150.0f };
    Rectangle frameRec = { 0.0f, 0.0f, (float)scarfy.width / 6, (float)scarfy.height };
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 9;

    float moveSpeed = 3.5f;
    bool facingRight = true;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 move = get_movement_input(moveSpeed);

        bool isMoving = (move.x != 0.0f || move.y != 0.0f);

        if (move.x > 0) facingRight = true;
        else if (move.x < 0) facingRight = false;

        position.x += move.x;
        position.y += move.y;

        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x + frameRec.width > screenWidth)  position.x = screenWidth - frameRec.width;
        if (position.y + frameRec.height > screenHeight) position.y = screenHeight - frameRec.height;

        if (isMoving)
        {
            framesCounter++;
            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;

                if (currentFrame > 5) currentFrame = 0;

                frameRec.x = (float)currentFrame * frameRec.width;
            }
        }
        else
        {
            currentFrame = 0;
            framesCounter = 0;
            frameRec.x = 0;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTMX(map, 0, 0, WHITE);

        if (facingRight)
        {
            DrawTextureRec(scarfy, frameRec, position, WHITE);
        }
        else
        {
            Rectangle flipped = frameRec;
            flipped.width = -frameRec.width;
            flipped.x += frameRec.width;
            DrawTextureRec(scarfy, flipped, position, WHITE);
        }


        EndDrawing();
    }

    UnloadTMX(map);
    UnloadTexture(scarfy);
    CloseWindow();

    return 0;
}
