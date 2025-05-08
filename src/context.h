#ifndef CONTEXT_H
#define CONTEXT_H

#include <flecs.h>

typedef struct {
    ecs_world_t *world;
    ecs_entity_t player;
} GameContext;

extern GameContext global_ctx;

#endif // CONTEXT_H