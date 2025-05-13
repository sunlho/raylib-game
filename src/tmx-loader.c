
#include "constants.h"
#include "context.h"
#include "ecs/components/basic.h"
#include "ecs/components/player.h"
#include "ecs/components/tile-collider.h"
#include "ecs/entities/player.h"
#include "utils.h"
#define TMX_LOADER_IMPLEMENTATION
#include "tmx-loader.h"
#define ECS_COLLISION_ENTITY_IMPLEMENTATION
#include "ecs/entities/collision.h"

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(PlayerData);
extern ECS_COMPONENT_DECLARE(TileCollider);

void DrawTMXLayerObjectFunc(tmx_map *map, tmx_object *obj, int posX, int posY, Color tint) {
    if (obj->type) {
        if (!strcmp(obj->type, "start")) {
            const PlayerData *playerTag = ecs_get(GetWorld(), GetPlayerEntity(), PlayerData);
            InitPlayerPosition(GetWorld(), (Vector2){obj->x, obj->y});
        }
    }
}

void DrawTmxTileCollisionFunc(tmx_tile *tile, int posX, int posY) {
    tmx_object *collision = tile->collision;
    while (collision) {
        switch (collision->obj_type) {
        case OT_POLYLINE:
            PolylineCollisionEntity(GetWorld(), posX + collision->x, posY + collision->y, tile->id, collision->content.shape);
            break;
        }
        collision = collision->next;
    }
}

LayerRenderTexture *InitLayerRenderTexture(tmx_map *map, tmx_layer *layer, int tex_width, int tex_height) {
    LayerRenderTexture *layerTexture = (LayerRenderTexture *)malloc(sizeof(LayerRenderTexture));
    layerTexture->texture = LoadRenderTexture(tex_width, tex_height);
    BeginTextureMode(layerTexture->texture);
    ClearBackground(BLANK);
    DrawTMXLayer(map, layer, 0, 0, WHITE);
    EndTextureMode();
    if (layer->next) {
        layerTexture->next = InitLayerRenderTexture(map, layer->next, tex_width, tex_height);
    } else {
        layerTexture->next = NULL;
    }
    return layerTexture;
}

LayerRenderTexture *InitMap(const char *mapPath) {
    tmx_map *map = LoadTMX(mapPath);
    int tex_width = map->width * map->tile_width;
    int tex_height = map->height * map->tile_height;
    tmx_layer *layer = map->ly_head;
    LayerRenderTexture *layerTextures = InitLayerRenderTexture(map, layer, tex_width, tex_height);
    return layerTextures;
}