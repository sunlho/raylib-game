#ifndef CONTEXT_H
#define CONTEXT_H

#include <flecs.h>

typedef struct {
    ecs_world_t *world;
    ecs_entity_t player;
} GameContext;

ecs_world_t *GetWorld();
ecs_entity_t GetPlayerEntity();
void SetPlayerEntity(ecs_entity_t player);
void SetWorld(ecs_world_t *world);

#endif // CONTEXT_H

#ifdef CONTEXT_IMPLEMENTATION
#ifndef CONTEXT_IMPLEMENTATION_ONLY
#define CONTEXT_IMPLEMENTATION_ONLY

GameContext global_ctx = {0};

ecs_world_t *GetWorld() { return global_ctx.world; }
ecs_entity_t GetPlayerEntity() { return global_ctx.player; }
void SetPlayerEntity(ecs_entity_t player) { global_ctx.player = player; }
void SetWorld(ecs_world_t *world) { global_ctx.world = world; }

#endif // CONTEXT_IMPLEMENTATION_ONLY
#endif // CONTEXT_IMPLEMENTATION
