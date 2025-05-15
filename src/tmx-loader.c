
#include "constants.h"
#include "context.h"
#include "ecs/components/basic.h"
#include "ecs/components/player.h"
#include "ecs/components/tile-collider.h"
#include "ecs/entities/player.h"
#include "ecs/entities/tile-animate.h"
#include "utils.h"
#define TMX_LOADER_IMPLEMENTATION
#include "tmx-loader.h"
#define ECS_COLLISION_ENTITY_IMPLEMENTATION
#include "ecs/entities/collision.h"

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(PlayerData);
extern ECS_COMPONENT_DECLARE(TileCollider);

void DrawTMXLayerObjectCallBack(tmx_map *map, tmx_object *obj, double posX, double posY, Color tint) {
    if (obj->type) {
        if (!strcmp(obj->type, "start")) {
            const PlayerData *playerTag = ecs_get(GetWorld(), GetPlayerEntity(), PlayerData);
            InitPlayerPosition(GetWorld(), (Vector2){obj->x, obj->y});
        }
    }
}

void DrawTMXTileCallBack(tmx_tile *tile, TMXRenderContext *ctx) {
    if (tile->collision) {
        tmx_object *collision = tile->collision;
        while (collision) {
            switch (collision->obj_type) {
            case OT_POLYLINE:
                PolylineCollisionEntity(GetWorld(), collision, ctx->posX, ctx->posY, tile->id, ctx->scale);
                break;
            }
            collision = collision->next;
        }
    }
    if (tile->animation) {
        CreateTileAnimationEntity(GetWorld(), tile, ctx->posX, ctx->posY);
    }
}

LayerRenderTexture *InitLayerRenderTexture(tmx_layer *layer, double tex_width, double tex_height, TMXRenderContext *ctx) {
    LayerRenderTexture *layerTexture = (LayerRenderTexture *)malloc(sizeof(LayerRenderTexture));
    layerTexture->texture = LoadRenderTexture(tex_width, tex_height);
    BeginTextureMode(layerTexture->texture);
    ClearBackground(BLANK);

    DrawTMXLayer(layer, ctx);
    EndTextureMode();
    if (layer->next) {
        layerTexture->next = InitLayerRenderTexture(layer->next, tex_width, tex_height, ctx);
    } else {
        layerTexture->next = NULL;
    }
    return layerTexture;
}

// void DrawAnimatedTiles(AnimatedTile *animatedTiles, TMXRenderContext *ctx) {
//     AnimatedTile *current = animatedTiles;
//     framesCounter++;

//     while (current) {
//         TMXRenderContext _ctx = {ctx->map, current->posX, current->posY, ctx->tint, ctx->scale};
//         unsigned int baseGid = current->tile->animation[current->currentFrame].tile_id;
//         unsigned int gid = baseGid & TMX_FLIP_BITS_REMOVAL;
//         tmx_tile *tile = ctx->map->tiles[gid + 1];
//         DrawTMXTile(tile, true, &_ctx);
//         if (framesCounter >= FPS / 3) {
//             current->currentFrame = (current->currentFrame + 1) % current->tile->animation_len;
//         }
//         current = current->next;
//     }
//     if (framesCounter >= FPS / 3) {
//         framesCounter = 0;
//     }
// }

LayerRenderTexture *InitMap(const char *mapPath, TMXRenderContext *ctx) {
    tmx_map *map = LoadTMX(mapPath);
    ctx->map = map;
    double tex_width = map->width * map->tile_width * ctx->scale;
    double tex_height = map->height * map->tile_height * ctx->scale;
    LayerRenderTexture *layerTextures = InitLayerRenderTexture(map->ly_head, tex_width, tex_height, ctx);
    return layerTextures;
}