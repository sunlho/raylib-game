#ifndef TILEMAP_CHUNK_TYPE_H
#define TILEMAP_CHUNK_TYPE_H

#define CHUNK_SIZE 16

#include "flecs.h"
#include "raylib.h"
#include "tmx.h"

typedef struct TilemapChunkTile {
    unsigned int tile_gid;
    Rectangle dest_rect;
    struct TilemapChunkTile *next;
} TilemapChunkTile;

typedef struct TilemapChunkAnimTile {
    TilemapChunkTile *tile;
    unsigned int current_frame;
    unsigned int first_gid;
    unsigned int animation_len;
    tmx_anim_frame *animation;
    float start_time;
    struct TilemapChunkAnimTile *next;
} TilemapChunkAnimTile;

typedef struct TilemapChunk {
    int chunk_x, chunk_y;
    Rectangle dest_rect;
    int layer_index;
    TilemapChunkTile *tiles;
    TilemapChunkAnimTile *anim_tiles;

    RenderTexture2D texture;
    bool is_dirty;
    bool initialized;
} TilemapChunk;

typedef struct TilemapChunkHashEntity {
    ecs_entity_t entity;
    struct TilemapChunkHashEntity *next;
} TilemapChunkHashEntity;

typedef struct {
    uint64_t key;
    TilemapChunkHashEntity *value;
} TilemapChunkHashMap;

typedef struct TilemapObject {
    ecs_entity_t entity;
    Rectangle dest_rect;
    struct TilemapObject *next;
} TilemapObject;

ECS_COMPONENT_DECLARE(TilemapChunkTile);
ECS_COMPONENT_DECLARE(TilemapChunk);
extern ECS_COMPONENT_DECLARE(Rectangle);

static inline void TilemapRegisterChunkComponent() {
    ECS_COMPONENT(tilemap_ecs_world, TilemapChunkTile);
    ecs_struct(
        tilemap_ecs_world,
        {
            .entity = ecs_id(TilemapChunkTile),
            .members = {

                {.name = "tile_gid", .type = ecs_id(ecs_u32_t)},
                {.name = "src_rect", .type = ecs_id(Rectangle)},
                {.name = "dest_rect", .type = ecs_id(Rectangle)},
                {.name = "next", .type = ecs_id(ecs_iptr_t)},
            },
        });

    ECS_COMPONENT(tilemap_ecs_world, TilemapChunkAnimTile);
    ecs_struct(
        tilemap_ecs_world,
        {
            .entity = ecs_id(TilemapChunkAnimTile),
            .members = {
                {.name = "tile", .type = ecs_id(TilemapChunkTile)},
                {.name = "current_frame", .type = ecs_id(ecs_u32_t)},
                {.name = "animation_len", .type = ecs_id(ecs_u32_t)},
                {.name = "animation", .type = ecs_id(ecs_iptr_t)},
                {.name = "first_gid", .type = ecs_id(ecs_u32_t)},
                {.name = "start_time", .type = ecs_id(ecs_f32_t)},
                {.name = "next", .type = ecs_id(ecs_iptr_t)},
            },
        });

    ECS_COMPONENT_DEFINE(tilemap_ecs_world, TilemapChunk);
    ecs_struct(
        tilemap_ecs_world,
        {
            .entity = ecs_id(TilemapChunk),
            .members = {
                {.name = "chunk_x", .type = ecs_id(ecs_i32_t)},
                {.name = "chunk_y", .type = ecs_id(ecs_i32_t)},
                {.name = "dest_rect", .type = ecs_id(Rectangle)},
                {.name = "layer_index", .type = ecs_id(ecs_i32_t)},
                {.name = "tiles", .type = ecs_id(ecs_iptr_t)},
            },
        });
}

#endif // TILEMAP_CHUNK_TYPE_H
