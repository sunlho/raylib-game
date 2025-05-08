#ifndef COLLISION_ENTITY_H
#define COLLISION_ENTITY_H

#include <raylib.h>
#include <flecs.h>
#include "ecs/components/basic.h"
#include "ecs/components/tile-collider.h"

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(TileCollider);

static inline ecs_entity_t CreateCollisionEntity(ecs_world_t *world, Vector2 position, Vector2 points[8], ecs_entity_t parent)
{
  ecs_entity_t collider = ecs_new(world);
  ecs_add(world, collider, Position);
  ecs_set(world, collider, Position, {position.x, position.y});
  ecs_add(world, collider, TileCollider);
  ecs_set(world, collider, TileCollider, {position.x, position.y, points});
  if (parent)
  {
    ecs_add_pair(world, collider, EcsChildOf, parent);
  }
  return collider;
}

#endif // COLLISION_ENTITY_H