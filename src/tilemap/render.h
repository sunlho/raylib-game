#ifndef TILEMAP_RENDER_H
#define TILEMAP_RENDER_H

#include "flecs.h"
#include "raylib.h"

#include "types/render.h"

#include "utils.h"

void TilemapRegisterRenderSystem();

#endif // TILEMAP_RENDER_H

#ifdef TILEMAP_RENDER_IMPLEMENTATION
#ifndef TILEMAP_RENDER_IMPLEMENTATION_ONCE
#define TILEMAP_RENDER_IMPLEMENTATION_ONCE

#define TILEMAP_VIEWPORT_IMPLEMENTATION
#include "viewport.h"

ECS_COMPONENT_DECLARE(TilemapInView);
extern ECS_COMPONENT_DECLARE(TilemapChunk);
extern ECS_COMPONENT_DECLARE(TilemapDrawable);

static int CompareDrawableBySortY(ecs_entity_t e1, const void *ptr1, ecs_entity_t e2, const void *ptr2) {
    (void)e1;
    (void)e2;
    const TilemapDrawable *drawable1 = ptr1;
    const TilemapDrawable *drawable2 = ptr2;
    float sort_y1 = drawable1->is_dynamic ? GetLayerSortY(drawable1->layer_index, drawable1->sort_y + 1) : drawable1->sort_y;
    float sort_y2 = drawable2->is_dynamic ? GetLayerSortY(drawable2->layer_index, drawable2->sort_y + 1) : drawable2->sort_y;

    if (sort_y1 < sort_y2) {
        return -1;
    } else if (sort_y1 > sort_y2) {
        return 1;
    }
    return 0;
}

static void RenderChunksSorted(ecs_iter_t *it) {
    TilemapDrawable *drawables = ecs_field(it, TilemapDrawable, 0);
    TilemapInView *views = ecs_field(it, TilemapInView, 1);
    for (int i = 0; i < it->count; i++) {
        if (views[i].frame < GetTime()) {
            ecs_remove(it->world, it->entities[i], TilemapInView);
            continue;
        }
        TilemapDrawable *drawable = &drawables[i];
        drawable->render_fn(it->world, it->entities[i]);
    }
}

void TilemapRegisterRenderSystem() {
    ECS_TAG(tilemap_ecs_world, render);
    ecs_set_scope(tilemap_ecs_world, render);

    ECS_COMPONENT_DEFINE(tilemap_ecs_world, TilemapInView);
    ecs_struct(
        tilemap_ecs_world,
        {
            .entity = ecs_id(TilemapInView),
            .members = {
                {.name = "frame", .type = ecs_id(ecs_f64_t)},
            },
        });

    ecs_query_desc_t chunk_query = {
        .terms = {{.id = ecs_id(TilemapDrawable)}, {.id = ecs_id(TilemapInView)}},
        .order_by = ecs_id(TilemapDrawable),
        .order_by_callback = CompareDrawableBySortY,
    };

    ecs_entity_t system_entity = ecs_entity(
        tilemap_ecs_world,
        {
            .name = "TilemapRenderSystem",
            .add = ecs_ids(ecs_dependson(EcsOnUpdate)),
        });

    TilemapRegisterViewportCullingSystem();
    ecs_system(tilemap_ecs_world, {.entity = system_entity, .query = chunk_query, .callback = RenderChunksSorted});
}

#endif // TILEMAP_RENDER_IMPLEMENTATION_ONCE
#endif // TILEMAP_RENDER_IMPLEMENTATION
