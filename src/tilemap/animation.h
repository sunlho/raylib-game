#ifndef TILEMAP_ANIMATION_H
#define TILEMAP_ANIMATION_H

#endif // TILEMAP_ANIMATION_H

#ifdef TILEMAP_ANIMATION_IMPLEMENTATION
#ifndef TILEMAP_ANIMATION_IMPLEMENTATION_ONCE
#define TILEMAP_ANIMATION_IMPLEMENTATION_ONCE

void TilemapAnimateTileSystem(ecs_iter_t *it) {
    float now = GetTime();
    TilemapChunk *chunks = ecs_field(it, TilemapChunk, 0);

    for (int i = 0; i < it->count; i++) {
        TilemapChunk *chunk = &chunks[i];
        if (!chunk->anim_tiles)
            continue;

        TilemapChunkAnimTile *anim_tile = chunk->anim_tiles;
        while (anim_tile) {
            if (!anim_tile->start_time) {
                anim_tile->start_time = now;
            }

            float t = (now - anim_tile->start_time) * 1000.0f;
            int frame_index = (int)(t / anim_tile->animation[anim_tile->current_frame].duration);
            frame_index %= anim_tile->animation_len;

            anim_tile->current_frame = frame_index;
            anim_tile->tile->tile_gid = (anim_tile->animation[frame_index].tile_id + anim_tile->first_gid) & TMX_FLIP_BITS_REMOVAL;

            anim_tile = anim_tile->next;
        }
    }
}

ecs_entity_t TilemapRegisterAnimationSystem() {
    ECS_SYSTEM(tilemap_ecs_world, TilemapAnimateTileSystem, EcsPreUpdate, TilemapChunk, TilemapInView);
    return ecs_id(TilemapAnimateTileSystem);
}

#endif // TILEMAP_ANIMATION_IMPLEMENTATION_ONCE
#endif // TILEMAP_ANIMATION_IMPLEMENTATION
