#ifndef ECS_COLLISION_ENTITY_H
#define ECS_COLLISION_ENTITY_H

#include "../components/basic.h"
#include "../components/tile-collider.h"
#include "constants.h"
#include "context.h"
#include <flecs.h>
#include <raylib.h>
#include <raymath.h>

void CreateCollisionEntity(ecs_world_t *world, int posX, int posY, int tileId, tmx_shape *shape);
ecs_entity_t GetCollisionEntityGroup();
void DrawCollisionRectangle();

#endif // ECS_COLLISION_ENTITY_H

#ifdef ECS_COLLISION_ENTITY_IMPLEMENTATION
#ifndef ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY
#define ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(TileCollider);

ecs_entity_t GetCollisionEntityGroup() {
    ecs_entity_t group = ecs_lookup(GetWorld(), TILE_COLLIDER_GROUP);
    if (!group || !ecs_is_valid(GetWorld(), group)) {
        group = ecs_entity(GetWorld(), {.name = TILE_COLLIDER_GROUP});
    }
    return group;
}

void CreateCollisionEntity(ecs_world_t *world, int posX, int posY, int tileId, tmx_shape *shape) {
    if (shape->points_len == 1)
        return;
    ecs_entity_t group = GetCollisionEntityGroup();
    for (int i = 0; i < shape->points_len - 1; i++) {
        ecs_entity_t collider = ecs_new(world);
        ecs_add(world, collider, TileCollider);
        Vector2 a = {posX + shape->points[i][0], posY + shape->points[i][1]};
        Vector2 b = {posX + shape->points[(i + 1)][0], posY + shape->points[(i + 1)][1]};
        float length = Vector2Distance(a, b);
        Vector2 center = {(a.x + b.x) / 2, (a.y + b.y) / 2};
        Vector2 size = {length, 5.0f};
        Rectangle aabb = {
            center.x - size.x / 2,
            center.y - size.y / 2,
            size.x,
            size.y,
        };
        ecs_set(world, collider, TileCollider, {.tileId = tileId, .aabb = aabb, .a = a, .b = b});
        if (group) {
            ecs_add_pair(world, collider, EcsChildOf, group);
        }
    }
}

void DrawCollisionRectangle() {
    ecs_iter_t it = ecs_children(GetWorld(), GetCollisionEntityGroup());
    while (ecs_children_next(&it)) {
        for (int i = 0; i < it.count; i++) {
            ecs_entity_t child = it.entities[i];
            const TileCollider *tc = ecs_get(GetWorld(), child, TileCollider);
            if (tc) {
                DrawLineEx(tc->a, tc->b, 2.0f, RED);
            }
        }
    }
}

#endif // ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY
#endif // ECS_COLLISION_ENTITY_IMPLEMENTATION
