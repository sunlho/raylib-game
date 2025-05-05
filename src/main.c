#include "utils.h"
#include "raylib.h"
#include "tmx.h"
#define RAYLIB_TMX_IMPLEMENTATION
#include "tmx-loader.h"
#include <stdio.h>
#include <flecs.h>

typedef struct
{
    float x, y;
} Position, Velocity;

void Move(ecs_iter_t *it)
{
    Position *p = ecs_field(it, Position, 0);
    Velocity *v = ecs_field(it, Velocity, 1);

    for (int i = 0; i < it->count; i++)
    {
        p[i].x += v[i].x;
        p[i].y += v[i].y;
    }
}

int main(void)
{
    const int screenWidth = 925;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "[raylib-tmx] example");

    tmx_map *map = LoadTMX(get_asset_path("island.tmx"));

    if (!map)
    {
        TraceLog(LOG_ERROR, "Failed to load map: %s", get_asset_path("island.tmx"));
        return 1;
    }

    Texture2D playerTexture = LoadTexture(get_asset_path("scarfy.png"));
    playerTexture.width /= 3;
    playerTexture.height /= 3;
    Rectangle frameRec = {0.0f, 0.0f, (float)playerTexture.width / 6, (float)playerTexture.height};
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 9;

    float moveSpeed = 3.5f;
    bool facingRight = true;

    SetTargetFPS(60);

    ecs_world_t *world = ecs_init();
    ECS_TAG(world, PlayerTag);
    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_SYSTEM(world, Move, EcsOnUpdate, Position, Velocity);

    ecs_entity_t player = ecs_new(world);
    ecs_set(world, player, Position, {350.0f, 150.0f});
    ecs_set(world, player, Velocity, {0.0f, 0.0f});
    ecs_add(world, player, PlayerTag);

    while (!WindowShouldClose() && ecs_progress(world, GetFrameTime()))
    {
        Vector2 move = get_movement_input(moveSpeed);
        ecs_set(world, player, Velocity, {move.x, move.y});
        bool isMoving = (move.x != 0.0f || move.y != 0.0f);

        if (move.x > 0)
            facingRight = true;
        else if (move.x < 0)
            facingRight = false;

        const Position *p = ecs_get(world, player, Position);
        float x = p->x;
        float y = p->y;
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;
        if (x + frameRec.width > screenWidth)
            x = screenWidth - frameRec.width;
        if (y + frameRec.height > screenHeight)
            y = screenHeight - frameRec.height;
        ecs_set(world, player, Position, {x, y});

        if (isMoving)
        {
            framesCounter++;
            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                currentFrame = (currentFrame + 1) % 6;
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
            DrawTextureRec(playerTexture, frameRec, (Vector2){p->x, p->y}, WHITE);
        }
        else
        {
            Rectangle flipped = frameRec;
            flipped.width = -frameRec.width;
            flipped.x += frameRec.width;
            DrawTextureRec(playerTexture, flipped, (Vector2){p->x, p->y}, WHITE);
        }

        EndDrawing();
    }

    ecs_fini(world);
    UnloadTMX(map);
    UnloadTexture(playerTexture);
    CloseWindow();

    return 0;
}