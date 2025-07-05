#ifndef TILEMAP_TYPES_H
#define TILEMAP_TYPES_H

#include "tmx.h"

typedef struct TilemapCtx {
    tmx_map *map;
    double pos_x;
    double pos_y;
    int layer_index;
    tmx_layer *layer;
} TilemapCtx;

typedef struct {
    ecs_world_t *world;
    b2WorldId physics_world;
    Camera2D *camera;
    const char *collision_type;
    bool *debug_mode;
} TilemapOptions;

typedef struct {
    double width;
    double height;
} TilemapWorldSize;

typedef struct {
    TilemapWorldSize size;
    tmx_map *map;
    void (*render)();
} TilemapInstance;

#endif // TILEMAP_TYPES_H
