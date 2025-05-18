#ifndef ECS_PLAYER_ENTITY_H
#define WCS_PLAYER_ENTITY_H

#include "../components/basic.h"
#include "../components/player.h"
#include "box2d/box2d.h"
#include "constants.h"
#include "context.h"
#include "flecs.h"
#include "raylib.h"
#include "utils.h"

void DrawPlayer(ecs_world_t *world);
ecs_entity_t CreatePlayerEntity(ecs_world_t *world);
void InitPlayerPosition(ecs_world_t *world, Vector2 position, double scale);
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
        b2Body_SetTransform(pp->body, (b2Vec2){playerSpawn->x, playerSpawn->y}, (b2Rot){0, 0});
    }
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

    DrawTextureRec(playerData->texture, playerData->frameRect, (Vector2){p->x - playerData->width / 2, p->y - playerData->height / 2}, WHITE);

    const PlayerPhysics *pp = ecs_get(world, GetPlayerEntity(), PlayerPhysics);
    b2ShapeId shapeIds[1];
    int returnCount = b2Body_GetShapes(pp->body, shapeIds, 1);
    b2ShapeId shapeId = shapeIds[0];
    b2Circle c = b2Shape_GetCircle(shapeId);
    b2Vec2 center = b2Body_GetWorldPoint(pp->body, c.center);
    DrawCircleLines(center.x, center.y, c.radius, BLUE);
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

void InitPlayerPosition(ecs_world_t *world, Vector2 position, double scale) {
    position.x *= scale;
    position.y *= scale;
    ecs_set(world, GetPlayerEntity(), Position, {position.x, position.y});
    const Velocity *v = ecs_get(world, GetPlayerEntity(), Velocity);

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = (b2Vec2){position.x, position.y};

    b2Circle circle = {0.0f, 0.0f, 16.0f};
    b2BodyId body = b2CreateBody(GetPhyWorld(), &bodyDef);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;
    b2CreateCircleShape(body, &shapeDef, &circle);

    ecs_set(world, GetPlayerEntity(), PlayerPhysics, {.body = body});
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