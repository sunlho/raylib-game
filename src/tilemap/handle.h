#ifndef TILEMAP_HANDLE_MAP_H
#define TILEMAP_HANDLE_MAP_H

#include "tmx.h"

#include "types/chunk.h"
#include "types/types.h"

#include "collison.h"
#include "utils.h"

void TilemapHandleMap(tmx_map *map);

#endif // TILEMAP_HANDLE_MAP_H

#ifdef TILEMAP_HANDLE_MAP_IMPLEMENTATION
#ifndef TILEMAP_HANDLE_MAP_IMPLEMENTATION_ONCE
#define TILEMAP_HANDLE_MAP_IMPLEMENTATION_ONCE

#define TILEMAP_CHUNK_IMPLEMENTATION
#include "chunk.h"

extern ECS_COMPONENT_DECLARE(TilemapDrawable);
extern ECS_COMPONENT_DECLARE(TilemapChunk);
extern ECS_COMPONENT_DECLARE(Rectangle);

static void TilemapHandleAnimateTile() {
}

static void TilemapHandleCollision(tmx_object *obj, double pos_x, double pos_y) {
    switch (obj->obj_type) {
    case OT_POLYGON:
        TilemapPolygonCollision(obj, pos_x, pos_y);
        break;
    case OT_SQUARE:
        TilemapSquareCollision(obj, pos_x, pos_y);
        break;
    case OT_ELLIPSE:
        TilemapEllipseCollision(obj, pos_x, pos_y);
        break;
    default:
        break;
    }
}

static void TilemapHandleTileCollision(tmx_tile *tile, double pos_x, double pos_y) {
    if (tile->collision) {
        tmx_object *collision = tile->collision;
        while (collision) {
            TilemapHandleCollision(collision, pos_x + collision->x, pos_y + collision->y);
            collision = collision->next;
        }
    }
}

static void TilemapHandleLayerTiles(tmx_layer *layer, TilemapCtx *ctx) {
    int width = ctx->map->width;
    int height = ctx->map->height;

    for (int by = 0; by < CeilDivSafe(height, CHUNK_SIZE); by++) {
        for (int bx = 0; bx < CeilDivSafe(width, CHUNK_SIZE); bx++) {
            TilemapChunkTile *tiles = NULL;
            TilemapChunkAnimTile *anim_tiles = NULL;
            int tiles_in_chunk_x = ECS_MIN(CHUNK_SIZE, width - bx * CHUNK_SIZE);
            int tiles_in_chunk_y = ECS_MIN(CHUNK_SIZE, height - by * CHUNK_SIZE);
            for (int y = 0; y < tiles_in_chunk_y; y++) {
                for (int x = 0; x < tiles_in_chunk_x; x++) {
                    unsigned int index = (by * CHUNK_SIZE + y) * width + (bx * CHUNK_SIZE + x);
                    unsigned int gid = layer->content.gids[index] & TMX_FLIP_BITS_REMOVAL;
                    tmx_tile *tile = ctx->map->tiles[gid];
                    if (tile) {
                        double pos_x = ctx->pos_x + (bx * CHUNK_SIZE + x) * tile->width;
                        double pos_y = ctx->pos_y + (by * CHUNK_SIZE + y) * tile->height;

                        Rectangle src_rect = {tile->ul_x, tile->ul_y, tile->width, tile->height};
                        Rectangle dest_rect = {pos_x, pos_y, tile->width, tile->height};

                        TilemapChunkTile *chunk_tile = MemAlloc(sizeof(TilemapChunkTile));

                        if (chunk_tile) {
                            chunk_tile->tile_gid = gid;
                            chunk_tile->dest_rect = dest_rect;
                            chunk_tile->next = tiles;
                            tiles = chunk_tile;
                        }
                        if (tile->animation_len > 0 && tile->animation) {
                            unsigned int first_gid = tmx_find_tileset_by_name(ctx->map, tile->tileset->name)->firstgid;
                            TilemapChunkAnimTile *anim_tile = MemAlloc(sizeof(TilemapChunkAnimTile));
                            if (anim_tile) {
                                anim_tile->tile = chunk_tile;
                                anim_tile->current_frame = 0;
                                anim_tile->first_gid = first_gid;
                                anim_tile->animation_len = tile->animation_len;
                                anim_tile->animation = tile->animation;
                                anim_tile->next = anim_tiles;
                                anim_tiles = anim_tile;
                            }
                        }
                        TilemapHandleTileCollision(tile, pos_x, pos_y);
                    }
                }
            }
            if (!tiles) {
                continue;
            }
            int tile_offset_y = ctx->pos_y + by * CHUNK_SIZE * ctx->map->tile_height;
            TilemapChunk chunk_data = {
                .chunk_x = bx,
                .chunk_y = by,
                .dest_rect = (Rectangle){
                    .x = bx * CHUNK_SIZE * ctx->map->tile_width,
                    .y = tile_offset_y,
                    .width = CHUNK_SIZE * ctx->map->tile_width,
                    .height = CHUNK_SIZE * ctx->map->tile_height,
                },
                .layer_index = ctx->layer_index,
                .tiles = tiles,
                .anim_tiles = anim_tiles,
            };
            int sort_y = GetLayerSortY(ctx->layer_index, tile_offset_y + CHUNK_SIZE * ctx->map->tile_height);
            ecs_entity_t chunk = TilemapCreateChunkEntity(&chunk_data, sort_y, false);
        }
    }
}

