#ifndef ECS_PLAYER_ENTITY_H
#define WCS_PLAYER_ENTITY_H

#include "../components/basic.h"
#include "../components/player.h"
#include "constants.h"
#include "context.h"
#include "utils.h"
#include <flecs.h>
#include <raylib.h>

void DrawPlayer(ecs_world_t *world);
ecs_entity_t CreatePlayerEntity(ecs_world_t *world);
void InitPlayerPosition(ecs_world_t *world, Vector2 position);
void FreePlayerEntity(ecs_world_t *world);

#endif // ECS_PLAYER_ENTITY_H

#ifdef ECS_PLAYER_ENTITY_IMPLEMENTATION
#ifndef ECS_PLAYER_ENTITY_IMPLEMENTATION_ONLY
#define ECS_PLAYER_ENTITY_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(PlayerTag);

static int framesCounter = 0;
static int framesSpeed = 9;
static int currentFrame = 0;

PlayerDirection GetDirection(Vector2 move) {
    float x = move.x, y = move.y;
    if (x < 0)
        return (y < 0) ? LEFT_UP : (y > 0) ? LEFT_DOWN : LEFT;
    if (x > 0)
        return (y < 0) ? RIGHT_UP : (y > 0) ? RIGHT_DOWN : RIGHT;
    return (y < 0) ? UP : (y > 0) ? DOWN : NONE;
}

void UpdatePlayerFrameRec(PlayerTag *playerTag, int currentFrame, float frameWidth) {
    if (playerTag->direction == LEFT || playerTag->direction == LEFT_UP || playerTag->direction == LEFT_DOWN) {
        playerTag->frameRec.width = -frameWidth;
    } else if (playerTag->direction == RIGHT || playerTag->direction == RIGHT_UP || playerTag->direction == RIGHT_DOWN) {
        playerTag->frameRec.width = frameWidth;
    }
    playerTag->frameRec.x += frameWidth;
}

void DrawPlayer(ecs_world_t *world) {
    PlayerTag *playerTag = ecs_get_mut(world, GetPlayerEntity(), PlayerTag);
    const Position *p = ecs_get(world, GetPlayerEntity(), Position);
    Vector2 move = GetMovementInput(PLAYER_MOVE_SPEED);
    ecs_set(world, GetPlayerEntity(), Velocity, {move.x, move.y});
    playerTag->direction = GetDirection(move);

    bool isMoving = (move.x != 0.0f || move.y != 0.0f);
    if (isMoving) {
        float x = p->x;
        float y = p->y;
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;
        if (x + playerTag->frameRec.width > SCREEN_WIDTH)
            x = SCREEN_WIDTH - playerTag->frameRec.width;
        if (y + playerTag->frameRec.height > SCREEN_HEIGHT)
            y = SCREEN_HEIGHT - playerTag->frameRec.height;
        ecs_set(world, GetPlayerEntity(), Position, {x, y});

        framesCounter++;
        if (framesCounter >= (60 / framesSpeed)) {
            framesCounter = 0;
            currentFrame = (currentFrame + 1) % 6;
            UpdatePlayerFrameRec(playerTag, currentFrame, playerTag->texture.width / 6.0f);
        }

    } else {
        framesCounter = 0;
        currentFrame = 0;
    }
    ecs_modified(world, GetPlayerEntity(), PlayerTag);

    DrawTextureRec(playerTag->texture, playerTag->frameRec, (Vector2){p->x, p->y}, WHITE);
}

ecs_entity_t CreatePlayerEntity(ecs_world_t *world) {
    if (!GetPlayerEntity()) {
        ecs_entity_t player = ecs_entity(world, {.name = "Player"});

        ecs_add(world, player, Position);
        ecs_add(world, player, Velocity);
        ecs_add(world, player, PlayerTag);
        ecs_set(world, player, Velocity, {0.0f, 0.0f});

        Texture2D playerTexture = LoadTexture(GetAssetPath("scarfy.png"));
        playerTexture.width /= 3;
        playerTexture.height /= 3;
        Rectangle frameRec = {0.0f, 0.0f, playerTexture.width / 6.0f, (float)playerTexture.height};
        ecs_set(world, player, PlayerTag, {.texture = playerTexture, .frameRec = frameRec});
        return player;
    } else {
        return GetPlayerEntity();
    }
}

void InitPlayerPosition(ecs_world_t *world, Vector2 position) { ecs_set(world, GetPlayerEntity(), Position, {position.x, position.y}); }

void FreePlayerEntity(ecs_world_t *world) {
    const PlayerTag *playerTag = ecs_get(world, GetPlayerEntity(), PlayerTag);
    if (playerTag != NULL) {
        UnloadTexture(playerTag->texture);
    }
    ecs_delete(world, GetPlayerEntity());
}

#endif // ECS_PLAYER_ENTITY_IMPLEMENTATION_ONLY
#endif // ECS_PLAYER_ENTITY_IMPLEMENTATION