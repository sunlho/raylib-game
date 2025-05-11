#ifndef ECS_COLLISION_ENTITY_H
#define ECS_COLLISION_ENTITY_H

#include "../components/basic.h"
#include "../components/tile-collider.h"
#include "constants.h"
#include "context.h"
#include "flecs.h"
#include "raylib.h"
#include "raymath.h"

void PolylineCollisionEntity(ecs_world_t *world, int posX, int posY, int tileId, tmx_shape *shape);
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

void ExtendSegment(const cpVect a, const cpVect b, cpVect *prev, cpVect *next) {
    cpVect dir = cpvsub(b, a);

    cpFloat length = cpvlength(dir);
    if (length > 0) {
        cpVect unit_dir = cpvmult(dir, 1.0 / length);
        *prev = cpvsub(a, cpvmult(unit_dir, 1.0));
        *next = cpvadd(b, cpvmult(unit_dir, 1.0));
    } else {
        *prev = a;
        *next = b;
    }
}

void PolylineCollisionEntity(ecs_world_t *world, int posX, int posY, int tileId, tmx_shape *shape) {
    if (shape->points_len == 1)
        return;
    ecs_entity_t group = GetCollisionEntityGroup();
    cpBody *staticBody = cpSpaceGetStaticBody(GetSpace());
    for (int i = 0; i < shape->points_len - 1; i++) {
        ecs_entity_t collider = ecs_new(world);
        cpVect a = cpv(posX + shape->points[i][0], posY + shape->points[i][1]);
        cpVect b = cpv(posX + shape->points[(i + 1)][0], posY + shape->points[(i + 1)][1]);
        cpVect prev, next;
        ExtendSegment(a, b, &prev, &next);

        cpShape *wall = cpSegmentShapeNew(staticBody, prev, next, 2.0);
        cpShapeSetFriction(wall, 1.0);
        cpShapeSetElasticity(wall, 0.0);
        cpSpaceAddShape(GetSpace(), wall);

        ecs_set(world, collider, TileCollider, {.tileId = tileId, .a = {prev.x, prev.y}, .b = {next.x, next.y}, .shape = wall});
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
