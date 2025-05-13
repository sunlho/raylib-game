#ifndef ECS_COLLISION_ENTITY_H
#define ECS_COLLISION_ENTITY_H

#include "../components/basic.h"
#include "../components/tile-collider.h"
#include "constants.h"
#include "context.h"
#include "flecs.h"
#include "raylib.h"
#include "raymath.h"

void PolylineCollisionEntity(ecs_world_t *world, tmx_object *collision, double posX, double posY, int tileId, double scale);
ecs_entity_t GetCollisionEntityGroup();
void DrawCollisionRectangle();

#endif // ECS_COLLISION_ENTITY_H

#ifdef ECS_COLLISION_ENTITY_IMPLEMENTATION
#ifndef ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY
#define ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(TileCollider);
extern ECS_COMPONENT_DECLARE(PhysicsWorld);

ecs_entity_t GetCollisionEntityGroup() {
    ecs_entity_t group = ecs_lookup(GetWorld(), TILE_COLLIDER_GROUP);
    if (!group || !ecs_is_valid(GetWorld(), group)) {
        group = ecs_entity(GetWorld(), {.name = TILE_COLLIDER_GROUP});
    }
    return group;
}

void PolylineCollisionEntity(ecs_world_t *world, tmx_object *collision, double posX, double posY, int tileId, double scale) {
    tmx_shape *shape = collision->content.shape;
    if (shape->points_len == 1)
        return;
    ecs_entity_t group = GetCollisionEntityGroup();
    cpBody *staticBody = cpSpaceGetStaticBody(GetSpace());
    cpVect offset = cpvmult(cpv(collision->x, collision->y), scale);
    cpVect position = cpvadd(cpv(posX, posY), offset);
    for (int i = 0; i < shape->points_len - 1; i++) {
        cpVect p1 = cpvadd(cpvmult(cpv(shape->points[i][0], shape->points[i][1]), scale), position);
        cpVect p2 = cpvadd(cpvmult(cpv(shape->points[i + 1][0], shape->points[i + 1][1]), scale), position);

        cpShape *wall = cpSegmentShapeNew(staticBody, p1, p2, 2.0);
        cpShapeSetFriction(wall, 1.0);
        cpShapeSetElasticity(wall, 0.0);
        cpSpaceAddShape(GetSpace(), wall);

        ecs_entity_t collider = ecs_new(world);
        ecs_set(world, collider, TileCollider, {.tileId = tileId, .a = {p1.x, p1.y}, .b = {p2.x, p2.y}, .shape = wall});
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
            cpVect a = cpSegmentShapeGetA(tc->shape);
            cpVect b = cpSegmentShapeGetB(tc->shape);

            if (tc) {
                DrawLineEx((Vector2){a.x, a.y}, (Vector2){b.x, b.y}, 1, RED);
            }
        }
    }
}

#endif // ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY
#endif // ECS_COLLISION_ENTITY_IMPLEMENTATION
