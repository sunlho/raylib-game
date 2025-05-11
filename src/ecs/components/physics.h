

#ifndef ECS_PHYSICS_H
#define ECS_PHYSICS_H

#include "chipmunk/chipmunk.h"

typedef struct {
    cpBody *body;
} PhysicsBody;

typedef struct {
    cpSpace *space;
} PhysicsWorld;

#endif // ECS_PHYSICS_H