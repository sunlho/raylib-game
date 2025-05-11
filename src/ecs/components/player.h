#ifndef ECS_PLAYER_COMPONENT_H
#define ECS_PLAYER_COMPONENT_H

#include "flecs.h"
#include "raylib.h"

typedef enum {
    LEFT_UP,
    LEFT_DOWN,
    RIGHT_UP,
    RIGHT_DOWN,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
} PlayerDirection;

typedef struct {
    int x;
    int y;
} PlayerSpawn;

typedef struct {
    Texture2D texture;
    float width;
    float height;
    Rectangle frameRect;
    PlayerDirection direction;
} PlayerData;

typedef struct {
    cpShape *shape;
    cpBody *body;
} PlayerPhysics;

#endif // ECS_PLAYER_COMPONENT_H