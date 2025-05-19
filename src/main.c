#include "box2d/box2d.h"
#include "constants.h"
#include "ecs/components/basic.h"
#include "ecs/components/player.h"
#include "ecs/components/tile-animate.h"
#include "flecs.h"
#include "raylib.h"
#include "tmx.h"
#include "utils.h"
#include <stdio.h>

#define CONTEXT_IMPLEMENTATION
#include "context.h"
#define TMX_LOADER_IMPLEMENTATION
#include "tmx-loader.h"
#define ECS_COLLISION_ENTITY_IMPLEMENTATION
#include "ecs/entities/collision.h"
#define ECS_PLAYER_ENTITY_IMPLEMENTATION
#include "ecs/entities/player.h"
#define ECS_PHYSICS_WORLD_IMPLEMENTATION
#include "ecs/entities/physics-world.h"
#define ECS_TILE_ANIMATE_ENTITY_IMPLEMENTATION
#include "ecs/entities/tile-animate.h"
#define ECS_COLLISION_SYSTEM_IMPLEMENTATION
#include "ecs/systems/movement.h"
#define ECS_PHYSICS_BASIC_IMPLEMENTATION
#include "ecs/systems/physics-basic.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(PlayerData);
ECS_COMPONENT_DECLARE(PlayerSpawn);
ECS_COMPONENT_DECLARE(PlayerPhysics);
ECS_COMPONENT_DECLARE(TileAnimation);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "[raylib-tmx] example");
    SetTargetFPS(FPS);

    Camera2D camera = {0};
    camera.zoom = 2.0f;
    camera.offset = (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SetCamera(&camera);

    ecs_world_t *world = ecs_init();
    SetWorld(world);
    ECS_COMPONENT_DEFINE(world, Position);
    ECS_COMPONENT_DEFINE(world, Velocity);
    ECS_COMPONENT_DEFINE(world, PlayerData);
    ECS_COMPONENT_DEFINE(world, PlayerSpawn);
    ECS_COMPONENT_DEFINE(world, PlayerPhysics);
    ECS_COMPONENT_DEFINE(world, TileAnimation);

    ecs_singleton_set(world, EcsRest, {0});

    b2WorldId phyWorld = InitPhysicsWorld(world);
    SetPhyWorld(phyWorld);

    ECS_IMPORT(world, PhysicsBasic);

    ecs_entity_t player = CreatePlayerEntity(world);
    SetPlayerEntity(player);

    TMXRenderContext ctx = {NULL, 0, 0, WHITE};
    LayerRenderTexture *mapTexture = InitMap(GetAssetPath("island.tmx"), &ctx);
    LayerRenderTexture *mapTextureHead = mapTexture;

    ECS_IMPORT(world, Movement);

    while (!WindowShouldClose() && !ecs_should_quit(world)) {

        ecs_progress(world, GetFrameTime());
        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(BLACK);

        DrawRenderTextureFixed(mapTexture->texture, 0, 0, 1, WHITE);
        if (!mapTexture->next)
            mapTexture = mapTextureHead;
        DrawAnimatedTiles(&ctx);
        DrawRenderTextureFixed(mapTexture->next->texture, 0, 0, 1, WHITE);
        DrawPlayer(world);
        DrawRenderTextureFixed(mapTexture->next->next->texture, 0, 0, 1, WHITE);
        DrawRenderTextureFixed(mapTexture->next->next->next->texture, 0, 0, 1, WHITE);
        DrawRenderTextureFixed(mapTexture->next->next->next->next->texture, 0, 0, 1, WHITE);

        EndMode2D();

        EndDrawing();
    }

    b2DestroyWorld(phyWorld);
    FreePlayerEntity(world);
    ecs_fini(world);
    CloseWindow();

    return 0;
}