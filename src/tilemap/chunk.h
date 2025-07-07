#ifndef TILEMAP_CHUNK_H
#define TILEMAP_CHUNK_H

#include "flecs.h"
#include "raylib.h"
#include "tmx.h"

#include "types/chunk.h"
#include "types/render.h"
#include "types/types.h"

#include "utils.h"
#include "viewport.h"

ecs_entity_t TilemapCreateChunkEntity(TilemapChunk *chunk_data, float sort_y, bool is_object);

#endif // TILEMAP_CHUNK_H

#ifdef TILEMAP_CHUNK_IMPLEMENTATION
#ifndef TILEMAP_CHUNK_IMPLEMENTATION_ONCE
#define TILEMAP_CHUNK_IMPLEMENTATION_ONCE

static TilemapChunkHashMap *tilemap_chunk_map = NULL;

static TilemapObject *tilemap_object_list = NULL;

extern ECS_COMPONENT_DECLARE(TilemapChunk);
extern ECS_COMPONENT_DECLARE(TilemapDrawable);

static void TilemapChunkDraw(TilemapChunkTile *chunk_tile) {
    if (chunk_tile) {
        while (chunk_tile) {
            tmx_tile *tile = tilemap_tmx_map->tiles[chunk_tile->tile_gid];
            if (!tile) {
                chunk_tile = chunk_tile->next;
                continue;
            }
            tmx_image *im = tile->image ? tile->image : tile->tileset->image;
            if (im && im->resource_image) {
                Texture *image = (Texture *)im->resource_image;
                DrawTexturePro(*image, (Rectangle){tile->ul_x, tile->ul_y, tile->width, tile->height}, chunk_tile->dest_rect, (Vector2){0, 0}, 0, WHITE);
            }
            chunk_tile = chunk_tile->next;
        }
    }
}

// Why would this approach actually result in worse performance?
static void TilemapChunkCacheRender(ecs_world_t *world, ecs_entity_t entity) {
    TilemapChunk *chunk_data = ecs_get_mut(world, entity, TilemapChunk);
    if (chunk_data && chunk_data->tiles) {
        if (!chunk_data->initialized && chunk_data->texture.id != 0) {
            chunk_data->initialized = true;
            if (chunk_data->texture.id == 0) {
                chunk_data->texture = LoadRenderTexture(chunk_data->dest_rect.width, chunk_data->dest_rect.height);
            }
            BeginTextureMode(chunk_data->texture);
            ClearBackground(BLANK);
            TilemapChunkDraw(chunk_data->tiles);
            EndTextureMode();
        }

        if (chunk_data->is_dirty && chunk_data->texture.id != 0) {
            chunk_data->is_dirty = false;
            BeginTextureMode(chunk_data->texture);
            ClearBackground(BLANK);
            TilemapChunkDraw(chunk_data->tiles);
            EndTextureMode();
        }

        if (chunk_data->texture.id != 0) {
            DrawTexturePro(
                chunk_data->texture.texture,
                (Rectangle){0, 0, chunk_data->dest_rect.width, -chunk_data->dest_rect.height},
                chunk_data->dest_rect,
                (Vector2){0, 0},
                0,
                WHITE);
        }

        if (*tilemap_debug_mode) {
            Color draw_color = BLUE;
            draw_color.a = 128;
            DrawRectangleLinesEx(chunk_data->dest_rect, 1, draw_color);
            DrawText(
                TextFormat("(%d,%d)", chunk_data->chunk_x, chunk_data->chunk_y),
                chunk_data->dest_rect.x + 5,
                chunk_data->dest_rect.y + 5,
                10,
                draw_color);
        }
    }
}

static void TilemapChunkRender(ecs_world_t *world, ecs_entity_t entity) {
    const TilemapChunk *chunk_data = ecs_get(world, entity, TilemapChunk);
    if (chunk_data && chunk_data->tiles) {
        TilemapChunkDraw(chunk_data->tiles);
        if (*tilemap_debug_mode) {
            Color draw_color = BLUE;
            draw_color.a = 128;
            DrawRectangleLinesEx(chunk_data->dest_rect, 1, draw_color);
            DrawText(
                TextFormat("(%d,%d)", chunk_data->chunk_x, chunk_data->chunk_y),
                chunk_data->dest_rect.x + 5,
                chunk_data->dest_rect.y + 5,
                10,
                draw_color);
        }
    }
}

static ecs_entity_t TilemapGetChunkEntityLayer(int index) {
    char layer_name[32];
    snprintf(layer_name, sizeof(layer_name), "TilemapChunkLayer_%d", index);
    ecs_entity_t layer_group = ecs_lookup(tilemap_ecs_world, layer_name);
    if (!layer_group || !ecs_is_valid(tilemap_ecs_world, layer_group)) {
        layer_group = ecs_entity(tilemap_ecs_world, {.name = layer_name});
    }
    return layer_group;
}

ecs_entity_t TilemapCreateChunkEntity(TilemapChunk *chunk_data, float sort_y, bool is_object) {

    ecs_entity_t chunk = ecs_new(tilemap_ecs_world);

    ecs_set_id(tilemap_ecs_world, chunk, ecs_id(TilemapChunk), sizeof(TilemapChunk), chunk_data);
    ecs_set(tilemap_ecs_world, chunk, TilemapDrawable, {.sort_y = sort_y, .layer_index = (float)chunk_data->layer_index, .render_fn = TilemapChunkRender});

    ecs_entity_t layer_group = TilemapGetChunkEntityLayer(chunk_data->layer_index);
    ecs_add_pair(tilemap_ecs_world, chunk, EcsChildOf, layer_group);

    if (!is_object) {
        uint64_t key = MortonEncode(chunk_data->chunk_x, chunk_data->chunk_y);
        TilemapChunkHashMap *hash_entity = hmgetp_null(tilemap_chunk_map, key);
        if (!hash_entity) {
            TilemapChunkHashEntity *value = MemAlloc(sizeof(TilemapChunkHashEntity));
            value->entity = chunk;
            value->next = NULL;
            hmput(tilemap_chunk_map, key, value);
        } else {
            TilemapChunkHashEntity *value = MemAlloc(sizeof(TilemapChunkHashEntity));
            if (value) {
                value->entity = chunk;
                value->next = NULL;
                value->next = hash_entity->value;
                hash_entity->value = value;
            }
        }
    } else {
        TilemapObject *object = MemAlloc(sizeof(TilemapObject));
        if (object) {
            object->entity = chunk;
            object->dest_rect = chunk_data->dest_rect;
            object->next = tilemap_object_list;
            tilemap_object_list = object;
        }
    }

    return chunk;
}

#endif // TILEMAP_CHUNK_IMPLEMENTATION_ONCE
#endif // TILEMAP_CHUNK_IMPLEMENTATION
