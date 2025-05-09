
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
extern ECS_COMPONENT_DECLARE(PlayerTag);
extern ECS_COMPONENT_DECLARE(TileCollider);

void DrawTMXLayerObjectFunc(tmx_map *map, tmx_object *obj, int posX, int posY, Color tint) {
    if (obj->type) {
        if (strcmp(obj->type, "start") == 0) {
            const PlayerTag *playerTag = ecs_get(GetWorld(), GetPlayerEntity(), PlayerTag);
            InitPlayerPosition(GetWorld(), (Vector2){obj->x - (playerTag->texture.width / 36), obj->y - (playerTag->texture.height / 6)});
        }
    }
}

void DrawTmxTileCollisionFunc(tmx_object *collision, int posX, int posY) {
    Vector2 points[8];
    int pointCount = 0;
    while (collision) {
        points[pointCount].x = collision->x;
        points[pointCount].y = collision->y;
        pointCount++;
        collision = collision->next;
    }
    CreateCollisionEntity(GetWorld(), (Vector2){posX, posY}, points);
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
    EndTextureMode();
    return mapTexture;
}