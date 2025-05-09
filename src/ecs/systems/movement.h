#ifndef ECS_MOVEMENT_SYSTEM_H
#define ECS_MOVEMENT_SYSTEM_H

#include "../components/basic.h"
#include <flecs.h>

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);

static inline void MovementSystem(ecs_iter_t *it) {
    Position *p = ecs_field(it, Position, 0);
    Velocity *v = ecs_field(it, Velocity, 1);

    for (int i = 0; i < it->count; i++) {
        p[i].x += v[i].x;
        p[i].y += v[i].y;
    }
}

static inline void MovementImport(ecs_world_t *world) {
    ECS_MODULE(world, Movement);
    ECS_SYSTEM(world, MovementSystem, EcsOnUpdate, Position, Velocity);
}

#endif // ECS_MOVEMENT_SYSTEM_H