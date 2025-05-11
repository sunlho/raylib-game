#ifndef ECS_PHYSICS_WORLD_H
#define ECS_PHYSICS_WORLD_H

#include "chipmunk/chipmunk.h"
#include "flecs.h"

cpSpace *InitPhysicsWorld(ecs_world_t *world);

#endif // ECS_PHYSICS_WORLD_H

#ifdef ECS_PHYSICS_WORLD_IMPLEMENTATION
#ifndef ECS_PHYSICS_WORLD_IMPLEMENTATION_ONLY
#define ECS_PHYSICS_WORLD_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(PhysicsWorld);

cpSpace *InitPhysicsWorld(ecs_world_t *world) {
    cpSpace *space = cpSpaceNew();
    ecs_singleton_set(world, PhysicsWorld, {.space = space});

    const PhysicsWorld *pw = ecs_singleton_get(world, PhysicsWorld);
    cpSpaceSetGravity(space, cpv(0, 0));
    cpSpaceSetCollisionSlop(space, 0.5);
    ecs_singleton_modified(world, PhysicsWorld);
    return space;
}

#endif // ECS_PHYSICS_WORLD_IMPLEMENTATION_ONLY
#endif // ECS_PHYSICS_WORLD_IMPLEMENTATION