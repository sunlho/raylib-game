#ifndef TILEMAP_ANIMATE_H
#define TILEMAP_ANIMATE_H

#include "flecs.h"
#include "raylib.h"
#include "tmx.h"

#include "utils.h"

#define MAX_QUEUE 65536

#endif // TILEMAP_ANIMATE_H

#define MAX_WIDTH 256
#define MAX_HEIGHT 256

#ifdef TILEMAP_ANIMATE_IMPLEMENTATION
#ifndef TILEMAP_ANIMATE_IMPLEMENTATION_ONCE
#define TILEMAP_ANIMATE_IMPLEMENTATION_ONCE

typedef struct Coord {
    unsigned int index;
} Coord;

void FloodFillGroup(tmx_tile *tile, unsigned int index, TilemapCtx *ctx) {
    static int group_counter = 0;
    if (tile->animation_len == 0 || tile->animation == NULL || tile->user_data.integer != 0)
        return;

    Coord queue[MAX_QUEUE];
    int q_start = 0, q_end = 0;
    queue[q_end++] = (Coord){index};

    tile->user_data = (tmx_user_data){.integer = group_counter};

    while (q_start < q_end) {
        Coord c = queue[q_start++];
        const Coord neighbors[4] = {
            index - 1,
            index + 1,
            index - ctx->map->width,
            index + ctx->map->width,
        };

        for (int i = 0; i < 4; i++) {
            if (neighbors[i].index < 0 || neighbors[i].index >= ctx->map->tilecount)
                continue;

            unsigned int base_gid = ctx->layer->content.gids[index];
            unsigned int gid = base_gid & TMX_FLIP_BITS_REMOVAL;

            tmx_tile *new_tile = ctx->map->tiles[neighbors[i].index];
        }
    }

    group_counter++;
}

#endif // TILEMAP_ANIMATE_IMPLEMENTATION_ONCE
#endif // TILEMAP_ANIMATE_IMPLEMENTATION
