#ifndef ECS_PLAYER_COMPONENT_H
#define ECS_PLAYER_COMPONENT_H

#include <flecs.h>
#include <raylib.h>

typedef struct
{
    Texture2D texture;
} PlayerTag;

#endif // ECS_PLAYER_COMPONENT_H