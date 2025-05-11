#ifndef CONTEXT_H
#define CONTEXT_H

#include "chipmunk/chipmunk.h"
#include "flecs.h"

typedef struct {
    ecs_world_t *world;
    ecs_entity_t player;
    cpSpace *space;
} GameContext;

ecs_world_t *GetWorld();
ecs_entity_t GetPlayerEntity();
cpSpace *GetSpace();

void SetPlayerEntity(ecs_entity_t player);
void SetWorld(ecs_world_t *world);
void SetSpace(cpSpace *space);

#endif // CONTEXT_H

#ifdef CONTEXT_IMPLEMENTATION
#ifndef CONTEXT_IMPLEMENTATION_ONLY
#define CONTEXT_IMPLEMENTATION_ONLY

GameContext global_ctx = {0};

ecs_world_t *GetWorld() { return global_ctx.world; }
ecs_entity_t GetPlayerEntity() { return global_ctx.player; }
cpSpace *GetSpace() { return global_ctx.space; }
void SetPlayerEntity(ecs_entity_t player) { global_ctx.player = player; }
void SetWorld(ecs_world_t *world) { global_ctx.world = world; }
void SetSpace(cpSpace *space) { global_ctx.space = space; }

#endif // CONTEXT_IMPLEMENTATION_ONLY
#endif // CONTEXT_IMPLEMENTATION
