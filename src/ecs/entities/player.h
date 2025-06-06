#ifndef ECS_PLAYER_ENTITY_H
#define WCS_PLAYER_ENTITY_H

#include "box2d/box2d.h"
#include "flecs.h"
#include "raylib.h"

#include "constants.h"
#include "context.h"
#include "utils.h"

#include "../components/basic.h"
#include "../components/player.h"

#define ECS_PLAYER_ANIM_SYSTEM_IMPLEMENTATION
#include "../systems/player-anim.h"

ecs_entity_t CreatePlayerEntity(ecs_world_t *world);
b2BodyId InitPlayerSpawn(ecs_world_t *world, Vector2 position);
void FreePlayerEntity(ecs_world_t *world);

#endif // ECS_PLAYER_ENTITY_H

#ifdef ECS_PLAYER_ENTITY_IMPLEMENTATION
#ifndef ECS_PLAYER_ENTITY_IMPLEMENTATION_ONLY
#define ECS_PLAYER_ENTITY_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(PlayerSpawn);
extern ECS_COMPONENT_DECLARE(PlayerPhysics);
extern ECS_COMPONENT_DECLARE(PlayerAnimState);
extern ECS_COMPONENT_DECLARE(PlayerAnimController);
extern ECS_COMPONENT_DECLARE(TilemapDrawable);

void ResetPlayerPosition(ecs_world_t *world) {
    const PlayerSpawn *playerSpawn = ecs_get(world, GetPlayerEntity(), PlayerSpawn);
    const PlayerPhysics *pp = ecs_get(world, GetPlayerEntity(), PlayerPhysics);
    if (playerSpawn != NULL) {
        b2Body_SetTransform(pp->body, (b2Vec2){playerSpawn->x, playerSpawn->y}, (b2Rot){1.0f, 0.0f});
    }
}

ecs_entity_t CreatePlayerEntity(ecs_world_t *world) {
    if (!GetPlayerEntity()) {
        ecs_entity_t player = ecs_entity(world, {.name = "Player"});

        ecs_set(world, player, Velocity, {0.0f, 0.0f});
        ecs_set(world, player, PlayerAnimController, {.state = STATE_IDLE, .direction = DOWN, .locked = false, .prev_state = STATE_IDLE});
        ecs_set(world, player, PlayerAnimState, {0});

        return player;
    } else {
        return GetPlayerEntity();
    }
}

b2BodyId InitPlayerSpawn(ecs_world_t *world, Vector2 position) {
    ecs_set(world, GetPlayerEntity(), Position, {position.x, position.y});
    const Velocity *v = ecs_get(world, GetPlayerEntity(), Velocity);

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){position.x, position.y};

    b2Circle circle = {0.0f, 0.0f, 30.0f};
    b2BodyId body = b2CreateBody(GetPhyWorld(), &bodyDef);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;
    b2CreateCircleShape(body, &shapeDef, &circle);

    ecs_set(world, GetPlayerEntity(), PlayerPhysics, {.body = body});
    ecs_set(world, GetPlayerEntity(), PlayerSpawn, {position.x, position.y});
    ecs_set(world, GetPlayerEntity(), TilemapDrawable, {.sort_y = position.y + 15.0f, .layer_index = 4.0f, .is_dynamic = true, .render_fn = DrawPlayerSprite});
    GetCamera()->target = (Vector2){position.x, position.y};

    return body;
}

void LoadPlayerAnimClips() {
}

void FreePlayerEntity(ecs_world_t *world) {
    ecs_delete(world, GetPlayerEntity());
}

#endif // ECS_PLAYER_ENTITY_IMPLEMENTATION_ONLY
#endif // ECS_PLAYER_ENTITY_IMPLEMENTATION
