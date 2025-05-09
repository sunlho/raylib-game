#ifndef ECS_COLLISION_ENTITY_H
#define ECS_COLLISION_ENTITY_H

#include "../components/basic.h"
#include "../components/tile-collider.h"
#include "constants.h"
#include "context.h"
#include <flecs.h>
#include <raylib.h>

ecs_entity_t CreateCollisionEntity(ecs_world_t *world, int posX, int posY, int tileId, tmx_shape *shape);
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

ecs_entity_t CreateCollisionEntity(ecs_world_t *world, int posX, int posY, int tileId, tmx_shape *shape) {
    ecs_entity_t collider = ecs_new(world);
    ecs_add(world, collider, Position);
    ecs_set(world, collider, Position, {posX, posY});
    ecs_add(world, collider, TileCollider);
    ecs_set(world, collider, TileCollider, {.tileId = tileId, .pointCount = shape->points_len, .points = shape->points});
    ecs_entity_t group = GetCollisionEntityGroup();
    if (group) {
        ecs_add_pair(world, collider, EcsChildOf, group);
    }
    return collider;
}

void DrawCollisionRectangle() {
    ecs_iter_t it = ecs_children(GetWorld(), GetCollisionEntityGroup());
    while (ecs_children_next(&it)) {
        for (int i = 0; i < it.count; i++) {
            ecs_entity_t child = it.entities[i];
            const Position *p = ecs_get(GetWorld(), child, Position);
            const TileCollider *tc = ecs_get(GetWorld(), child, TileCollider);
            if (!p || !tc)
                continue;
            for (int j = 0; j < tc->pointCount; j++) {
                int next = (j + 1) % tc->pointCount;
                DrawLineEx(
                    (Vector2){p->x + tc->points[j][0], p->y + tc->points[j][1]},
                    (Vector2){p->x + tc->points[next][0], p->y + tc->points[next][1]},
                    2.0f,
                    RED);
            }
        }
    }
}

#endif // ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY
#endif // ECS_COLLISION_ENTITY_IMPLEMENTATION
