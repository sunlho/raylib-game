#ifndef ECS_PHYSICS_BASIC_H
#define ECS_PHYSICS_BASIC_H

#include "box2d/box2d.h"
#include "context.h"
#include "flecs.h"

void PhysicsBasicImport(ecs_world_t *world);

#endif // ECS_PHYSICS_BASIC_H

#ifdef ECS_PHYSICS_BASIC_IMPLEMENTATION
#ifndef ECS_PHYSICS_BASIC_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(PlayerData);
extern ECS_COMPONENT_DECLARE(PlayerPhysics);

void PhysicsStepSystem(const ecs_iter_t *it) {
    b2World_Step(GetPhyWorld(), it->delta_time, 4);
}

void SyncPositionSystem(ecs_iter_t *it) {
    Position *p = ecs_field(it, Position, 0);
    PlayerPhysics *pb = ecs_field(it, PlayerPhysics, 2);
    Camera2D *camera = GetCamera();
    for (int i = 0; i < it->count; i++) {
        b2Vec2 pos = b2Body_GetPosition(pb[i].body);
        p[i].x = pos.x;
        p[i].y = pos.y;
        Vector2 target = Vector2Lerp(camera->target, (Vector2){p[i].x, p[i].y}, 0.1f);
        float halfW = (SCREEN_WIDTH / 2.0f) / camera->zoom;
        float halfH = (SCREEN_HEIGHT / 2.0f) / camera->zoom;
        if (target.x < halfW)
            target.x = halfW;
        if (target.y < halfH)
            target.y = halfH;
        if (target.x > GetWorldSize().x - halfW)
            target.x = GetWorldSize().x - halfW;
        if (target.y > GetWorldSize().y - halfH)
            target.y = GetWorldSize().y - halfH;
        camera->target = target;
    }
}

void PhysicsBasicImport(ecs_world_t *world) {
    ECS_MODULE(world, PhysicsBasicImport);
    ECS_SYSTEM(world, PhysicsStepSystem, EcsPreUpdate);
    ECS_SYSTEM(world, SyncPositionSystem, EcsOnUpdate, Position, PlayerData, PlayerPhysics);
}

#endif // ECS_PHYSICS_BASIC_IMPLEMENTATION_ONLY
#endif // ECS_PHYSICS_BASIC_IMPLEMENTATION
