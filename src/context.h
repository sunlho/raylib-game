#ifndef CONTEXT_H
#define CONTEXT_H

#include "box2d/box2d.h"
#include "flecs.h"

typedef struct {
    ecs_world_t *world;
    ecs_entity_t player;
    b2WorldId phyWorld;
    Camera2D *camera;
    Vector2 size;
} GameContext;

ecs_world_t *GetWorld();
ecs_entity_t GetPlayerEntity();
b2WorldId GetPhyWorld();
Camera2D *GetCamera();
Vector2 GetWorldSize();

void SetPlayerEntity(ecs_entity_t player);
void SetWorld(ecs_world_t *world);
void SetPhyWorld(b2WorldId worldId);
void SetCamera(Camera2D *camera);
void SetWorldSize(double width, double height);

#endif // CONTEXT_H

#ifdef CONTEXT_IMPLEMENTATION
#ifndef CONTEXT_IMPLEMENTATION_ONLY
#define CONTEXT_IMPLEMENTATION_ONLY

GameContext global_ctx = {0};

ecs_world_t *GetWorld() { return global_ctx.world; }
ecs_entity_t GetPlayerEntity() { return global_ctx.player; }
b2WorldId GetPhyWorld() { return global_ctx.phyWorld; }
Camera2D *GetCamera() { return global_ctx.camera; }
Vector2 GetWorldSize() { return global_ctx.size; }
void SetPlayerEntity(ecs_entity_t player) { global_ctx.player = player; }
void SetWorld(ecs_world_t *world) { global_ctx.world = world; }
void SetPhyWorld(b2WorldId worldId) { global_ctx.phyWorld = worldId; }
void SetCamera(Camera2D *camera) { global_ctx.camera = camera; }
void SetWorldSize(double width, double height) {
    global_ctx.size.x = width;
    global_ctx.size.y = height;
}

#endif // CONTEXT_IMPLEMENTATION_ONLY
#endif // CONTEXT_IMPLEMENTATION
