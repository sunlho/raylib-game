#ifndef TILEMAP_H
#define TILEMAP_H

#define TILEMAP_SCOPE tilemap

#define TilemapCom(path) \
    tilemap.##path

#include "flecs.h"
#include "raylib.h"
#include "tmx.h"

#ifndef STB_DS_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#endif // STB_DS_IMPLEMENTATION

#include "types/types.h"
#include "utils.h"

static ecs_world_t *tilemap_ecs_world = NULL;
static b2WorldId tilemap_physics_world = {0};
static const char *tilemap_collision_type = NULL;
static bool *tilemap_debug_mode = NULL;
static Camera2D *tilemap_world_camera = NULL;

#include "types/common.h"

#define TILEMAP_TMX_LOADER_IMPLEMENTATION
#include "tmx/loader.h"

#define TILEMAP_CAMERA_IMPLEMENTATION
#include "camera.h"

#define TILEMAP_COLLISION_IMPLEMENTATION
#include "collison.h"

#define TILEMAP_HANDLE_MAP_IMPLEMENTATION
#include "handle.h"

#define TILEMAP_RENDER_IMPLEMENTATION
#include "render.h"

#define Tilemap(path, ...) \
    TilemapInit(path, (TilemapOptions)__VA_ARGS__)

TilemapWorldSize TilemapInit(const char *path, TilemapOptions options);

#endif // TILEMAP_H

#ifdef TILEMAP_IMPLEMENTATION
#ifndef TILEMAP_IMPLEMENTATION_ONCE
#define TILEMAP_IMPLEMENTATION_ONCE

TilemapWorldSize TilemapInit(const char *path, TilemapOptions options) {
    tilemap_ecs_world = options.world;
    tilemap_physics_world = options.physics_world;
    tilemap_collision_type = options.collision_type;
    tilemap_debug_mode = options.debug_mode;
    tilemap_world_camera = options.camera;

    ECS_TAG(tilemap_ecs_world, TILEMAP_SCOPE);
    ecs_set_scope(tilemap_ecs_world, TILEMAP_SCOPE);

    TilemapRegisterCommonComponent();

    tmx_map *map = TilemapLoadTMX(path);
    TilemapHandleMap(map);
    TilemapWorldSize world_size = {
        .width = map->width * map->tile_width,
        .height = map->height * map->tile_height,
    };

    TilemapRegisterRenderSystem();

    return world_size;
}

#endif // TILEMAP_IMPLEMENTATION_ONCE
#endif // TILEMAP_IMPLEMENTATION
