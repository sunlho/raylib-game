#ifndef TILEMAP_CHUNK_TYPE_H
#define TILEMAP_CHUNK_TYPE_H

#define CHUNK_SIZE 16

#include "flecs.h"
#include "raylib.h"
#include "tmx.h"

typedef struct TilemapChunkTile {
    unsigned int tile_gid;
    Rectangle src_rect;
    Rectangle dest_rect;
    struct TilemapChunkTile *next;
} TilemapChunkTile;

typedef struct TilemapChunkAnimTile {
    TilemapChunkTile *tile;
    unsigned int current_frame;
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

#endif // TILEMAP_CHUNK_TYPE_H
