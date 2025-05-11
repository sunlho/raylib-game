#ifndef ECS_TILE_COLLIDER_COMPONENT_H
#define ECS_TILE_COLLIDER_COMPONENT_H

#include "chipmunk/chipmunk.h"
#include "flecs.h"
#include "raylib.h"

typedef struct {
    int tileId;
    Vector2 a;
    Vector2 b;
    cpShape *shape;
} TileCollider;

#endif // ECS_TILE_COLLIDER_COMPONENT_H