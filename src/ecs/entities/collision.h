#ifndef ECS_COLLISION_ENTITY_H
#define ECS_COLLISION_ENTITY_H

#include "../components/basic.h"
#include "../components/tile-collider.h"
#include "constants.h"
#include "context.h"
#include <flecs.h>
#include <raylib.h>

ecs_entity_t CreateCollisionEntity(ecs_world_t *world, Vector2 position, Vector2 points[8]);
ecs_entity_t GetCollisionEntityGroup();

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

ecs_entity_t CreateCollisionEntity(ecs_world_t *world, Vector2 position, Vector2 points[8]) {
    ecs_entity_t collider = ecs_new(world);
    ecs_add(world, collider, Position);
    ecs_set(world, collider, Position, {position.x, position.y});
    ecs_add(world, collider, TileCollider);
    ecs_set(world, collider, TileCollider, {position.x, position.y, points});
    ecs_entity_t group = GetCollisionEntityGroup();
    if (group) {
        ecs_add_pair(world, collider, EcsChildOf, group);
    }
    return collider;
}

#endif // ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY
#endif // ECS_COLLISION_ENTITY_IMPLEMENTATION
