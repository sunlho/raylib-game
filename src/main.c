#include "constants.h"
#include "ecs/components/basic.h"
#include "ecs/components/physics.h"
#include "ecs/components/player.h"
#include "ecs/components/tile-animate.h"
#include "ecs/components/tile-collider.h"
#include "ecs/entities/tile-animate.h"
#include "flecs.h"
#include "raylib.h"
#include "tmx-loader.h"
#include "tmx.h"
#include "utils.h"
#include <stdio.h>

#define CONTEXT_IMPLEMENTATION
#include "context.h"
#define ECS_PLAYER_ENTITY_IMPLEMENTATION
#include "ecs/entities/player.h"
#define ECS_PHYSICS_WORLD_IMPLEMENTATION
#include "ecs/entities/physics-world.h"
#define ECS_COLLISION_SYSTEM_IMPLEMENTATION
#include "ecs/systems/movement.h"
#define ECS_PHYSICS_BASIC_IMPLEMENTATION
#include "ecs/systems/physics-basic.h"
#define ECS_TILE_ANIMATE_ENTITY_IMPLEMENTATION
#include "ecs/entities/tile-animate.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(PlayerData);
ECS_COMPONENT_DECLARE(PlayerSpawn);
ECS_COMPONENT_DECLARE(PlayerPhysics);
ECS_COMPONENT_DECLARE(TileCollider);
ECS_COMPONENT_DECLARE(PhysicsBody);
ECS_COMPONENT_DECLARE(PhysicsWorld);
ECS_COMPONENT_DECLARE(TileAnimation);

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "[raylib-tmx] example");
    SetTargetFPS(FPS);

    ecs_world_t *world = ecs_init();
    bool isDebug = false;

    ECS_COMPONENT_DEFINE(world, Position);
    ECS_COMPONENT_DEFINE(world, Velocity);
    ECS_COMPONENT_DEFINE(world, PlayerData);
    ECS_COMPONENT_DEFINE(world, PlayerSpawn);
    ECS_COMPONENT_DEFINE(world, PlayerPhysics);
    ECS_COMPONENT_DEFINE(world, TileCollider);
    ECS_COMPONENT_DEFINE(world, PhysicsBody);
    ECS_COMPONENT_DEFINE(world, PhysicsWorld);
    ECS_COMPONENT_DEFINE(world, TileAnimation);

    ecs_singleton_set(world, EcsRest, {0});
    SetWorld(world);

    cpSpace *space = InitPhysicsWorld(world);
    SetSpace(space);
    ECS_IMPORT(world, PhysicsBasic);

    ecs_entity_t player = CreatePlayerEntity(world);

    SetPlayerEntity(player);

    TMXRenderContext ctx = {NULL, 0, 0, WHITE, TILE_SCALE};
    LayerRenderTexture *mapTexture = InitMap(GetAssetPath("island.tmx"), &ctx);
    LayerRenderTexture *mapTextureHead = mapTexture;

    ECS_IMPORT(world, Movement);

    while (!WindowShouldClose() && !ecs_should_quit(world)) {
        ecs_progress(world, GetFrameTime());

        BeginDrawing();
        ClearBackground(BLACK);

        // while (mapTexture->next) {
        DrawRenderTextureFixed(mapTexture->texture, 0, 0, 1, WHITE);
        //     mapTexture = mapTexture->next;
        // }
        if (!mapTexture->next)
            mapTexture = mapTextureHead;
        DrawAnimatedTiles(&ctx);

        DrawPlayer(world);

        EndDrawing();
    }

    FreePlayerEntity(world);
    ecs_fini(world);
    CloseWindow();

    return 0;
}