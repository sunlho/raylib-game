#ifndef ECS_COLLISION_SYSTEM_H
#define ECS_COLLISION_SYSTEM_H

#include "../components/basic.h"
#include <flecs.h>

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(PlayerData);
extern ECS_COMPONENT_DECLARE(TileCollider);

bool PointInPolygon(Vector2 point, double **polygon, int pointCount) {
    bool inside = false;
    for (int i = 0, j = pointCount - 1; i < pointCount; j = i++) {
        double xi = polygon[i][0], yi = polygon[i][1];
        double xj = polygon[j][0], yj = polygon[j][1];

        bool intersect = ((yi > point.y) != (yj > point.y)) && (point.x < (xj - xi) * (point.y - yi) / (yj - yi + 1e-10) + xi);
        if (intersect) {
            inside = !inside;
        }
    }
    return inside;
}

static inline void CollisionSystem(ecs_iter_t *it) {
    Position *p = ecs_field(it, Position, 0);
    Velocity *v = ecs_field(it, Velocity, 1);
    PlayerData *playerData = ecs_field(it, PlayerData, 2);

    for (int i = 0; i < it->count; i++) {
        Vector2 nextPos = {
            p[i].x + v[i].x,
            p[i].y + v[i].y};

        ecs_iter_t colliders = ecs_children(GetWorld(), GetCollisionEntityGroup());
        bool isColliding = false;
        while (ecs_children_next(&colliders)) {
            for (int j = 0; j < colliders.count; j++) {
                ecs_entity_t collider = colliders.entities[j];
                const Position *cp = ecs_get(GetWorld(), collider, Position);
                const TileCollider *tc = ecs_get(GetWorld(), collider, TileCollider);
                if (!cp || !tc)
                    continue;

                Vector2 localPos = {
                    nextPos.x - cp->x,
                    nextPos.y - cp->y};
                isColliding = PointInPolygon(localPos, tc->points, tc->pointCount);
            }
        }

        if (isColliding) {
            v[i].x = 0;
            v[i].y = 0;
        }
    }
}

static inline void CollisionImport(ecs_world_t *world) {
    ECS_MODULE(world, Collision);
    ECS_SYSTEM(world, CollisionSystem, EcsOnUpdate, Position, Velocity, PlayerData);
}

#endif // ECS_COLLISION_SYSTEM_H