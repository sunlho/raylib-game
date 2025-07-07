#ifndef ECS_PHYSICS_BASIC_H
#define ECS_PHYSICS_BASIC_H

#include "box2d/box2d.h"
#include "flecs.h"
#include "raymath.h"

#include "context.h"

void PhysicsBasicImport(ecs_world_t *world);

#endif // ECS_PHYSICS_BASIC_H

#ifdef ECS_PHYSICS_BASIC_IMPLEMENTATION
#ifndef ECS_PHYSICS_BASIC_IMPLEMENTATION_ONLY

extern ECS_COMPONENT_DECLARE(Position);
extern ECS_COMPONENT_DECLARE(PlayerPhysics);

static void PhysicsStepSystem(const ecs_iter_t *it) {
    b2World_Step(global_ctx.phy_world, it->delta_time, 4);
}

static void SyncPositionSystem(ecs_iter_t *it) {
    Position *p = ecs_field(it, Position, 0);
    PlayerPhysics *pb = ecs_field(it, PlayerPhysics, 1);
    TilemapDrawable *td = ecs_field(it, TilemapDrawable, 2);

    Camera2D *camera = GetCamera();

    for (int i = 0; i < it->count; i++) {
        b2Vec2 pos = b2Body_GetPosition(pb[i].body);
        p[i].x = pos.x;
        p[i].y = pos.y;

        Vector2 target = Vector2Lerp(camera->target, (Vector2){p[i].x, p[i].y}, 1.0f);

        float half_w = (SCREEN_WIDTH / 2.0f) / camera->zoom;
        float half_h = (SCREEN_HEIGHT / 2.0f) / camera->zoom;

        if (target.x < half_w)
            target.x = half_w;
        if (target.y < half_h)
            target.y = half_h;
        if (target.x > GetWorldSize().width - half_w)
            target.x = GetWorldSize().width - half_w;
        if (target.y > GetWorldSize().height - half_h)
            target.y = GetWorldSize().height - half_h;

        camera->target = target;

        td[i].sort_y = pos.y + 15.0f;
    }
}

void PhysicsBasicImport(ecs_world_t *world) {
    ECS_MODULE(world, PhysicsBasic);
    ECS_SYSTEM(world, SyncPositionSystem, EcsOnUpdate, Position, PlayerPhysics, TilemapCom(TilemapDrawable));
    ECS_SYSTEM(world, PhysicsStepSystem, EcsPreUpdate);
}

#endif // ECS_PHYSICS_BASIC_IMPLEMENTATION_ONLY
#endif // ECS_PHYSICS_BASIC_IMPLEMENTATION
