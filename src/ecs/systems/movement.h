#ifndef ECS_MOVEMENT_SYSTEM_H
#define ECS_MOVEMENT_SYSTEM_H

#include "../components/basic.h"
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

bool CheckCollision(Rectangle a, Rectangle b) {
    return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}

float maxSpeed = 500.0f;

void MovementSystem(ecs_iter_t *it) {
    Velocity *v = ecs_field(it, Velocity, 0);
    PlayerData *pd = ecs_field(it, PlayerData, 1);
    PlayerPhysics *pp = ecs_field(it, PlayerPhysics, 2);

    for (int i = 0; i < it->count; i++) {
        cpVect velocity = cpv(v[i].x, v[i].y);
        cpBody *body = pp[i].body;

        cpVect pos = cpBodyGetPosition(body);
        float w = pd[i].frameRect.width;
        float h = pd[i].frameRect.height;
        if ((pos.x < 0 && velocity.x < 0) || (pos.x + w > SCREEN_WIDTH && velocity.x > 0))
            velocity.x = 0;
        if ((pos.y < 0 && velocity.y < 0) || (pos.y + h > SCREEN_HEIGHT && velocity.y > 0))
            velocity.y = 0;
        if (cpvlength(velocity) > maxSpeed) {
            cpBodySetVelocity(body, cpvmult(cpvnormalize(velocity), maxSpeed));
        } else {
            cpBodySetVelocity(body, velocity);
        }
    }
}

void MovementImport(ecs_world_t *world) {
    ECS_MODULE(world, Movement);
    ECS_SYSTEM(world, MovementSystem, EcsOnUpdate, Velocity, PlayerData, PlayerPhysics);
}

#endif // ECS_COLLISION_SYSTEM_IMPLEMENTATION_ONLY
#endif // ECS_COLLISION_SYSTEM_IMPLEMENTATION