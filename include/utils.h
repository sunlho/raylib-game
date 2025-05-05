#include "raylib.h"
#include <math.h>

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