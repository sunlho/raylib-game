
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
            InitPlayerPosition(GetWorld(), (Vector2){obj->x - (playerTag->texture.width / 36), obj->y - (playerTag->texture.height / 6)});
        }
    }
}

void DrawTmxTileCollisionFunc(tmx_tile *tile, int posX, int posY) {
    tmx_object *collision = tile->collision;
    while (collision) {
        CreateCollisionEntity(GetWorld(), posX + collision->x, posY + collision->y, tile->id, collision->content.shape);
        collision = collision->next;
    }
}

RenderTexture2D InitMap(const char *mapPath) {
    tmx_map *map = LoadTMX(mapPath);
    if (map == NULL) {
        fprintf(stderr, "Failed to load map: %s\n", mapPath);
        return (RenderTexture2D){0};
    }

    int tex_width = map->width * map->tile_width;
    int tex_height = map->height * map->tile_height;

    RenderTexture2D mapTexture = LoadRenderTexture(tex_width, tex_height);

    BeginTextureMode(mapTexture);
    DrawTMX(map, 0, 0, RAYWHITE);
    DrawCollisionRectangle();
    EndTextureMode();
    return mapTexture;
}