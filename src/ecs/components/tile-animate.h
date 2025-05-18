#ifndef ECS_TILE_ANIMATE_H
#define ECS_TILE_ANIMATE_H

#include "flecs.h"
#include "tmx.h"

typedef struct TileAnimationInstance {
    float posX, posY;
    struct TileAnimationInstance *next;
} TileAnimationInstance;

typedef struct TileAnimation {
    ecs_entity_t clock;
    tmx_tile *tile;
    int currentFrame;
    TileAnimationInstance *instances;
} TileAnimation;

#endif // ECS_TILE_ANIMATE_H