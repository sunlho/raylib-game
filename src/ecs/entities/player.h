#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include "../components/basic.h"
#include "../components/player.h"
#include "constants.h"
#include "context.h"
#include "utils.h"
#include <flecs.h>
#include <raylib.h>

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(PlayerTag);

static inline ecs_entity_t GetPlayer() { return global_ctx.player; }
static int framesCounter = 0;
static int framesSpeed = 9;
static int currentFrame = 0;

static inline PlayerDirection GetDirection(Vector2 move) {
    float x = move.x, y = move.y;
    if (x < 0)
        return (y < 0) ? LEFT_UP : (y > 0) ? LEFT_DOWN : LEFT;
    if (x > 0)
        return (y < 0) ? RIGHT_UP : (y > 0) ? RIGHT_DOWN : RIGHT;
    return (y < 0) ? UP : (y > 0) ? DOWN : NONE;
}

static inline void UpdatePlayerFrameRec(PlayerTag *playerTag, int currentFrame, float frameWidth) {
    if (playerTag->direction == LEFT || playerTag->direction == LEFT_UP || playerTag->direction == LEFT_DOWN) {
        playerTag->frameRec.width = -frameWidth;
    } else if (playerTag->direction == RIGHT || playerTag->direction == RIGHT_UP || playerTag->direction == RIGHT_DOWN) {
        playerTag->frameRec.width = frameWidth;
    }
    playerTag->frameRec.x += frameWidth;
}

static inline void DrawPlayer(ecs_world_t *world) {
    PlayerTag *playerTag = ecs_get_mut(world, global_ctx.player, PlayerTag);
    const Position *p = ecs_get(world, global_ctx.player, Position);
    Vector2 move = GetMovementInput(PLAYER_MOVE_SPEED);
    ecs_set(world, global_ctx.player, Velocity, {move.x, move.y});
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
        ecs_set(world, global_ctx.player, Position, {x, y});

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
    ecs_modified(world, global_ctx.player, PlayerTag);

    DrawTextureRec(playerTag->texture, playerTag->frameRec, (Vector2){p->x, p->y}, WHITE);
}

static inline ecs_entity_t CreatePlayerEntity(ecs_world_t *world) {
    if (!global_ctx.player) {
        global_ctx.player = ecs_entity(world, {.name = "Player"});

        ecs_add(world, global_ctx.player, Position);
        ecs_add(world, global_ctx.player, Velocity);
        ecs_add(world, global_ctx.player, PlayerTag);
        ecs_set(world, global_ctx.player, Velocity, {0.0f, 0.0f});

        Texture2D playerTexture = LoadTexture(GetAssetPath("scarfy.png"));
        playerTexture.width /= 3;
        playerTexture.height /= 3;
        Rectangle frameRec = {0.0f, 0.0f, playerTexture.width / 6.0f, (float)playerTexture.height};
        ecs_set(world, global_ctx.player, PlayerTag, {.texture = playerTexture, .frameRec = frameRec});
        return global_ctx.player;
    } else {
        return global_ctx.player;
    }
}

static inline void InitPlayerPosition(ecs_world_t *world, Vector2 position) { ecs_set(world, global_ctx.player, Position, {position.x, position.y}); }

static inline void FreePlayerEntity(ecs_world_t *world) {
    const PlayerTag *playerTag = ecs_get(world, global_ctx.player, PlayerTag);
    if (playerTag != NULL) {
        UnloadTexture(playerTag->texture);
    }
    ecs_delete(world, global_ctx.player);
}

#endif // PLAYER_ENTITY_H