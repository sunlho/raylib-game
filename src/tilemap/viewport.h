#ifndef TILEMAP_VIEWPORT_H
#define TILEMAP_VIEWPORT_H

#include "flecs.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>

#include "types/chunk.h"
#include "types/render.h"
#include "utils.h"

#define CULL_PADDING 0.0f

ecs_entity_t TilemapRegisterViewportCullingSystem();

#endif // TILEMAP_VIEWPORT_H

#ifdef TILEMAP_VIEWPORT_IMPLEMENTATION
#ifndef TILEMAP_VIEWPORT_IMPLEMENTATION_ONCE
#define TILEMAP_VIEWPORT_IMPLEMENTATION_ONCE

extern ECS_COMPONENT_DECLARE(TilemapInView);

static void UpdateChunkVisibility(ecs_iter_t *it) {
    Vector2 screen_min = GetScreenToWorld2D((Vector2){-CULL_PADDING, -CULL_PADDING}, *tilemap_world_camera);
    Vector2 screen_max = GetScreenToWorld2D((Vector2){GetScreenWidth() + CULL_PADDING, GetScreenHeight() + CULL_PADDING}, *tilemap_world_camera);

    int min_cx = (int)floorf(screen_min.x / CHUNK_SIZE / 32);
    int min_cy = (int)floorf(screen_min.y / CHUNK_SIZE / 32);
    int max_cx = (int)floorf(screen_max.x / CHUNK_SIZE / 32);
    int max_cy = (int)floorf(screen_max.y / CHUNK_SIZE / 32);
    Rectangle screen_box = {
        .x = screen_min.x,
        .y = screen_min.y,
        .width = screen_max.x - screen_min.x,
        .height = screen_max.y - screen_min.y};

    for (int cy = min_cy; cy <= max_cy; ++cy) {
        for (int cx = min_cx; cx <= max_cx; ++cx) {
            uint64_t key = MortonEncode(cx, cy);
            TilemapChunkHashMap *bucket = hmgetp_null(tilemap_chunk_map, key);
            if (!bucket)
                continue;

            TilemapChunkHashEntity *cur = bucket->value;
            while (cur) {
                ecs_set(tilemap_ecs_world, cur->entity, TilemapInView, {.frame = GetTime() + (double)1.0});
                cur = cur->next;
            }
        }
    }

    TilemapObject *head = tilemap_object_list;
    while (head) {
        ecs_entity_t entity = head->entity;
        Rectangle dest_rect = head->dest_rect;
        bool collision = CheckCollisionRecs(screen_box, dest_rect);
        if (collision) {
            ecs_set(tilemap_ecs_world, entity, TilemapInView, {.frame = GetTime() + (double)1.0});
        }
        head = head->next;
    }
}

ecs_entity_t TilemapRegisterViewportCullingSystem() {

    ecs_entity_t system_entity = ecs_entity(
        tilemap_ecs_world,
        {
            .name = "TilemapViewportCullingSystem",
            .add = ecs_ids(ecs_dependson(EcsPreUpdate)),
        });

    ecs_system(tilemap_ecs_world, {.entity = system_entity, .callback = UpdateChunkVisibility});

    return system_entity;
}

#endif // TILEMAP_VIEWPORT_IMPLEMENTATION_ONCE
#endif // TILEMAP_VIEWPORT_IMPLEMENTATION