static void TilemapHandleLayerObjectGroup(tmx_object_group *object_group, TilemapCtx *ctx) {
    tmx_object *head = object_group->head;
    while (head) {
        if (head->visible) {
            if (head->type && !strcmp(head->type, tilemap_collision_type)) {
                TilemapHandleCollision(head, ctx->pos_x + head->x, ctx->pos_y + head->y);
            }

            if (head->obj_type == OT_TILE) {
                tmx_tile *tile = ctx->map->tiles[head->content.gid];
                if (tile) {
                    double pos_x = ctx->pos_x + head->x;
                    double pos_y = ctx->pos_y + head->y - tile->height;
                    Rectangle dest_rect = {pos_x, pos_y, tile->width, tile->height};
                    Rectangle dest_rect2 = {0, 0, tile->width, tile->height};

                    TilemapChunkTile *chunk_tile = MemAlloc(sizeof(TilemapChunkTile));

                    if (chunk_tile) {
                        chunk_tile->tile_gid = head->content.gid;
                        chunk_tile->dest_rect = dest_rect;
                        chunk_tile->next = NULL;
                    }

                    TilemapChunkAnimTile *anim_tile = NULL;

                    if (tile->animation_len > 0 && tile->animation) {
                        unsigned int first_gid = tmx_find_tileset_by_name(ctx->map, tile->tileset->name)->firstgid;
                        anim_tile = MemAlloc(sizeof(TilemapChunkAnimTile));
                        if (anim_tile) {
                            anim_tile->tile = chunk_tile;
                            anim_tile->current_frame = 0;
                            anim_tile->first_gid = first_gid;
                            anim_tile->animation_len = tile->animation_len;
                            anim_tile->animation = tile->animation;
                            anim_tile->next = NULL;
                        }
                    }

                    int chunk_x = (int)floor(pos_x / (CHUNK_SIZE * ctx->map->tile_width));
                    int chunk_y = (int)floor(pos_y / (CHUNK_SIZE * ctx->map->tile_height));

                    TilemapChunk chunk_data = {
                        .chunk_x = chunk_x,
                        .chunk_y = chunk_y,
                        .dest_rect = dest_rect,
                        .layer_index = ctx->layer_index,
                        .tiles = chunk_tile,
                        .anim_tiles = anim_tile,
                    };
                    int sort_y = GetLayerSortY(ctx->layer_index, pos_y + tile->height);
                    ecs_entity_t chunk = TilemapCreateChunkEntity(&chunk_data, sort_y, true);
                    TilemapHandleTileCollision(tile, pos_x, pos_y);
                }
            }
            head = head->next;
        }
    }
}

void TilemapHandleMap(tmx_map *map) {
    TilemapRegisterChunkComponent();
    TilemapRegisterRenderComponent();

    TilemapCtx ctx = {map, 0, 0, 0};
    tmx_layer *layer = map->ly_head;
    while (layer) {
        switch (layer->type) {
        case L_OBJGR:
            TilemapHandleLayerObjectGroup(layer->content.objgr, &ctx);
            break;
        case L_IMAGE:
            break;
        case L_LAYER:
            TilemapHandleLayerTiles(layer, &ctx);
            break;
        case L_NONE:
            break;
        }

        ctx.layer_index++;
        layer = layer->next;
    }
}

#endif // TILEMAP_HANDLE_MAP_IMPLEMENTATION_ONCE
#endif // TILEMAP_HANDLE_MAP_IMPLEMENTATION
