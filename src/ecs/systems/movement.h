#ifndef ECS_MOVEMENT_SYSTEM_H
#define ECS_MOVEMENT_SYSTEM_H

#include "box2d/box2d.h"
#include "flecs.h"

#include "../components/basic.h"

#include "tilemap/viewport.h"

void MovementImport(ecs_world_t *world);

#endif // ECS_MOVEMENT_SYSTEM_H

#ifdef ECS_MOVEMENT_SYSTEM_IMPLEMENTATION
#ifndef ECS_MOVEMENT_SYSTEM_IMPLEMENTATION_ONLY
#define ECS_MOVEMENT_SYSTEM_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(PlayerPhysics);

void MovementSystem(ecs_iter_t *it) {
    Velocity *v = ecs_field(it, Velocity, 0);
    PlayerPhysics *pp = ecs_field(it, PlayerPhysics, 1);

    for (int i = 0; i < it->count; i++) {
        b2Body_SetLinearVelocity(pp[i].body, (b2Vec2){v[i].x, v[i].y});
    }
}

void MovementImport(ecs_world_t *world) {
    ECS_MODULE(world, Movement);
    ECS_SYSTEM(world, MovementSystem, EcsOnUpdate, Velocity, PlayerPhysics);
}

#endif // ECS_MOVEMENT_SYSTEM_IMPLEMENTATION_ONLY
#endif // ECS_MOVEMENT_SYSTEM_IMPLEMENTATION
