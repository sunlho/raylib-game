#ifndef ECS_TILE_COLLIDER_COMPONENT_H
#define ECS_TILE_COLLIDER_COMPONENT_H

#include <flecs.h>
#include <raylib.h>

typedef struct {
    int tile_x, tile_y;
    int point_count;
    double **points;
} TileCollider;

#endif // ECS_TILE_COLLIDER_COMPONENT_H