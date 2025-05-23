#ifndef INCLUDE_TMX_LOADER_H
#define INCLUDE_TMX_LOADER_H

#include "raylib.h"
#include "tmx.h"

typedef struct LayerRenderTexture {
    RenderTexture2D texture;
    struct LayerRenderTexture *next;
} LayerRenderTexture;

typedef struct TMXRenderContext {
    tmx_map *map;
    double posX;
    double posY;
    Color tint;
} TMXRenderContext;

tmx_map *LoadTMX(const char *fileName);
void UnloadTMX(tmx_map *map);

Color ColorFromTMX(uint32_t color);
void DrawTMXLayer(tmx_layer *layer, TMXRenderContext *ctx);
void DrawTMXTile(tmx_tile *tile, bool onlyDraw, TMXRenderContext *ctx);

LayerRenderTexture *InitMap(const char *mapPath, TMXRenderContext *ctx);

void DrawTMXLayerObjectCallBack(tmx_object *obj, TMXRenderContext *ctx);
bool DrawTMXTileCallBack(tmx_tile *tile, TMXRenderContext *ctx);

#endif // INCLUDE_TMX_LOADER_H

#ifdef TMX_LOADER_IMPLEMENTATION
#ifndef TMX_LOADER_IMPLEMENTATION_ONCE
#define TMX_LOADER_IMPLEMENTATION_ONCE

Color ColorFromTMX(uint32_t color) {
    tmx_col_bytes res = tmx_col_to_bytes(color);
    return *((Color *)&res);
}

void *LoadTMXImage(const char *fileName) {
    Texture2D *texture = MemAlloc(sizeof(Texture2D));
    *texture = LoadTexture(fileName);
    SetTextureFilter(*texture, TEXTURE_FILTER_POINT);
    return texture;
}

void UnloadTMXImage(void *ptr) {
    if (ptr != NULL) {
        UnloadTexture(*((Texture2D *)ptr));
        MemFree(ptr);
    }
}

void *MemReallocTMX(void *address, size_t len) {
    return MemRealloc(address, (int)len);
}

tmx_map *LoadTMX(const char *fileName) {
    tmx_alloc_func = MemReallocTMX;
    tmx_free_func = MemFree;
    tmx_img_load_func = LoadTMXImage;
    tmx_img_free_func = UnloadTMXImage;
    tmx_map *map = tmx_load(fileName);
    if (!map) {
        TraceLog(LOG_ERROR, "TMX: Failed to load TMX file %s", fileName);
        return NULL;
    }
    return map;
}

void UnloadTMX(tmx_map *map) {
    if (map) {
        tmx_map_free(map);
        TraceLog(LOG_INFO, "TMX: Unloaded map");
    }
}

void DrawTMXLayerImage(tmx_image *image, TMXRenderContext *ctx) {
    if (image->resource_image) {
        Texture2D *texture = (Texture2D *)image->resource_image;
        DrawTexture(*texture, ctx->posX, ctx->posY, ctx->tint);
    }
}

void DrawTMXLayerObjects(tmx_object_group *objgr, TMXRenderContext *ctx) {
    tmx_object *head = objgr->head;
    Color color = ColorFromTMX(objgr->color);
    while (head) {
        if (head->visible) {
            if (DrawTMXLayerObjectCallBack) {
                DrawTMXLayerObjectCallBack(head, ctx);
            }
            if (head->obj_type == OT_TILE) {
                tmx_tile *tile = ctx->map->tiles[head->content.gid];
                if (tile != NULL) {
                    Rectangle dest = (Rectangle){(float)(ctx->posX + head->x), (float)(ctx->posY + head->y), (float)head->width, (float)head->height};
                    TMXRenderContext _ctx = {ctx->map, dest.x, dest.y, ctx->tint};
                    DrawTMXTile(tile, false, &_ctx);
                }
            }
            head = head->next;
        }
    }
}

void DrawTMXTile(tmx_tile *tile, bool onlyDraw, TMXRenderContext *ctx) {
    if (DrawTMXTileCallBack && !onlyDraw) {
        bool next = DrawTMXTileCallBack(tile, ctx);
        if (!next) {
            return;
        }
    }
    tmx_image *im = tile->image ? tile->image : tile->tileset->image;
    if (im && im->resource_image) {
        Texture *image = (Texture *)im->resource_image;
        if (image) {
            Rectangle srcRect = {tile->ul_x, tile->ul_y, tile->tileset->tile_width, tile->tileset->tile_height};
            Rectangle destRect = {
                ctx->posX,
                ctx->posY,
                srcRect.width,
                srcRect.height,
            };
            DrawTexturePro(*image, srcRect, destRect, (Vector2){0, 0}, 0, ctx->tint);
        }
    }
}

typedef struct {
    int startX, endX, stepX;
    int startY, endY, stepY;
} TileIter;

TileIter GetTileIterationParams(tmx_map *map) {
    int w = map->width;
    int h = map->height;

    switch (map->renderorder) {
    case R_RIGHTDOWN:
    case R_NONE:
        return (TileIter){0, w, 1, 0, h, 1};
    case R_RIGHTUP:
        return (TileIter){0, w, 1, h - 1, -1, -1};
    case R_LEFTDOWN:
        return (TileIter){w - 1, -1, -1, 0, h, 1};
    case R_LEFTUP:
        return (TileIter){w - 1, -1, -1, h - 1, -1, -1};
    default:
        return (TileIter){0, w, 1, 0, h, 1};
    }
}

void DrawTMXLayerTiles(tmx_layer *layer, TMXRenderContext *ctx) {
    Color newTint = ColorAlpha(ctx->tint, (float)layer->opacity);
    TileIter iter = GetTileIterationParams(ctx->map);

    for (int y = iter.startY; y != iter.endY; y += iter.stepY) {
        for (int x = iter.startX; x != iter.endX; x += iter.stepX) {
            unsigned int baseGid = layer->content.gids[(y * ctx->map->width) + x];
            unsigned int gid = baseGid & TMX_FLIP_BITS_REMOVAL;

            if (ctx->map->tiles[gid]) {
                tmx_tileset *ts = ctx->map->tiles[gid]->tileset;
                double drawX = ctx->posX + x * ts->tile_width;
                double drawY = ctx->posY + y * ts->tile_height;
                TMXRenderContext _ctx = {ctx->map, drawX, drawY, newTint};
                DrawTMXTile(ctx->map->tiles[gid], false, &_ctx);
            }
        }
    }
}

void DrawTMXLayer(tmx_layer *layer, TMXRenderContext *ctx) {
    switch (layer->type) {
    case L_OBJGR:
        DrawTMXLayerObjects(layer->content.objgr, ctx);
        break;
    case L_IMAGE:
        DrawTMXLayerImage(layer->content.image, ctx);
        break;
    case L_LAYER:
        DrawTMXLayerTiles(layer, ctx);
        break;
    case L_NONE:
        break;
    }
}

#endif // TMX_LOADER_IMPLEMENTATION_ONCE
#endif // TMX_LOADER_IMPLEMENTATION