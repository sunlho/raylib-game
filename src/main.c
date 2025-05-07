#include "utils.h"
#include "raylib.h"
#include "tmx.h"
#define RAYLIB_TMX_IMPLEMENTATION
#include <stdio.h>
#include <flecs.h>
#include "tmx-loader.h"

typedef struct
{
    ecs_world_t *world;
    ecs_entity_t player;
} GameContext;

GameContext global_ctx;

typedef struct
{
    float x, y;
} Position, Velocity;

typedef struct
{
    int tile_x, tile_y;
    Vector2 *points[8];
} TileCollider;

typedef struct
{
    Texture2D texture;
} PlayerTag;

void Move(ecs_iter_t *it)
{
    Position *p = ecs_field(it, Position, 0);
    Velocity *v = ecs_field(it, Velocity, 1);

    for (int i = 0; i < it->count; i++)
    {
        p[i].x += v[i].x;
        p[i].y += v[i].y;
    }
}

void DrawTMXLayerObject(tmx_map *map, tmx_object *obj, int posX, int posY, Color tint)
{
    if (obj->type)
    {
        if (strcmp(obj->type, "start") == 0)
        {
            ecs_world_t *world = global_ctx.world;
            ECS_COMPONENT(world, Position);
            ECS_COMPONENT(world, PlayerTag);
            ecs_entity_t player = global_ctx.player;
            const PlayerTag *playerTag = ecs_get(world, player, PlayerTag);
            ecs_set(world, player, Position, {obj->x - (playerTag->texture.width / 36), obj->y - (playerTag->texture.height / 6)});
        }
    }
}

void DrawTmxTileCollision(tmx_object *collision, int posX, int posY)
{
    Vector2 points[8];
    int pointCount = 0;

    ecs_entity_t collider = ecs_new(global_ctx.world);
    ECS_COMPONENT(global_ctx.world, Position);
    ECS_COMPONENT(global_ctx.world, TileCollider);
    ecs_add(global_ctx.world, collider, Position);
    ecs_set(global_ctx.world, collider, Position, {posX, posY});
    ecs_add(global_ctx.world, collider, TileCollider);

    while (collision)
    {

        points[pointCount].x = collision->x;
        points[pointCount].y = collision->y;
        pointCount++;
        collision = collision->next;
    }

    ecs_set(global_ctx.world, collider, TileCollider, {posX, posY, points});
}

int main(void)
{
    const int screenWidth = 925;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "[raylib-tmx] example");
    SetTargetFPS(60);

    DrawTMXLayerObjectFunc = DrawTMXLayerObject;
    DrawTmxTileCollisionFunc = DrawTmxTileCollision;

    ecs_world_t *world = ecs_init();
    global_ctx.world = world;
    ECS_COMPONENT(world, PlayerTag);
    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Velocity);
    ECS_COMPONENT(world, TileCollider);
    ECS_SYSTEM(world, Move, EcsOnUpdate, Position, Velocity);

    ecs_entity_t player = ecs_new(world);
    global_ctx.player = player;

    ecs_set(world, player, Velocity, {0.0f, 0.0f});
    ecs_add(world, player, PlayerTag);

    Texture2D playerTexture = LoadTexture(get_asset_path("scarfy.png"));
    ecs_set(world, player, PlayerTag, {playerTexture});

    playerTexture.width /= 3;
    playerTexture.height /= 3;
    Rectangle frameRec = {0.0f, 0.0f, (float)playerTexture.width / 6, (float)playerTexture.height};
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 9;

    float moveSpeed = 3.5f;
    bool facingRight = true;

    tmx_map *map = LoadTMX(get_asset_path("island.tmx"));

    if (!map)
    {
        TraceLog(LOG_ERROR, "Failed to load map: %s", get_asset_path("island.tmx"));
        return 1;
    }

    int tex_width = map->width * map->tile_width;
    int tex_height = map->height * map->tile_height;

    RenderTexture2D mapTexture = LoadRenderTexture(tex_width, tex_height);

    BeginTextureMode(mapTexture);
    DrawTMX(map, 0, 0, RAYWHITE);
    EndTextureMode();

    while (!WindowShouldClose() && ecs_progress(world, GetFrameTime()))
    {
        Vector2 move = get_movement_input(moveSpeed);
        ecs_set(world, player, Velocity, {move.x, move.y});
        bool isMoving = (move.x != 0.0f || move.y != 0.0f);

        if (move.x > 0)
            facingRight = true;
        else if (move.x < 0)
            facingRight = false;

        const Position *p = ecs_get(world, player, Position);
        float x = p->x;
        float y = p->y;
        if (x < 0)
            x = 0;
        if (y < 0)
            y = 0;
        if (x + frameRec.width > screenWidth)
            x = screenWidth - frameRec.width;
        if (y + frameRec.height > screenHeight)
            y = screenHeight - frameRec.height;
        ecs_set(world, player, Position, {x, y});

        if (isMoving)
        {
            framesCounter++;
            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                currentFrame = (currentFrame + 1) % 6;
                frameRec.x = (float)currentFrame * frameRec.width;
            }

            ecs_query_desc_t qd = {0};
            qd.terms[0].id = ecs_id(TileCollider);
            ecs_query_t *query = ecs_query_init(world, &qd);
            ecs_iter_t it = ecs_query_iter(world, query);
            while (ecs_query_next(&it))
            {
                TileCollider *collider = ecs_field(&it, TileCollider, 0);
            }
        }
        else
        {
            currentFrame = 0;
            framesCounter = 0;
            frameRec.x = 0;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRenderTextureFixed(mapTexture, 0, 0, WHITE);
        if (facingRight)
        {
            DrawTextureRec(playerTexture, frameRec, (Vector2){p->x, p->y}, WHITE);
        }
        else
        {
            Rectangle flipped = frameRec;
            flipped.width = -frameRec.width;
            flipped.x += frameRec.width;
            DrawTextureRec(playerTexture, flipped, (Vector2){p->x, p->y}, WHITE);
        }

        EndDrawing();
    }

    ecs_fini(world);
    UnloadTexture(playerTexture);
    CloseWindow();

    return 0;
}