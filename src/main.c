#include "constants.h"
#include "ecs/components/basic.h"
#include "ecs/components/player.h"
#include "ecs/components/tile-collider.h"
#include "ecs/systems/movement.h"
#include "tmx-loader.h"
#include "utils.h"
#include <flecs.h>
#include <raylib.h>
#include <stdio.h>
#include <tmx.h>
#define CONTEXT_IMPLEMENTATION
#include "context.h"
#define ECS_PLAYER_ENTITY_IMPLEMENTATION
#include "ecs/entities/player.h"

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(PlayerTag);
ECS_COMPONENT_DECLARE(TileCollider);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "[raylib-tmx] example");
    SetTargetFPS(60);

    ecs_world_t *world = ecs_init();

    ECS_COMPONENT_DEFINE(world, Position);
    ECS_COMPONENT_DEFINE(world, Velocity);
    ECS_COMPONENT_DEFINE(world, PlayerTag);
    ECS_COMPONENT_DEFINE(world, TileCollider);

    ecs_singleton_set(world, EcsRest, {0});
    ECS_IMPORT(world, Movement);
    SetWorld(world);

    ecs_entity_t player = CreatePlayerEntity(world);
    SetPlayerEntity(player);

    RenderTexture2D mapTexture = InitMap(GetAssetPath("island.tmx"));

    while (!WindowShouldClose() && !ecs_should_quit(world)) {
        ecs_progress(world, GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRenderTextureFixed(mapTexture, 0, 0, WHITE);
        DrawPlayer(world);
        EndDrawing();
    }

    FreePlayerEntity(world);
    ecs_fini(world);
    CloseWindow();

    return 0;
}