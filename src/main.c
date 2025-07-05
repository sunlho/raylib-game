#include "box2d/box2d.h"
#include "flecs.h"
#include "raylib.h"
#include "tmx.h"
#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "constants.h"
#include "ecs/components/basic.h"
#include "ecs/components/player.h"
#include "ecs/components/tile-anim.h"
#include "utils.h"

#define TILEMAP_IMPLEMENTATION
#include "tilemap/tilemap.h"
#define CONTEXT_IMPLEMENTATION
#include "context.h"
#define ECS_PLAYER_ENTITY_IMPLEMENTATION
#include "ecs/entities/player.h"
#define ECS_PHYSICS_WORLD_IMPLEMENTATION
#include "ecs/entities/physics-world.h"
#define ECS_MOVEMENT_SYSTEM_IMPLEMENTATION
#include "ecs/systems/movement.h"
#define ECS_PHYSICS_BASIC_IMPLEMENTATION
#include "ecs/systems/physics-basic.h"

ECS_COMPONENT_DECLARE(Position);
ECS_COMPONENT_DECLARE(Velocity);
ECS_COMPONENT_DECLARE(PlayerSpawn);
ECS_COMPONENT_DECLARE(PlayerPhysics);
ECS_COMPONENT_DECLARE(TileAnim);
ECS_COMPONENT_DECLARE(PlayerAnimState);
ECS_COMPONENT_DECLARE(PlayerAnimController);

int main(void) {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "[raylib-tmx] example");

    bool debug_mode = false;

    Camera2D camera = {0};
    camera.zoom = 2.0f;
    camera.offset = (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SetCamera(&camera);

    ecs_world_t *world = ecs_init();
    SetWorld(world);

    ECS_COMPONENT_DEFINE(world, Position);
    ECS_COMPONENT_DEFINE(world, Velocity);
    ECS_COMPONENT_DEFINE(world, PlayerSpawn);
    ECS_COMPONENT_DEFINE(world, PlayerPhysics);
    ECS_COMPONENT_DEFINE(world, TileAnim);
    ECS_COMPONENT_DEFINE(world, PlayerAnimState);
    ECS_COMPONENT_DEFINE(world, PlayerAnimController);

    ecs_singleton_set(world, EcsRest, {0});
    ECS_IMPORT(world, FlecsStats);

    b2WorldId phy_world = InitPhysicsWorld(world);
    SetPhyWorld(phy_world);

    TilemapInstance tilemap = Tilemap(
        GetAssetPath("Map.tmx"),
        {
            .world = world,
            .physics_world = phy_world,
            .collision_type = "Collision",
            .debug_mode = &debug_mode,
            .camera = &camera,
        });
    SetWorldSize(tilemap.size.width, tilemap.size.height);

    ECS_IMPORT(world, PhysicsBasic);

    b2DebugDraw debug_draw = GetDefaultDebugDraw();

    LoadPlayerAnimClips();
    ecs_entity_t player = CreatePlayerEntity(world);
    SetPlayerEntity(player);

    b2BodyId player_body = InitPlayerSpawn(world, (Vector2){1000, 1500});

    TilemapRegisterRenderSystem();

    while (!WindowShouldClose() && !ecs_should_quit(world)) {
        if (IsKeyPressed(KEY_R)) {
            ResetPlayerPosition(world);
        }
        if (IsKeyPressed(KEY_F3)) {
            debug_mode = !debug_mode;
        }

        PlayerAnimController *ctrl = ecs_get_mut(world, player, PlayerAnimController);
        PlayerAnimState *state = ecs_get_mut(world, player, PlayerAnimState);
        if (!ctrl->locked) {
            if (IsKeyPressed(KEY_SPACE)) {
                state->current_frame = 0;
                ctrl->prev_state = ctrl->state;
                ctrl->state = STATE_SLASH;
                ctrl->locked = true;
            } else if (IsKeyDown(KEY_D)) {
                ctrl->state = STATE_WALK;
                ctrl->direction = RIGHT;
            } else if (IsKeyDown(KEY_A)) {
                ctrl->state = STATE_WALK;
                ctrl->direction = LEFT;
            } else if (IsKeyDown(KEY_W)) {
                ctrl->state = STATE_WALK;
                ctrl->direction = UP;
            } else if (IsKeyDown(KEY_S)) {
                ctrl->state = STATE_WALK;
                ctrl->direction = DOWN;
            } else {
                ctrl->state = STATE_IDLE;
            }
        }
        Vector2 move = GetMovementInput(PLAYER_MOVE_SPEED);
        b2Body_SetLinearVelocity(player_body, (b2Vec2){move.x, move.y});
        ecs_progress(world, GetFrameTime());

        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(BLANK);

        tilemap.render();

        if (debug_mode) {
            b2World_Draw(phy_world, &debug_draw);
        }

        EndMode2D();

        DrawFPS(10, SCREEN_HEIGHT - 20);
        EndDrawing();
    }

    b2DestroyWorld(phy_world);
    FreePlayerEntity(world);
    ecs_fini(world);
    CloseWindow();

    return 0;
}
