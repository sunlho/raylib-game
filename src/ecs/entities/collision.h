#ifndef ECS_COLLISION_ENTITY_H
#define ECS_COLLISION_ENTITY_H

#include "../components/basic.h"
#include "box2d/box2d.h"
#include "constants.h"
#include "context.h"
#include "flecs.h"
#include "raylib.h"
#include "raymath.h"

ecs_entity_t GetCollisionEntityGroup();
void PolygonCollisionEntity(ecs_world_t *world, tmx_object *collision, double posX, double posY);

#endif // ECS_COLLISION_ENTITY_H

#ifdef ECS_COLLISION_ENTITY_IMPLEMENTATION
#ifndef ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY
#define ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(Velocity);

ecs_entity_t GetCollisionEntityGroup() {
    ecs_entity_t group = ecs_lookup(GetWorld(), TILE_COLLIDER_GROUP);
    if (!group || !ecs_is_valid(GetWorld(), group)) {
        group = ecs_entity(GetWorld(), {.name = TILE_COLLIDER_GROUP});
    }
    return group;
}

void PolygonCollisionEntity(ecs_world_t *world, tmx_object *obj, double posX, double posY) {
    int len = obj->content.shape->points_len;
    b2Vec2 *points = (b2Vec2 *)malloc(sizeof(b2Vec2) * len);
    for (int i = 0; i < len; i++) {
        points[i] = (b2Vec2){obj->content.shape->points[len - i - 1][0] + posX, obj->content.shape->points[len - i - 1][1] + posY};
    }
    // for (size_t i = 0; i < len; i++) {
    //     DrawLineEx(
    //         (Vector2){points[i].x, points[i].y},
    //         (Vector2){
    //             points[(i + 1) % len].x,
    //             points[(i + 1) % len].y,
    //         },
    //         1,
    //         RED);
    // }

    b2BodyDef bodyDef = b2DefaultBodyDef();
    b2BodyId bodyId = b2CreateBody(GetPhyWorld(), &bodyDef);
    b2ChainDef chainDef = b2DefaultChainDef();
    chainDef.points = points;
    chainDef.count = obj->content.shape->points_len;
    chainDef.isLoop = true;
    b2ChainId myChainId = b2CreateChain(bodyId, &chainDef);
}

#endif // ECS_COLLISION_ENTITY_IMPLEMENTATION_ONLY
#endif // ECS_COLLISION_ENTITY_IMPLEMENTATION
