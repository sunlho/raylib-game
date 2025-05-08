#include <raylib.h>
#include <tmx.h>
#include <stdio.h>
#include <flecs.h>
#include "utils.h"
#include "context.h"
#include "constants.h"
#define RAYLIB_TMX_IMPLEMENTATION
#include "tmx-loader.h"
#include "ecs/systems/movement.h"
#include "ecs/components/basic.h"
#include "ecs/components/player.h"
#include "ecs/components/tile-collider.h"

GameContext global_ctx = {0};

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(PlayerTag);
ECS_COMPONENT_DECLARE(TileCollider);

int main(void)
{
    const int screenWidth = 925;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "[raylib-tmx] example");
    SetTargetFPS(60);

    ecs_world_t *world = ecs_init();

    ECS_COMPONENT_DEFINE(world, Position);
    ECS_COMPONENT_DEFINE(world, Velocity);
    ECS_COMPONENT_DEFINE(world, PlayerTag);
    ECS_COMPONENT_DEFINE(world, TileCollider);

    ecs_singleton_set(world, EcsRest, {0});
    ECS_IMPORT(world, Movement);
    global_ctx.world = world;

    ecs_entity_t player = ecs_entity(world, {.name = "Player"});
    global_ctx.player = player;

    ecs_add(world, player, Position);
    ecs_add(world, player, Velocity);
    ecs_add(world, player, PlayerTag);
    ecs_set(world, player, Velocity, {0.0f, 0.0f});

    ecs_entity_t group = ecs_entity(world, {.name = TILE_COLLIDER_GROUP});

    Texture2D playerTexture = LoadTexture(GetAssetPath("scarfy.png"));
    ecs_set(world, player, PlayerTag, {playerTexture});

    playerTexture.width /= 3;
    playerTexture.height /= 3;
    Rectangle frameRec = {0.0f, 0.0f, (float)playerTexture.width / 6, (float)playerTexture.height};
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 9;

    float moveSpeed = 3.5f;
    bool facingRight = true;

    RenderTexture2D mapTexture = InitMap(GetAssetPath("island.tmx"));

    while (!WindowShouldClose() && !ecs_should_quit(world))
    {
        ecs_progress(world, GetFrameTime());

        Vector2 move = GetMovementInput(moveSpeed);
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

            ecs_query_t *query = ecs_query(world, {.terms = {{.id = ecs_id(TileCollider)}}});
            ecs_iter_t it = ecs_query_iter(world, query);
            while (ecs_query_next(&it))
            {
                TileCollider *collider = ecs_field(&it, TileCollider, 0);
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
        DrawRenderTextureFixed(mapTexture, 0, 0, WHITE);
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
    UnloadTexture(playerTexture);
    CloseWindow();

    return 0;
}