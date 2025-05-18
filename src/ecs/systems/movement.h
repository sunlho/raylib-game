#ifndef ECS_MOVEMENT_SYSTEM_H
#define ECS_MOVEMENT_SYSTEM_H

#include "../components/basic.h"
#include "box2d/box2d.h"
#include "flecs.h"

void MovementImport(ecs_world_t *world);

#endif // ECS_MOVEMENT_SYSTEM_H

#ifdef ECS_COLLISION_SYSTEM_IMPLEMENTATION
#ifndef ECS_COLLISION_SYSTEM_IMPLEMENTATION_ONLY
#define ECS_COLLISION_SYSTEM_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(PlayerData);
extern ECS_COMPONENT_DECLARE(PlayerPhysics);

float maxSpeed = 500.0f;

void MovementSystem(ecs_iter_t *it) {
    Velocity *v = ecs_field(it, Velocity, 0);
    PlayerPhysics *pp = ecs_field(it, PlayerPhysics, 2);

    for (int i = 0; i < it->count; i++) {
        b2Vec2 velocity = (b2Vec2){v[i].x, v[i].y};
        b2BodyId bodyId = pp[i].body;
        b2Body_SetLinearVelocity(bodyId, velocity);
    }
}

void MovementImport(ecs_world_t *world) {
    ECS_MODULE(world, Movement);
    ECS_SYSTEM(world, MovementSystem, EcsOnUpdate, Velocity, PlayerData, PlayerPhysics);
}

#endif // ECS_COLLISION_SYSTEM_IMPLEMENTATION_ONLY
#endif // ECS_COLLISION_SYSTEM_IMPLEMENTATION