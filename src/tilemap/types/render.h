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

ECS_COMPONENT_DECLARE(TilemapDrawable);
ECS_COMPONENT_DECLARE(TilemapInView);

static inline void TilemapRegisterRenderComponent() {
    ECS_COMPONENT_DEFINE(tilemap_ecs_world, TilemapDrawable);
    ecs_struct(
        tilemap_ecs_world,
        {
            .entity = ecs_id(TilemapDrawable),
            .members = {
                {.name = "sort_y", .type = ecs_id(ecs_f32_t)},
                {.name = "layer_index", .type = ecs_id(ecs_f32_t)},
                {.name = "is_dynamic", .type = ecs_id(ecs_bool_t)},
                {.name = "render_fn", .type = ecs_id(ecs_iptr_t)},
            },
        });

    ECS_COMPONENT_DEFINE(tilemap_ecs_world, TilemapInView);
    ecs_struct(
        tilemap_ecs_world,
        {
            .entity = ecs_id(TilemapInView),
            .members = {
                {.name = "frame", .type = ecs_id(ecs_f64_t)},
            },
        });
}

#endif // TILEMAP_RENDER_TYPE_H
