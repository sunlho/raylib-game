#ifndef TILEMAP_TMX_LOADER_H
#define TILEMAP_TMX_LOADER_H

#include "raylib.h"
#include "tmx.h"

tmx_map *TilemapLoadTMX(const char *file_name);

#endif // TILEMAP_TMX_LOADER_H

#ifdef TILEMAP_TMX_LOADER_IMPLEMENTATION
#ifndef TILEMAP_TMX_LOADER_IMPLEMENTATION_ONCE
#define TILEMAP_TMX_LOADER_IMPLEMENTATION_ONCE

void *TilemapLoadTMXImage(const char *file_name) {
    Texture2D *texture = MemAlloc(sizeof(Texture2D));
    *texture = LoadTexture(file_name);
    SetTextureFilter(*texture, TEXTURE_FILTER_POINT);
    SetTextureWrap(*texture, TEXTURE_WRAP_CLAMP);
    texture->mipmaps = 1;
    return texture;
}

void TilemapUnloadTMXImage(void *ptr) {
    if (ptr != NULL) {
        UnloadTexture(*((Texture2D *)ptr));
        MemFree(ptr);
    }
}

void *TilemapMemReallocTMX(void *address, size_t len) {
    return MemRealloc(address, (int)len);
}

tmx_map *TilemapLoadTMX(const char *file_name) {
    tmx_alloc_func = TilemapMemReallocTMX;
    tmx_free_func = MemFree;
    tmx_img_load_func = TilemapLoadTMXImage;
    tmx_img_free_func = TilemapUnloadTMXImage;
    tmx_map *map = tmx_load(file_name);
    if (!map) {
        TraceLog(LOG_ERROR, "TMX: Failed to load TMX file %s", file_name);
        return NULL;
    }
    return map;
}

#endif // TILEMAP_TMX_LOADER_IMPLEMENTATION_ONCE
#endif // TILEMAP_TMX_LOADER_IMPLEMENTATION
