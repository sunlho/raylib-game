#ifndef ECS_BASIC_COMPONENT_H
#define ECS_BASIC_COMPONENT_H

#include "flecs.h"

typedef struct
{
    float x;
    float y;
} Position, Velocity;

#endif // ECS_BASIC_COMPONENT_H