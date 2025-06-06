#ifndef ECS_TILE_ANIM_H
#define ECS_TILE_ANIM_H

#include "flecs.h"
#include "tmx.h"

typedef struct TileAnimInstance {
    float pos_x, pos_y;
    struct TileAnimInstance *next;
} TileAnimInstance;

typedef struct {
    ecs_entity_t clock;
    tmx_tile *tile;
    int current_frame;
    TileAnimInstance *instances;
} TileAnim;

#endif // ECS_TILE_ANIM_H