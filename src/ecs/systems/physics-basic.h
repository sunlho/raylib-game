#ifndef ECS_PHYSICS_BASIC_H
#define ECS_PHYSICS_BASIC_H

#include "chipmunk/chipmunk.h"
#include "context.h"
#include "flecs.h"

void PhysicsBasicImport(ecs_world_t *world);

#endif // ECS_PHYSICS_BASIC_H

#ifdef ECS_PHYSICS_BASIC_IMPLEMENTATION
#ifndef ECS_PHYSICS_BASIC_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(PlayerData);
extern ECS_COMPONENT_DECLARE(PlayerPhysics);
extern ECS_COMPONENT_DECLARE(PhysicsWorld);

void PhysicsStepSystem(const ecs_iter_t *it) {
    cpSpaceStep(GetSpace(), it->delta_time);
}

void SyncPositionSystem(ecs_iter_t *it) {
    Position *p = ecs_field(it, Position, 0);
    PlayerPhysics *pb = ecs_field(it, PlayerPhysics, 2);

    for (int i = 0; i < it->count; i++) {
        cpVect pos = cpBodyGetPosition(pb[i].body);
        p[i].x = pos.x;
        p[i].y = pos.y;
    }
}

void PhysicsBasicImport(ecs_world_t *world) {
    ECS_MODULE(world, PhysicsBasicImport);
    ECS_SYSTEM(world, PhysicsStepSystem, EcsPreUpdate);
    ECS_SYSTEM(world, SyncPositionSystem, EcsOnUpdate, Position, PlayerData, PlayerPhysics);
}

#endif // ECS_PHYSICS_BASIC_IMPLEMENTATION_ONLY
#endif // ECS_PHYSICS_BASIC_IMPLEMENTATION
