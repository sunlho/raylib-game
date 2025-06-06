#ifndef ECS_TILE_ANIM_ENTITY_H
#define ECS_TILE_ANIM_ENTITY_H

#include "raylib.h"
// #include "stb_ds.h"
#include "tmx.h"

#include "../components/tile-anim.h"

typedef struct {
    int key;
    ecs_entity_t value;
} TileAnimMap;

void CreateTileAnimEntity(ecs_world_t *world, tmx_tile *tile, double pos_x, double pos_y);
void DrawAnimTiles(TMXRenderContext *ctx);

#endif // ECS_TILE_ANIM_ENTITY_H

#ifdef ECS_TILE_ANIM_ENTITY_IMPLEMENTATION
#ifndef ECS_TILE_ANIM_ENTITY_IMPLEMENTATION_ONCE
#define ECS_TILE_ANIM_ENTITY_IMPLEMENTATION_ONCE

extern ECS_COMPONENT_DECLARE(TileAnim);

static int framesCounter_a = 0;
static TileAnimMap *tile_animate_map = NULL;

ecs_entity_t GetTileAnimEntityGroup() {
    ecs_entity_t group = ecs_lookup(GetWorld(), TILE_ANIMATION_GROUP);
    if (!group || !ecs_is_valid(GetWorld(), group)) {
        group = ecs_entity(GetWorld(), {.name = TILE_ANIMATION_GROUP});
    }
    return group;
}

void DrawAnimTiles(TMXRenderContext *ctx) {
    framesCounter_a++;
    for (int i = 0; i < hmlen(tile_animate_map); i++) {
        ecs_entity_t tile_anim = tile_animate_map[i].value;
        TileAnim *ta = ecs_get_mut(GetWorld(), tile_anim, TileAnim);
        TileAnimInstance *tag = ta->instances;
        unsigned int base_gid = ta->tile->animation[ta->current_frame].tile_id;
        unsigned int gid = base_gid & TMX_FLIP_BITS_REMOVAL;
        tmx_tile *tile = ctx->map->tiles[gid + 1];

        while (tag) {
            TMXRenderContext _ctx = {ctx->map, tag->pos_x, tag->pos_y, ctx->tint};
            DrawTMXTile(tile, true, &_ctx);
            tag = tag->next;
        }
        if (framesCounter_a >= 30) {
            ta->current_frame = (ta->current_frame + 1) % ta->tile->animation_len;
        }
    }
    if (framesCounter_a >= 30) {
        framesCounter_a = 0;
    }
}

void CreateTileAnimEntity(ecs_world_t *world, tmx_tile *tile, double pos_x, double pos_y) {
    ecs_entity_t tile_anim = hmget(tile_animate_map, tile->id);
    if (tile_anim) {
        TileAnim *ta = ecs_get_mut(world, tile_anim, TileAnim);
        TileAnimInstance *instance = malloc(sizeof(TileAnimInstance));
        instance->pos_x = pos_x;
        instance->pos_y = pos_y;
        instance->next = ta->instances;
        ta->instances = instance;
    } else {
        TileAnimInstance *instance = malloc(sizeof(TileAnimInstance));
        instance->pos_x = pos_x;
        instance->pos_y = pos_y;
        instance->next = NULL;
        ecs_entity_t entity = ecs_new(world);
        ecs_set(
            world,
            entity,
            TileAnim,
            {
                .tile = tile,
                .current_frame = 0,
                .instances = instance,
            });
        hmput(tile_animate_map, tile->id, entity);
        ecs_entity_t group = GetTileAnimEntityGroup();
        if (group) {
            ecs_add_pair(world, entity, EcsChildOf, group);
        }
    }
}

#endif // ECS_TILE_ANIM_ENTITY_IMPLEMENTATION_ONCE
#endif // ECS_TILE_ANIM_ENTITY_IMPLEMENTATION
