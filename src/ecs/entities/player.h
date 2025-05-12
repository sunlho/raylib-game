#ifndef ECS_PLAYER_ENTITY_H
#define WCS_PLAYER_ENTITY_H

#include "../components/basic.h"
#include "../components/physics.h"
#include "../components/player.h"
#include "../components/tile-collider.h"
#include "chipmunk/chipmunk.h"
#include "chipmunk/chipmunk_private.h"
#include "constants.h"
#include "context.h"
#include "flecs.h"
#include "raylib.h"
#include "utils.h"

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
extern ECS_COMPONENT_DECLARE(PlayerData);
extern ECS_COMPONENT_DECLARE(PlayerSpawn);
extern ECS_COMPONENT_DECLARE(PlayerPhysics);
extern ECS_COMPONENT_DECLARE(PhysicsWorld);

static int framesCounter = 0;
static int framesSpeed = 9;
static int currentFrame = 0;

PlayerDirection GetDirection(Vector2 move) {
    float x = move.x, y = move.y;
    if (x < 0)
        return (y < 0) ? LEFT_UP : (y > 0) ? LEFT_DOWN
                                           : LEFT;
    if (x > 0)
        return (y < 0) ? RIGHT_UP : (y > 0) ? RIGHT_DOWN
                                            : RIGHT;
    return (y < 0) ? UP : (y > 0) ? DOWN
                                  : NONE;
}

void UpdatePlayerFrameRec(PlayerData *playerData, int currentFrame) {
    if (playerData->direction == LEFT || playerData->direction == LEFT_UP || playerData->direction == LEFT_DOWN) {
        playerData->frameRect.width = -playerData->width;
    } else if (playerData->direction == RIGHT || playerData->direction == RIGHT_UP || playerData->direction == RIGHT_DOWN) {
        playerData->frameRect.width = playerData->width;
    }
    playerData->frameRect.x = playerData->width * currentFrame;
}

void ResetPlayerPosition(ecs_world_t *world) {
    const PlayerSpawn *playerSpawn = ecs_get(world, GetPlayerEntity(), PlayerSpawn);
    const PlayerPhysics *pp = ecs_get(world, GetPlayerEntity(), PlayerPhysics);
    if (playerSpawn != NULL) {
        cpBodySetPosition(pp->body, cpv(playerSpawn->x, playerSpawn->y));
    }
}

void DrawShapeCallback(cpBody *body, cpShape *shape, void *data) {
    cpCircleShape *circle = (cpCircleShape *)shape;
    cpVect pos = cpBodyLocalToWorld(body, circle->c);
    float radius = circle->r;
    DrawCircle((int)pos.x, (int)pos.y, radius, RED);
}

void DrawPlayer(ecs_world_t *world) {
    PlayerData *playerData = ecs_get_mut(world, GetPlayerEntity(), PlayerData);
    const Position *p = ecs_get(world, GetPlayerEntity(), Position);
    Vector2 move = GetMovementInput(PLAYER_MOVE_SPEED);
    ecs_set(world, GetPlayerEntity(), Velocity, {move.x, move.y});
    playerData->direction = GetDirection(move);
    if (IsKeyPressed(KEY_R)) {
        ResetPlayerPosition(world);
    }

    bool isMoving = (move.x != 0.0f || move.y != 0.0f);
    if (isMoving) {
        framesCounter++;
        if (framesCounter >= (FPS / framesSpeed)) {
            framesCounter = 0;
            currentFrame = (currentFrame + 1) % 6;
            UpdatePlayerFrameRec(playerData, currentFrame);
        }
    } else {
        framesCounter = 0;
        currentFrame = 0;
        playerData->frameRect.x = 0.0f;
    }
    ecs_modified(world, GetPlayerEntity(), PlayerData);
    // const PlayerPhysics *pp = ecs_get(world, GetPlayerEntity(), PlayerPhysics);
    // cpBodyEachShape(pp->body, DrawShapeCallback, NULL);
    DrawTextureRec(playerData->texture, playerData->frameRect, (Vector2){p->x - playerData->width / 2, p->y - playerData->height / 2}, WHITE);
}

ecs_entity_t CreatePlayerEntity(ecs_world_t *world) {
    if (!GetPlayerEntity()) {
        ecs_entity_t player = ecs_entity(world, {.name = "Player"});

        ecs_set(world, player, Velocity, {0.0f, 0.0f});

        Texture2D playerTexture = LoadTexture(GetAssetPath("scarfy.png"));
        playerTexture.width /= 4;
        playerTexture.height /= 4;
        Rectangle frameRect = {0.0f, 0.0f, playerTexture.width / 6.0f, (float)playerTexture.height};
        ecs_set(
            world,
            player,
            PlayerData,
            {
                .texture = playerTexture,
                .frameRect = frameRect,
                .width = frameRect.width,
                .height = frameRect.height,
                .direction = NONE,
            });
        return player;
    } else {
        return GetPlayerEntity();
    }
}

void InitPlayerPosition(ecs_world_t *world, Vector2 position) {
    position.x *= TILE_SCALE;
    position.y *= TILE_SCALE;
    ecs_set(world, GetPlayerEntity(), Position, {position.x, position.y});
    const Velocity *v = ecs_get(world, GetPlayerEntity(), Velocity);
    cpFloat mass = 1.0f;
    cpFloat radius = 25.0f;
    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

    cpBody *body = cpBodyNew(mass, moment);
    cpBodySetPosition(body, cpv(position.x, position.y));
    cpSpaceAddBody(GetSpace(), body);

    cpShape *shape = cpCircleShapeNew(body, radius, cpvzero);
    cpShapeSetElasticity(shape, 0.0);
    cpShapeSetFriction(shape, 1.0);
    cpSpaceAddShape(GetSpace(), shape);

    ecs_set(world, GetPlayerEntity(), PlayerPhysics, {.body = body, .shape = shape});
    ecs_set(world, GetPlayerEntity(), PlayerSpawn, {position.x, position.y});
}

void FreePlayerEntity(ecs_world_t *world) {
    const PlayerData *playerData = ecs_get(world, GetPlayerEntity(), PlayerData);
    if (playerData != NULL) {
        UnloadTexture(playerData->texture);
    }
    ecs_delete(world, GetPlayerEntity());
}

#endif // ECS_PLAYER_ENTITY_IMPLEMENTATION_ONLY
#endif // ECS_PLAYER_ENTITY_IMPLEMENTATION