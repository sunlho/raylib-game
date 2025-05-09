#ifndef ECS_PLAYER_COMPONENT_H
#define ECS_PLAYER_COMPONENT_H

#include <flecs.h>
#include <raylib.h>

typedef enum PlayerDirection { LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN, UP, DOWN, LEFT, RIGHT, NONE } PlayerDirection;

typedef struct {
    Texture2D texture;
    float width;
    float height;
    Rectangle frameRec;
    PlayerDirection direction;
    bool isColliding;
} PlayerData;

#endif // ECS_PLAYER_COMPONENT_H