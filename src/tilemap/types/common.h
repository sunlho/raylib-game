#ifndef TILEMAP_COMMON_TYPE_H
#define TILEMAP_COMMON_TYPE_H

#include "flecs.h"
#include "raylib.h"

ECS_COMPONENT_DECLARE(Rectangle);

static inline void TilemapRegisterCommonComponent() {
    ECS_COMPONENT_DEFINE(tilemap_ecs_world, Rectangle);
    ecs_struct(
        tilemap_ecs_world,
        {
            .entity = ecs_id(Rectangle),
            .members = {
                {.name = "x", .type = ecs_id(ecs_f32_t)},
                {.name = "y", .type = ecs_id(ecs_f32_t)},
                {.name = "width", .type = ecs_id(ecs_f32_t)},
                {.name = "height", .type = ecs_id(ecs_f32_t)},
            },
        });
}

#endif // TILEMAP_COMMON_TYPE_H
