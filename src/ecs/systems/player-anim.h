#ifndef ECS_PLAYER_ANIM_SYSTEM_H
#define ECS_PLAYER_ANIM_SYSTEM_H

#include "flecs.h"

void PlayerAnimImport(ecs_world_t *world);

#endif // ECS_PLAYER_ANIM_SYSTEM_H

#ifdef ECS_PLAYER_ANIM_SYSTEM_IMPLEMENTATION
#ifndef ECS_PLAYER_ANIM_SYSTEM_IMPLEMENTATION_ONLY
#define ECS_PLAYER_ANIM_SYSTEM_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(PlayerPhysics);
extern ECS_COMPONENT_DECLARE(PlayerAnimState);
extern ECS_COMPONENT_DECLARE(PlayerAnimController);

static void DrawPlayerSprite(ecs_world_t *world, ecs_entity_t entity) {
    // const PlayerAnimController *ctrl = ecs_get(world, entity, PlayerAnimController);
    // const PlayerAnimState *state = ecs_get(world, entity, PlayerAnimState);
    const PlayerPhysics *pp = ecs_get(world, entity, PlayerPhysics);
    // const Position *p = ecs_get(world, entity, Position);
}

static void UpdatePlayerAnim(ecs_iter_t *it) {
    PlayerAnimController *ctrl = ecs_field(it, PlayerAnimController, 0);
    PlayerAnimState *state = ecs_field(it, PlayerAnimState, 1);

    for (int i = 0; i < it->count; i++) {
        PlayerAnimClip *clip = NULL;

        // state[i].elapsed += GetFrameTime();
        // if (state[i].elapsed >= clip->frame_time) {
        //     state[i].elapsed = 0;
        //     state[i].current_frame++;

        //     if (state[i].current_frame >= clip->total_frames) {
        //         if (clip->loop) {
        //             state[i].current_frame = 0;
        //         } else {
        //             state[i].current_frame = clip->total_frames - 1;
        //             if (ctrl[i].state == STATE_SLASH) {
        //                 ctrl[i].locked = false;
        //                 ctrl[i].state = ctrl[i].prev_state;
        //             }
        //         }
        //     }
        // }
    }
}

void PlayerAnimImport(ecs_world_t *world) {
    ECS_MODULE(world, PlayerAnim);
    // ECS_SYSTEM(world, UpdatePlayerAnim, EcsOnUpdate, PlayerAnimController, PlayerAnimState);
    // ECS_SYSTEM(world, DrawPlayerSprite, EcsOnUpdate, PlayerAnimController, PlayerAnimState, PlayerPhysics, Position);
}

#endif // ECS_PLAYER_ANIM_SYSTEM_IMPLEMENTATION_ONLY
#endif // ECS_PLAYER_ANIM_SYSTEM_IMPLEMENTATION
