#ifndef CONTEXT_H
#define CONTEXT_H

#include "box2d/box2d.h"
#include "ecs/components/player.h"
#include "flecs.h"

typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    ecs_world_t *world;
    ecs_entity_t player;
    b2WorldId phy_world;
    Camera2D *camera;
    Size size;
} GameContext;

ecs_world_t *GetWorld();
ecs_entity_t GetPlayerEntity();
b2WorldId GetPhyWorld();
Camera2D *GetCamera();
Size GetWorldSize();
void SetPlayerEntity(ecs_entity_t player);
void SetWorld(ecs_world_t *world);
void SetPhyWorld(b2WorldId world_id);
void SetCamera(Camera2D *camera);
void SetWorldSize(int width, int height);

#endif // CONTEXT_H

#ifdef CONTEXT_IMPLEMENTATION
#ifndef CONTEXT_IMPLEMENTATION_ONLY
#define CONTEXT_IMPLEMENTATION_ONLY

GameContext global_ctx = {0};

ecs_world_t *GetWorld() { return global_ctx.world; }
ecs_entity_t GetPlayerEntity() { return global_ctx.player; }
b2WorldId GetPhyWorld() { return global_ctx.phy_world; }
Camera2D *GetCamera() { return global_ctx.camera; }
Size GetWorldSize() { return global_ctx.size; }

void SetPlayerEntity(ecs_entity_t player) { global_ctx.player = player; }
void SetWorld(ecs_world_t *world) { global_ctx.world = world; }
void SetPhyWorld(b2WorldId world_id) { global_ctx.phy_world = world_id; }
void SetCamera(Camera2D *camera) { global_ctx.camera = camera; }
void SetWorldSize(int width, int height) {
    global_ctx.size.width = width;
    global_ctx.size.height = height;
}

#endif // CONTEXT_IMPLEMENTATION_ONLY
#endif // CONTEXT_IMPLEMENTATION
