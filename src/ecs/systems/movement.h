#ifndef ECS_MOVEMENT_SYSTEM_H
#define ECS_MOVEMENT_SYSTEM_H

#include "../components/basic.h"
#include <flecs.h>

void MovementImport(ecs_world_t *world);

#endif // ECS_MOVEMENT_SYSTEM_H

#ifdef ECS_COLLISION_SYSTEM_IMPLEMENTATION
#ifndef ECS_COLLISION_SYSTEM_IMPLEMENTATION_ONLY
#define ECS_COLLISION_SYSTEM_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);
extern ECS_COMPONENT_DECLARE(PlayerData);
extern ECS_COMPONENT_DECLARE(TileCollider);

bool CheckCollision(Rectangle a, Rectangle b) {
    return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}

void MovementSystem(ecs_iter_t *it) {
    Position *p = ecs_field(it, Position, 0);
    Velocity *v = ecs_field(it, Velocity, 1);
    PlayerData *playerData = ecs_field(it, PlayerData, 2);

    for (int i = 0; i < it->count; i++) {
        Vector2 delta = {v[i].x, v[i].y};

        Vector2 tryX = {p[i].x + delta.x, p[i].y};
        Vector2 tryY = {p[i].x, p[i].y + delta.y};

        ecs_iter_t colliders = ecs_children(GetWorld(), GetCollisionEntityGroup());
        bool isColliding = false;

        Rectangle tryXBox = {tryX.x, tryX.y, playerData[i].width, playerData[i].height};
        Rectangle tryYBox = {tryY.x, tryY.y, playerData[i].width, playerData[i].height};
        bool collideX = false;
        bool collideY = false;
        while (ecs_children_next(&colliders)) {
            for (int j = 0; j < colliders.count; j++) {
                ecs_entity_t collider = colliders.entities[j];
                const TileCollider *tc = ecs_get(GetWorld(), collider, TileCollider);
                if (!tc)
                    continue;
                collideX = CheckCollisionRecs(tryXBox, tc->aabb);
                collideY = CheckCollisionRecs(tryYBox, tc->aabb);
                if (collideX || collideY) {
                    isColliding = true;
                    break;
                }
            }
        }

        if (!collideX) {
            p[i].x += delta.x;
        }
        if (!collideY) {
            p[i].y += delta.y;
        }
    }
}

void MovementImport(ecs_world_t *world) {
    ECS_MODULE(world, Movement);
    ECS_SYSTEM(world, MovementSystem, EcsOnUpdate, Position, Velocity, PlayerData);
}

#endif // ECS_COLLISION_SYSTEM_IMPLEMENTATION_ONLY
#endif // ECS_COLLISION_SYSTEM_IMPLEMENTATION