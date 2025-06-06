#ifndef ECS_PHYSICS_WORLD_H
#define ECS_PHYSICS_WORLD_H

#include "box2d/box2d.h"
#include "raylib.h"

b2WorldId InitPhysicsWorld(ecs_world_t *world);

#endif // ECS_PHYSICS_WORLD_H

#ifdef ECS_PHYSICS_WORLD_IMPLEMENTATION
#ifndef ECS_PHYSICS_WORLD_IMPLEMENTATION_ONLY
#define ECS_PHYSICS_WORLD_IMPLEMENTATION_ONLY

void B2HexColorToRaylibColor(b2HexColor color, Color *c) {
    c->r = (color >> 16) & 0xFF;
    c->g = (color >> 8) & 0xFF;
    c->b = color & 0xFF;
    c->a = 255;
}

void b2DrawPolygon(const b2Vec2 *vertices, int vertexCount, b2HexColor color, void *context) {
    Color c;
    B2HexColorToRaylibColor(color, &c);
    for (size_t i = 0; i < vertexCount; i++) {
        DrawLineEx(
            (Vector2){vertices[i].x, vertices[i].y},
            (Vector2){
                vertices[(i + 1) % vertexCount].x,
                vertices[(i + 1) % vertexCount].y,
            },
            2,
            c);
    }
}

void b2DrawSegment(b2Vec2 p1, b2Vec2 p2, b2HexColor color, void *context) {
    Color c;
    B2HexColorToRaylibColor(color, &c);
    DrawLineEx((Vector2){p1.x, p1.y}, (Vector2){p2.x, p2.y}, 2, c);
}

void b2DrawCircle(b2Vec2 center, float radius, b2HexColor color, void *context) {
    Color c;
    B2HexColorToRaylibColor(color, &c);
    DrawCircleLinesV((Vector2){center.x, center.y}, radius, c);
}

void b2DrawString(b2Vec2 p, const char *s, b2HexColor color, void *context) {
    DrawText(s, (int)p.x, (int)p.y, 10, RED);
}

b2WorldId InitPhysicsWorld(ecs_world_t *world) {
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = (b2Vec2){0.0f, 0.0f};
    b2WorldId phy_world = b2CreateWorld(&world_def);

    return phy_world;
}

b2DebugDraw GetDefaultDebugDraw(void) {
    b2DebugDraw debug_draw = b2DefaultDebugDraw();
    debug_draw.drawBounds = true;
    debug_draw.drawShapes = true;
    debug_draw.DrawPolygonFcn = b2DrawPolygon;
    debug_draw.DrawSegmentFcn = b2DrawSegment;
    debug_draw.DrawStringFcn = b2DrawString;
    debug_draw.DrawCircleFcn = b2DrawCircle;

    return debug_draw;
}

#endif // ECS_PHYSICS_WORLD_IMPLEMENTATION_ONLY
#endif // ECS_PHYSICS_WORLD_IMPLEMENTATION
