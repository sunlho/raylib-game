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

static tmx_map *tilemap_tmx_map = NULL;
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

TilemapInstance TilemapInit(const char *path, TilemapOptions options);

#endif // TILEMAP_H

#ifdef TILEMAP_IMPLEMENTATION
#ifndef TILEMAP_IMPLEMENTATION_ONCE
#define TILEMAP_IMPLEMENTATION_ONCE

TilemapInstance TilemapInit(const char *path, TilemapOptions options) {
    tilemap_ecs_world = options.world;
    tilemap_physics_world = options.physics_world;
    tilemap_collision_type = options.collision_type;
    tilemap_debug_mode = options.debug_mode;
    tilemap_world_camera = options.camera;

    ECS_TAG(tilemap_ecs_world, TILEMAP_SCOPE);
    ecs_set_scope(tilemap_ecs_world, TILEMAP_SCOPE);

    TilemapRegisterCommonComponent();

    tmx_map *map = TilemapLoadTMX(path);
    tilemap_tmx_map = map;
    TilemapHandleMap(map);

    TilemapRegisterRenderSystem();

    TilemapInstance instance = {
        .size = (TilemapWorldSize){
            .width = map->width * map->tile_width,
            .height = map->height * map->tile_height,
        },
        .map = map,
        .render = TilemapRenderManually,
    };
    return instance;
}

#endif // TILEMAP_IMPLEMENTATION_ONCE
#endif // TILEMAP_IMPLEMENTATION
