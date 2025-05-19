
#include "tmx-loader.h"
#include "constants.h"
#include "context.h"
#include "ecs/components/basic.h"
#include "ecs/components/player.h"
#include "ecs/entities/collision.h"
#include "ecs/entities/player.h"
#include "ecs/entities/tile-animate.h"
#include "utils.h"

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(PlayerData);

void DrawTMXLayerObjectCallBack(tmx_object *obj, TMXRenderContext *ctx) {
    if (obj->type) {
        if (!strcmp(obj->type, "start")) {
            const PlayerData *playerTag = ecs_get(GetWorld(), GetPlayerEntity(), PlayerData);
            InitPlayerPosition(GetWorld(), (Vector2){obj->x, obj->y});
        }
        if (!strcmp(obj->type, "Collision")) {
            PolygonCollisionEntity(GetWorld(), obj, ctx->posX + obj->x, ctx->posY + obj->y);
        }
    }
}

bool DrawTMXTileCallBack(tmx_tile *tile, TMXRenderContext *ctx) {
    if (tile->collision) {
        tmx_object *collision = tile->collision;
        while (collision) {
            switch (collision->obj_type) {
            case OT_POLYGON:
                PolygonCollisionEntity(GetWorld(), collision, ctx->posX + collision->x, ctx->posY + collision->y);
                break;
            }
            collision = collision->next;
        }
    }
    if (tile->animation) {
        CreateTileAnimationEntity(GetWorld(), tile, ctx->posX, ctx->posY);
    }
    return true;
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

LayerRenderTexture *InitMap(const char *mapPath, TMXRenderContext *ctx) {
    tmx_map *map = LoadTMX(mapPath);
    ctx->map = map;
    double tex_width = map->width * map->tile_width;
    double tex_height = map->height * map->tile_height;
    SetWorldSize(tex_width, tex_height);
    LayerRenderTexture *layerTextures = InitLayerRenderTexture(map->ly_head, tex_width, tex_height, ctx);
    return layerTextures;
}