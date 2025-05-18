#ifndef ECS_TILE_ANIMATE_ENTITY_H
#define ECS_TILE_ANIMATE_ENTITY_H

#include "../components/tile-animate.h"
#include "raylib.h"
#include "stb_ds.h"
#include "tmx-loader.h"
#include "tmx.h"

typedef struct TileAnimationMap {
    int key;
    ecs_entity_t value;
} TileAnimationMap;

void CreateTileAnimationEntity(ecs_world_t *world, tmx_tile *tile, double posX, double posY);
void DrawAnimatedTiles(TMXRenderContext *ctx);

#endif // ECS_TILE_ANIMATE_ENTITY_H

#ifdef ECS_TILE_ANIMATE_ENTITY_IMPLEMENTATION
#ifndef ECS_TILE_ANIMATE_ENTITY_IMPLEMENTATION_ONCE
#define ECS_TILE_ANIMATE_ENTITY_IMPLEMENTATION_ONCE

extern ECS_COMPONENT_DECLARE(TileAnimation);

static int framesCounterA = 0;
static TileAnimationMap *tile_animate_map = NULL;

ecs_entity_t GetTileAnimationEntityGroup() {
    ecs_entity_t group = ecs_lookup(GetWorld(), TILE_ANIMATION_GROUP);
    if (!group || !ecs_is_valid(GetWorld(), group)) {
        group = ecs_entity(GetWorld(), {.name = TILE_ANIMATION_GROUP});
    }
    return group;
}

void DrawAnimatedTiles(TMXRenderContext *ctx) {
    framesCounterA++;
    for (int i = 0; i < hmlen(tile_animate_map); i++) {
        ecs_entity_t tileAnimation = tile_animate_map[i].value;
        TileAnimation *ta = ecs_get_mut(GetWorld(), tileAnimation, TileAnimation);
        TileAnimationInstance *tag = ta->instances;
        unsigned int baseGid = ta->tile->animation[ta->currentFrame].tile_id;
        unsigned int gid = baseGid & TMX_FLIP_BITS_REMOVAL;
        tmx_tile *tile = ctx->map->tiles[gid + 1];

        while (tag) {
            TMXRenderContext _ctx = {ctx->map, tag->posX, tag->posY, ctx->tint};
            DrawTMXTile(tile, true, &_ctx);
            tag = tag->next;
        }
        if (framesCounterA >= 30) {
            ta->currentFrame = (ta->currentFrame + 1) % ta->tile->animation_len;
        }
    }
    if (framesCounterA >= 30) {
        framesCounterA = 0;
    }
}

void CreateTileAnimationEntity(ecs_world_t *world, tmx_tile *tile, double posX, double posY) {
    ecs_entity_t tileAnimation = hmget(tile_animate_map, tile->id);
    if (tileAnimation) {
        TileAnimation *ta = ecs_get_mut(world, tileAnimation, TileAnimation);
        TileAnimationInstance *instance = malloc(sizeof(TileAnimationInstance));
        instance->posX = posX;
        instance->posY = posY;
        instance->next = ta->instances;
        ta->instances = instance;
    } else {
        TileAnimationInstance *instance = malloc(sizeof(TileAnimationInstance));
        instance->posX = posX;
        instance->posY = posY;
        instance->next = NULL;
        ecs_entity_t entity = ecs_new(world);
        ecs_set(
            world,
            entity,
            TileAnimation,
            {
                .tile = tile,
                .currentFrame = 0,
                .instances = instance,
            });
        hmput(tile_animate_map, tile->id, entity);
        ecs_entity_t group = GetTileAnimationEntityGroup();
        if (group) {
            ecs_add_pair(world, entity, EcsChildOf, group);
        }
    }
}

#endif // ECS_TILE_ANIMATE_ENTITY_IMPLEMENTATION_ONCE
#endif // ECS_TILE_ANIMATE_ENTITY_IMPLEMENTATION
