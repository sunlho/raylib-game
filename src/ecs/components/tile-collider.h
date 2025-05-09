#ifndef ECS_TILE_COLLIDER_COMPONENT_H
#define ECS_TILE_COLLIDER_COMPONENT_H

#include <flecs.h>
#include <raylib.h>

typedef struct {
    int tileId;
    Vector2 a;
    Vector2 b;
    Rectangle aabb;
    Vector2 center;
} TileCollider;

#endif // ECS_TILE_COLLIDER_COMPONENT_H