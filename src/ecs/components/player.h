#ifndef ECS_PLAYER_COMPONENT_H
#define ECS_PLAYER_COMPONENT_H

#include "box2d/box2d.h"
#include "flecs.h"
#include "raylib.h"

typedef enum {
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3
} PlayerDirection;

typedef struct {
    int x;
    int y;
} PlayerSpawn;

typedef struct {
    b2BodyId body;
} PlayerPhysics;

typedef struct {
    Texture2D texture;
    int frame_width;
    int frame_height;
    int total_frames;
    int columns;
    float frame_time;
    bool loop;
} PlayerAnimClip;

typedef struct {
    PlayerAnimClip *clip;
    int current_frame;
    float timer;
    float elapsed;
} PlayerAnimState;

typedef enum {
    STATE_IDLE,
    STATE_WALK,
    STATE_SLASH
} PlayerState;

typedef struct {
    PlayerState state;
    PlayerState prev_state;
    PlayerDirection direction;
    bool locked;
} PlayerAnimController;

#endif // ECS_PLAYER_COMPONENT_H