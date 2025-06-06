#ifndef TILEMAP_RENDER_TYPE_H
#define TILEMAP_RENDER_TYPE_H

#include "flecs.h"
#include "raylib.h"

typedef struct TilemapDrawable {
    float sort_y;
    float layer_index;
    bool is_dynamic;
    void (*render_fn)(ecs_world_t *world, ecs_entity_t entity);
} TilemapDrawable;

typedef struct TilemapInView {
    double frame;
} TilemapInView;

#endif // TILEMAP_RENDER_TYPE_H
