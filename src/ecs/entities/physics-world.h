#ifndef ECS_PHYSICS_WORLD_H
#define ECS_PHYSICS_WORLD_H

#include "box2d/box2d.h"

b2WorldId InitPhysicsWorld(ecs_world_t *world);

#endif // ECS_PHYSICS_WORLD_H

#ifdef ECS_PHYSICS_WORLD_IMPLEMENTATION
#ifndef ECS_PHYSICS_WORLD_IMPLEMENTATION_ONLY
#define ECS_PHYSICS_WORLD_IMPLEMENTATION_ONLY

b2WorldId InitPhysicsWorld(ecs_world_t *world) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, 0.0f};
    b2WorldId phyWorld = b2CreateWorld(&worldDef);
    return phyWorld;
}

#endif // ECS_PHYSICS_WORLD_IMPLEMENTATION_ONLY
#endif // ECS_PHYSICS_WORLD_IMPLEMENTATION